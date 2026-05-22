#pragma once 

#include "oj_model.hpp"
#include "oj_view.hpp"
#include "../comm/util.hpp"
#include "../source/http/httpClient.hpp"
#include <mutex>

class Machine
{
public:
    std::string ip;
    std::string port;
    int load;    // 负载
    std::mutex *mutex;
    Machine() :ip(""), port(""), load(0), mutex(nullptr)
    {}
    ~Machine(){}
    // 提高机器的负载
    void IncreaseLoad()
    {
        mutex->lock();
        load++;
        mutex->unlock();
    }
    // 降低机器的负载
    void DecreaseLoad()
    {
        mutex->lock();
        load--;
        mutex->unlock();
    }
};

const std::string service_machine_path = "./conf/service_machine.conf";
class LoadBalance
{
private:
    std::vector<Machine> machines;
    std::vector<int> online_machines;
    std::vector<int> offline_machines;
    std::mutex mtx;
public:
    LoadBalance()
    {assert(DownLoadMachines(service_machine_path)); }
    ~LoadBalance(){}
    // 下载机器信息
    bool DownLoadMachines(const std::string &path)
    {
        std::ifstream ifs(path);
        if(!ifs.is_open())
        {
            LOG(FATAL) << "open file failed: " << path;
            return false;
        }
        // 逐行读取配置文件的机器信息
        std::string line;
        while(std::getline(ifs, line))
        {
            // ip:port --- 解析出ip和port
            std::vector<std::string> tokens;
            StringUtil::Split(line, ":", &tokens);
            if(tokens.size() != 2)
            {
                LOG(ERROR) << "invalid line: " << line;
                return false;
            }
            // 构建机器对象
            Machine machine;
            machine.ip = tokens[0];
            machine.port = tokens[1];
            machine.mutex = new std::mutex();
            // 添加到机器列表和在线机列表中
            online_machines.push_back(machines.size());
            machines.push_back(machine);
        }
        LOG(INFO)<< "成功加载所有机器……";
        ifs.close();
        return true;
    }
    // 负载均衡的分配机器
    bool SmartChoice(int* id, Machine** machine)
    {
        mtx.lock();
        if(online_machines.empty())
        {
            mtx.unlock();
            LOG(ERROR) << "no online machine, 请尽快检查!";
            raise(SIGUSR1);
            return false;
        }
        // 遍历找到负载最小的机器
        *id = online_machines[0];
        *machine = &machines[*id];
        for(size_t i = 1; i < online_machines.size(); i++)
        {
            if(machines[online_machines[i]].load < machines[*id].load)
            {
                *id = online_machines[i];
                *machine = &machines[*id];
            }
        }
        mtx.unlock();
        return true;
    }
    // 设置机器离线
    void OfflineMachine(int id)
    {
        mtx.lock();
        for(auto i = online_machines.begin(); i != online_machines.end(); i++)
        {
            if(*i == id)
            {
                machines[id].load = 0;
                online_machines.erase(i);
                offline_machines.push_back(id);
                break;
            }
        }
        mtx.unlock();
    }
    // 一键恢复机器在线
    void OnlineMachine()
    {
        mtx.lock();
        for(auto i = offline_machines.begin(); i != offline_machines.end(); i++)
        {
            online_machines.push_back(*i);
        }
        offline_machines.clear();
        LOG(INFO)<< "恢复所有机器在线……";

        mtx.unlock();
    }
    // 展示所有机器
    void ShowMachines()
    {
        mtx.lock();
        std::cout<< "在线机器: ";
        for(auto on : online_machines)
        {
            std::cout<< on<< ", ";
        }
        std::cout<< std::endl;
        std::cout<< "离线机器: ";
        for(auto off : offline_machines)
        {
            std::cout<< off<< ", ";
        }
        std::cout<< std::endl;
        mtx.unlock();
    }
};

class Control 
{
private:
    Model model; // 提供问题数据
    View view;   // 提供渲染页面
    LoadBalance lb; // 负载均衡器
public:
    Control(){}
    ~Control(){}
    // 一键恢复机器在线
    void OnlineMachine()
    {
        lb.OnlineMachine();
    }
    // 获取所有题目的网页
    bool AllQuestions(std::string *html)
    {
        std::vector<struct Question> questions;
        if(!model.GetAllQuestions(&questions))
        {
            LOG(ERROR)<< "获取题目失败!";
            view.Error("获取所有题目列表页面失败!", html);
            return false;
        }
        view.AllQuestionHtml(questions, html);
        return true;
    }
    // 获取单个题目的做题网页
    bool Question(const std::string &number, std::string *html)
    {
        struct Question question;
        if(!model.GetQuestion(number, &question))
        {
            LOG(ERROR)<< "获取题目失败!";
            view.Error("获取单个题目页面失败!", html);
            return false;
        }
        view.OneQuestionHtml(question, html);
        return true;
    }

    // 构建,发送并接收OJ请求
    void Judge(const std::string &number, const std::string &in_json, std::string *out_json)
    {
        struct Question question;
        if(!model.GetQuestion(number, &question))
            return; 
        // 解析输入的json
        Json::Value in_value;
        Json::Reader reader;
        if(!reader.parse(in_json, in_value))
            return;
        std::string code = in_value["code"].asString();
        // 构建编译运行参数
        Json::Value compile_value;
        Json::FastWriter writer;
        compile_value["code"] = code + "\n" + question.tail;  // 真正拿去编译的代码,加上测试用例
        compile_value["input"] = in_value["input"];
        compile_value["cpu_limit"] = question.cpu_limit;
        compile_value["mem_limit"] = question.mem_limit;
        std::string compile_in_json = writer.write(compile_value);
        // 获取机器
        while(true)
        {
            int id = -1;
            Machine* machine = nullptr;
            if(!lb.SmartChoice(&id, &machine))
            {
                LOG(ERROR)<< "没有可用的机器!";
                return;
            }
            // 向机器申请网络服务
            HttpClient client(machine->ip, machine->port);
            machine->IncreaseLoad(); // 增加负载
            if(auto ret = client.HttpPost("/compile_run", compile_in_json))
            {
                machine->DecreaseLoad(); // 调用完了,减少负载
                if(ret != nullptr && ret->_statu == 200)
                {
                    *out_json = ret->_body;
                    LOG(INFO) << "编译运行成功……";
                    break;
                }
            }
            // 申请服务失败，或者服务器返回错误---编译运行失败
            // 设置机器为离线
            LOG(ERROR)<< "当前申请的主机："<< id<< " 可能已经离线，尽快检查！";
            lb.OfflineMachine(id);
            lb.ShowMachines();
        }
        
    }
};
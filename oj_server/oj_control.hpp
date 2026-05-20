#pragma once 

#include "oj_model.hpp"
#include "oj_view.hpp"
#include "../compile_server/compile_run.hpp"
#include <mutex>

class Machine
{
public:
    std::string ip;
    std::string port;
    int load;    // 负载
    Mutex *mutex;
    Machine() :ip(""), port(""), load(0), mutex(nullptr)
    {}
    ~Machine(){}
    // 提高机器的负载
    void IncreaseLoad()
    {
        mutex->Lock();
        load++;
        mutex->unlock();
    }
    // 降低机器的负载
    void DecreaseLoad()
    {
        mutex->Lock();
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
            machine.mutex = new Mutex();
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
                online_machines.erase(i);
                offline_machines.push_back(id);
                break;
            }
        }
        mtx.unlock();
    }
    // 恢复机器在线
    void OnlineMachine(int id)
    {}
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
    }
};
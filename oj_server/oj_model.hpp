#pragma once

#include "../comm/log.hpp"
#include "../comm/util.hpp"
#include <unordered_map>
#include <vector>
#include <assert.h>
#include <algorithm>

using namespace LogModule;

/* 文件版本 */

// 题目结构体
struct Question
{
    std::string id;        // 题号
    std::string title;     // 标题
    std::string dificulty; // 难度
    int cpu_limit;         // cpu限制
    int mem_limit;         // 内存限制
    std::string desc;      // 描述
    std::string header;    // 预设代码
    std::string tail;      // 测试代码
};

const std::string question_list = "./question/question.list";
const std::string question_dir = "./question/";

class Model
{
private:      //      题目编号      题目细节
    std::unordered_map<std::string, Question> question_map;
public:
    Model()
    {
        assert(LoadQuestions(question_list));
    }
    ~Model(){}
    // 加载所有题目
    bool LoadQuestions(const std::string &path)
    {
        std::ifstream ifs(path);
        if(!ifs.is_open())
        {
            LOG(FATAL)<< "IFSTREAM open file failed: "<< path;
            return false;
        }
        std::string line;
        while(std::getline(ifs, line))
        {
            // 解析 读取 问题细节数据
            /*id title dificulty cpu_limit mem_limit*/
            std::vector<std::string> array;
            size_t count = StringUtil::Split(line, " ", &array);
            if(count != 5)
            {
                LOG(FATAL)<< "Split line failed: "<< line<< ": "<< count;
                return false;
            }
            Question question;
            question.id = array[0];
            question.title = array[1];
            question.dificulty = array[2];
            question.cpu_limit = std::stoi(array[3]);
            question.mem_limit = std::stoi(array[4]);
            /*desc header tail*/
            std::string desc_path = question_dir + question.id + "/desc";
            if(!FileUtil::ReadFile(desc_path, &question.desc))
            {
                LOG(FATAL)<< "Read file failed: "<< desc_path;
                return false;
            }
            std::string header_path = question_dir + question.id + "/header.cpp";
            if(!FileUtil::ReadFile(header_path, &question.header))
            {
                LOG(FATAL)<< "Read file failed: "<< header_path;
                return false;
            }
            std::string tail_path = question_dir + question.id + "/tail.cc";
            if(!FileUtil::ReadFile(tail_path, &question.tail))  
            {
                LOG(FATAL)<< "Read file failed: "<< tail_path;  
                return false;   
            }
            // 保存 插入
            question_map.emplace(question.id, question);
        }
       
        LOG(INFO)<< "Load questions success: "<< question_map.size();
        return true;
    }
    // 获取所有题目细节
    bool GetAllQuestions(std::vector<Question> *questions)
    {
        if(question_map.size() == 0)
        {
            LOG(FATAL)<< "Question map is empty";
            return false;
        }
        for(auto &item : question_map)
        {
            questions->push_back(item.second);
        }
        // 根据 id 排序
        std::sort(questions->begin(), questions->end(), [](const Question &a, const Question &b)
        {
            return a.id < b.id;
        });
        return true;
    }
    // 获取单个题目
    bool GetQuestion(const std::string &id, Question *question)
    {
        auto iter = question_map.find(id);
        if(iter == question_map.end())
        {
            LOG(FATAL)<< "Question not found: "<< id;
            return false;
        }
        *question = iter->second;
        return true;
    }
};
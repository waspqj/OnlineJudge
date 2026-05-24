#pragma once

#include "../comm/log.hpp"
#include "../comm/util.hpp"
#include <vector>
#include <assert.h>
#include <algorithm>
#include <mysql/mysql.h>

using namespace LogModule;

/* 数据库版 */

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

// const char *host, const char *user, const char *passwd, const char *db, unsigned int port
const std::string host = "127.0.0.1";
const std::string user = "oj_client";
const std::string passwd = "123456";
const std::string db = "oj";
const std::string table = "oj_questions";
const unsigned int port = 3306;

class Model
{
public:
    Model(){}
    ~Model(){}

    // 数据库操作
    bool QueryMysql(const std::string &sql, std::vector<Question> *out)
    {
        // 创建mysql句柄
        MYSQL *mysql = mysql_init(nullptr);
        // 连接数据库
        if(nullptr == mysql_real_connect(mysql, host.c_str(), user.c_str(), passwd.c_str(), db.c_str(), port, nullptr, 0))
        {
            LOG(ERROR) << "数据库连接失败";
            return false;
        }
        // 执行sql
        if(0 != mysql_query(mysql, sql.c_str()))
        {
            LOG(ERROR) << "数据库执行sql失败";
            return false;
        }
        // 获取结果
        MYSQL_RES *result = mysql_store_result(mysql);
        int rows = mysql_num_rows(result);
        int fields = mysql_num_fields(result);
        
        Question question;
        for(int i = 0; i < rows; i++)
        {
            MYSQL_ROW row = mysql_fetch_row(result);
            question.id = row[0];
            question.title = row[1];
            question.dificulty = row[2];
            question.desc = row[3];
            question.header = row[4];
            question.tail = row[5];
            question.cpu_limit = atoi(row[6]);
            question.mem_limit = atoi(row[7]);
            out->push_back(question);
        }
        mysql_free_result(result);
        mysql_close(mysql);
        return true;
    }

    // 获取所有题目细节
    bool GetAllQuestions(std::vector<Question> *questions)
    {
        std::string sql = "select * from ";
        sql += table;
        return QueryMysql(sql, questions);
    }
    // 获取单个题目
    bool GetQuestion(const std::string &id, Question *question)
    {
        std::string sql = "select * from ";
        sql += table;
        sql += " where id = ";
        sql += id;
        std::vector<Question> questions;
        if(!QueryMysql(sql, &questions))
        {
            LOG(ERROR) << "GET QUESTION ERROR……";
            return false;
        }
        if (questions.size() == 0)
        {
            LOG(ERROR) << "GET QUESTION ERROR……";
            return false;
        }
        *question = questions[0];
        if(questions.size() > 1)
        {
            LOG(ERROR) << "GET TOO MUCH QUESTIONS……";
        }
        return true;
    }
};
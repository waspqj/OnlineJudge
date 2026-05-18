#pragma once

#include "compiler.hpp"
#include "runner.hpp"
#include "jsoncpp/json/json.h"

class CompileAndRun
{
public:
    CompileAndRun(){}
    ~CompileAndRun(){}

    static std::string StatuToReason(int status, const std::string &filename)
    {
        std::string reason;
        switch(status)
        {
            case 0: return "运行成功";
            case -1: return "输入的代码为空";
            case -2: return "未知错误";
            case -3: 
            FileUtil::ReadFile(PathUtil::Compile_err(filename), &reason); 
            break;
            case SIGABRT:  // 6
            return "超出内存限制";
            case SIGXCPU: 
            return "超出时间限制";
            case SIGFPE: 
            return "浮点数溢出";
            case SIGSEGV: 
            return "段错误";
            default:
            return "运行崩溃";
        } 
        return reason;
    }
    // 解析in_json：{"code": "#include<……", "input": "……", 
    // "cpu_limit": 1, "mem_limit": 10*1024}
    // compile and run
    // 构建out_json：{"status": "0", "reason": "……", "stdout": "", "stderr": ""}
    static void Run(const std::string &in_json, std::string *out_json)
    {
        // 解析in_json
        Json::Value in_json_value;
        Json::Reader reader;
        reader.parse(in_json, in_json_value);
        std::string code = in_json_value["code"].asString();
        std::string input = in_json_value["input"].asString();
        int cpu_limit = in_json_value["cpu_limit"].asInt();
        int mem_limit = in_json_value["mem_limit"].asInt();
        // 构建out_json
        Json::Value out_json_value;
        int status = 0;
        std::string reason;
        std::string std_out;
        std::string std_err;
        int run_ret;
        std::string filename;
        // 将代码写入文件
        if(code.empty())
        {
            status = -1; // 表示输入的代码为空
            goto END;
        }
        filename = FileUtil::GetUniqueFilename();
        if(FileUtil::WriteFile(PathUtil::Src(filename), code) == false)
        {
            status = -2; // 未知错误
            goto END;
        }
        // compile
        if(Compiler::Compile(PathUtil::Src(filename)) == false)
        {
            status = -3; // 编译失败
            goto END;
        }
        // run
        run_ret = Runner::Run(filename, cpu_limit, mem_limit);
        if(run_ret < 0)
        {
            status = -2; // 未知错误
        }
        else if(run_ret == 0)
        {
            status = 0;
            FileUtil::ReadFile(PathUtil::Stdout(filename), &std_out);
            FileUtil::ReadFile(PathUtil::Run_err(filename), &std_err);
        }
        else
        {
            status = run_ret; // 运行崩溃
        }
        // 构建out_json
        END:
        out_json_value["status"] = status;
        out_json_value["reason"] = StatuToReason(status, filename);
        if(status == 0)
        {
            out_json_value["stdout"] = std_out;
            out_json_value["stderr"] = std_err;
        }
        Json::FastWriter writer;
        *out_json = writer.write(out_json_value);
    }
};
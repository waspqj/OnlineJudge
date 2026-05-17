#pragma once

#include "compiler.hpp"
#include "runner.hpp"

class CompileAndRun
{
public:
    CompileAndRun(){}
    ~CompileAndRun(){}

    // 解析in_json：{"code": "#include<……", "input": "……", 
    // "cpu_limit": 1, "mem_limit": 10*1024}
    // compile and run
    // 构建out_json：{"status": "0", "reason": "……", "stdout": "", "stderr": ""}
    static void Run(const std::string &in_json, const std::string *out_json)
    {

    }
};
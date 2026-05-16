#pragma once
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define temp_path "./temp/"
struct PathUtil 
{
    // 添加后缀
    static std::string AddSuffix(const std::string& filename, const std::string& suffix)
    {
        std::string pathname = temp_path;
        pathname += filename;
        pathname += suffix;
        return pathname;
    }
    // 编译时对应文件
    // 构建并得到源文件完整路径
    static std::string Src(const std::string& filename) 
    {
        return PathUtil::AddSuffix(filename, ".cpp");
    }
    // 构建并得到可执行文件完整路径
    static std::string Exe(const std::string& filename)
    {
        return PathUtil::AddSuffix(filename, ".exe");
    }
    // 构建并得到编译时错误文件完整路径
    static std::string Compile_err(const std::string& filename)
    {
        return PathUtil::AddSuffix(filename, ".compile_err");
    }
    // 运行时对应文件
    // 构建并得到标准输出文件完整路径
    static std::string Stdout(const std::string& filename)
    {
        return PathUtil::AddSuffix(filename, ".stdout");
    }
    // 构建并得到标准输入文件完整路径
    static std::string Stdin(const std::string& filename)
    {
        return PathUtil::AddSuffix(filename, ".stdin");
    }
    // 构建并得到运行时错误文件完整路径
    static std::string Run_err(const std::string& filename)
    {
        return PathUtil::AddSuffix(filename, ".run_err");
    }

};

struct FileUtil
{
    static bool IsFileExits(const std::string &filename)
    {
        struct stat st;
        if(stat(filename.c_str(), &st) == 0)
            return true;
        return false;
    }
};


#pragma once
#include <string>
#include <sys/types.h>
#include <sys/stat.h>

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
    // 构建并得到标准错误文件完整路径
    static std::string Stderr(const std::string& filename)
    {
        return PathUtil::AddSuffix(filename, ".stderr");
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

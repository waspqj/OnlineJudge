#pragma once
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <atomic>
#include <vector>
#include "log.hpp"
#include "jsoncpp/json/json.h"


struct TimeUtil
{
public:
    // 获得毫秒级别时间戳
    static std::string GetTimeMs()
    {
        struct timeval _time;
        gettimeofday(&_time, nullptr);
        return std::to_string(_time.tv_sec * 1000 + _time.tv_usec / 1000);
    }
};

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
    static std::string GetUniqueFilename()
    {
        std::atomic_uint id(0);
        id++;
        std::string ms = TimeUtil::GetTimeMs();
        std::string sid = std::to_string(id);
        return ms + "_" + sid;
    }
    // 读取文件数据
    static bool ReadFile(const std::string &filename, std::string *buf)
    {
        // 以二进制模式打开文件
        std::ifstream ifs(filename, std::ios::binary);
        if(!ifs.is_open())
        {
            // LOG(ERROR)<< "IFSTREAM open file failed: "<< filename;
            return false;
        }
        // 获取文件数据大小
        size_t fsize = 0;
        ifs.seekg(0, ifs.end); // 跳转读写位置到文件末尾
        fsize = ifs.tellg();   // 获取离起始位置的偏移量
        ifs.seekg(0, ifs.beg); // 跳转回去
        buf->resize(fsize);          // 预留文件数据大小的空间
        ifs.read(&(*buf)[0], fsize); // 读取文件数据
        if(!ifs.good())
        {
            // LOG(ERROR) <<"IFSTREAM read file failed: "<< filename;
            ifs.close();
            return false;
        }
        ifs.close();
        return true;
    }
    // 向文件写入数据
    static bool WriteFile(const std::string &filename, const std::string &buf)
    {
        // 以二进制模式打开文件  并且覆盖原先内容
        std::ofstream ofs(filename, std::ios::binary | std::ios::trunc);
        if(!ofs.is_open())
        {
            // LOG(ERROR)<< "OFSTREAM open file failed: "<< filename;
            return false;
        }
        // 向文件写入buf中的数据
        ofs.write(buf.c_str(), buf.size());
        if(!ofs.good())
        {
            // LOG(ERROR) <<"OFSTREAM write file failed: "<< filename;
            ofs.close();
            return false;
        }
        ofs.close();
        return true;
    }
    static bool IsFileExits(const std::string &filename)
    {
        struct stat st;
        if(stat(filename.c_str(), &st) == 0)
            return true;
        return false;
    }
};

struct StringUtil
{
    // 分割字符串
    static size_t Split(const std::string &str, const std::string &sep, std::vector<std::string> *array)
    {
        size_t offset = 0;
        while (offset < str.size())
        {
            std::string tmp;
            size_t pos = str.find(sep, offset);
            if (pos == std::string::npos) // 最后一段
            {
                tmp = str.substr(offset);
                if (tmp.size() > 0)
                    array->push_back(tmp);
                break;
            }
            else if (pos - offset > 0)
                tmp = str.substr(offset, pos - offset);

            if (tmp.size() > 0)
                array->push_back(tmp);
            offset = pos + sep.size();
        }
        return array->size();
    }
};
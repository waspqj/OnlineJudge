#pragma once

#include "../comm/log.hpp"
#include "../comm/util.hpp"

using namespace LogModule;
// 只负责代码的编译

class Compiler 
{
public:
    Compiler() {}
    ~Compiler() {}
    static bool Compile(const std::string &filename)
    {
        pid_t pid = fork();
        if(pid < 0)
        {
            LOG(ERROR) << "fork error";
            return false;
        }
        else if(pid == 0) // 子进程
        {
            // 创建一个文件来装程序运行时的标准错误，创建、打开，最后重定向标准错误到这个文件
            // 掩码设为0，创建的文件权限好设置
            umask(0);
            int errfd = open(PathUtil::Compile_err(filename).c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
            if(errfd < 0)
            {
                LOG(ERROR) << "open error";
                exit(1);
            }
            dup2(errfd, 2);
            // 程序替换：g++生成可执行
            execlp("g++", "g++", "-o", PathUtil::Exe(filename).c_str(), PathUtil::Src(filename).c_str(), "-D", "COMPILER_ONLINE", "-std=c++11", NULL);
            LOG(ERROR) << "execlp error";
            exit(2);
        }
        else // 父进程
        {
            waitpid(pid, NULL, 0);
            if(FileUtil::IsFileExits(PathUtil::Exe(filename)))
            {
                LOG(INFO) << "Compile [" << filename << "] success";
                return true;
            }
        }
        return false;
    }

};
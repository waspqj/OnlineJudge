#pragma once

#include "../comm/util.hpp"
#include "../comm/log.hpp"

using namespace LogModule;
// 只需要运行可执行文件

class Runner 
{
public:
    Runner(){}
    ~Runner(){}
public:
    static int Run(const std::string& filename)
    {
        // 创建并打开三个文件,分别对应标准 输入 输出 错误, 重定向
        umask(0);
        int fd_in = open(PathUtil::Stdin(filename).c_str(), O_RDONLY | O_CREAT, 0644);
        int fd_out = open(PathUtil::Stdout(filename).c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        int fd_err = open(PathUtil::Run_err(filename).c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if(fd_in < 0 || fd_out < 0 || fd_err < 0)
        {
            LOG(ERROR) << "open error";
            return -1;
        }
        // 用子进程运行可执行
        pid_t pid = fork();
        if(pid < 0)
        {
            close(fd_in);
            close(fd_out);
            close(fd_err);
            LOG(ERROR) << "fork error";
            return -2;
        }
        else if(pid == 0) // 子进程
        {
            // 重定向
            dup2(fd_in, 0);
            dup2(fd_out, 1);
            dup2(fd_err, 2);
            // 程序替换,运行可执行
            execl(PathUtil::Exe(filename).c_str(), PathUtil::Exe(filename).c_str(), NULL);
            LOG(ERROR) << "execl error";
            exit(1);
        } 
        else // 父进程
        {
            close(fd_in);
            close(fd_out);
            close(fd_err);
            int status = 0;
            waitpid(pid, &status, 0);
            // 执行异常一定会收到信号, 返回信号码
            LOG(INFO) << "run end, info: "<< (status & 0X7F);
            return status & 0X7F;
        }
    }
};
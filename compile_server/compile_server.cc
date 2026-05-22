#include "compile_run.hpp"
#include "../source/http/Http.hpp"
#include "../source/http/Daemon.hpp"

//  包装成网络服务
int main(int argc, char const *argv[])
{
    if(argc != 2)
    {
        LOG(FATAL)<< "Usage: "<< argv[0] << " port" ;
        exit(1);
    }
    uint16_t port = atoi(argv[1]);
    // // 设置输出重定向
    // int log_fd = open("./log/log.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
    // if (log_fd < 0) exit(1);
    // dup2(log_fd, 1);
    // dup2(log_fd, 2);
    // close(log_fd);  // 关闭多余的文件描述符
    // setvbuf(stdout, nullptr, _IONBF, 0);
    // setvbuf(stderr, nullptr, _IONBF, 0);
    
    // Use_File_Log_Strategy;
    // DaemonModule::Daemon(1, 0);

    HttpServer server(port);
    // 要在一台4核机子上开三个编译运行服务 和 一个OJ服务
    // 都用单Reactor
    server.SetCount(0);
    server.Post("/compile_run", [](HttpServer &server, HttpRequest &reqs, HttpResponse &resp) { 
        std::string in_json = reqs._body;
        std::string out_json;
        if(in_json.size())
        {
            CompileAndRun::Run(in_json, &out_json);
            resp.SetContent(out_json, "application/json;charset=utf-8");
        }
    });

    server.Start();
    return 0;
}



// int main()
// {
//     Json::Value in_json;
//     std::string code; 
//     FileUtil::ReadFile(PathUtil::Src("hello"), &code);
//     in_json["code"] = code;
//     in_json["input"] = "";
//     in_json["cpu_limit"] = 1;
//     in_json["mem_limit"] = 30 * 1024;
//     Json::FastWriter writer;
//     std::string in_str = writer.write(in_json);
//     std::string out_str;
//     CompileAndRun::Run(in_str, &out_str);
//     Json::Value out_json;
//     Json::Reader reader;
//     reader.parse(out_str, out_json);
//     std::cout<< out_str<< std::endl;
//     std::cout<< out_json["status"].asInt() << std::endl;
//     std::cout<< out_json["reason"].asString() << std::endl;
//     std::cout<< out_json["stdout"].asString() << std::endl;
//     std::cout<< out_json["stderr"].asString() << std::endl;
// }

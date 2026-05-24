#include "./oj_control.hpp"
// #include "../source/http/Http.hpp"
#include "../source/http/Daemon.hpp"

Control *col = nullptr;
void handler(int sig)
{
    col->OnlineMachine();
}
int main()
{
    Control control;
    col = &control;
    signal(SIGUSR1, handler);

    // 设置输出重定向
    int log_fd = open("./log/log.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (log_fd < 0) exit(1);
    dup2(log_fd, 1);
    dup2(log_fd, 2);
    close(log_fd);  // 关闭多余的文件描述符
    setvbuf(stdout, nullptr, _IONBF, 0);
    setvbuf(stderr, nullptr, _IONBF, 0);
    
    Use_File_Log_Strategy;
    DaemonModule::Daemon(1, 0);

    HttpServer server(8888);
    server.SetCount(3);
    server.SetBaseDir("./wwwroot");
    server.Get("/questions", [&](HttpServer &server, HttpRequest &reqs, HttpResponse &resp){
        std::string html;
        control.AllQuestions(&html);
        resp.SetContent(html, "text/html");
    });
    server.Get("/question/(\\d+)", [&](HttpServer &server, HttpRequest &reqs, HttpResponse &resp){
        std::string question_id = reqs._matches[1];
        std::string html;
        control.Question(question_id, &html);
        resp.SetContent(html, "text/html");
    });
    server.Post("/judge/(\\d+)", [&](HttpServer &server, HttpRequest &reqs, HttpResponse &resp){
        std::string in_json = reqs._body;
        std::string out_json;
        control.Judge(reqs._matches[1], in_json, &out_json);
        resp.SetContent(out_json, "application/json");
    });
    server.Start();
    return 0;
}
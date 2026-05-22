#include "./oj_control.hpp"
// #include "../source/http/Http.hpp"

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
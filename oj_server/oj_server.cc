#include "./oj_control.hpp"
// #include "../source/http/Http.hpp"

int main()
{
    Control control;
    HttpServer server(8888);
    server.SetCount(0);
    server.Get("/questions", [&](HttpServer &server, HttpRequest &reqs, HttpResponse &resp){
        std::string html;
        control.AllQuestions(&html);
        resp.SetContent(html, "text/html");
    });
    server.Get("/question/(\\d+)", [&](HttpServer &server, HttpRequest &reqs, HttpResponse &resp){
        std::string question_id = reqs._matches[1];
        // LOG(INFO) << "question_id:" << question_id;
        std::string html;
        control.Question(question_id, &html);
        resp.SetContent(html, "text/html");
    });
    server.Post("/judge/(\\d+)", [&](HttpServer &server, HttpRequest &reqs, HttpResponse &resp){
        // LOG(INFO) << "judge";
        std::string in_json = reqs._body;
        std::string out_json;
        control.Judge(reqs._matches[1], in_json, &out_json);
        resp.SetContent(out_json, "application/json");
    });
    server.Start();
    return 0;
}
#include "./oj_control.hpp"
// #include "../source/http/Http.hpp"

int main()
{
    Control control;
    HttpServer server(8888);
    server.SetCount(0);
    server.Post("/judge", [&](HttpServer &server, HttpRequest &reqs, HttpResponse &resp){
        // LOG(INFO) << "judge";
        std::string in_json = reqs._body;
        std::string out_json;
        control.Judge("1", in_json, &out_json);
        resp.SetContent(out_json, "application/json");
    });
    server.Start();
    return 0;
}
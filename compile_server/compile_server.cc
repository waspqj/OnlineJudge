#include "compile_run.hpp"
int main()
{
    Json::Value in_json;
    std::string code; 
    FileUtil::ReadFile(PathUtil::Src("hello"), &code);
    in_json["code"] = code;
    in_json["input"] = "";
    in_json["cpu_limit"] = 1;
    in_json["mem_limit"] = 30 * 1024;
    Json::FastWriter writer;
    std::string in_str = writer.write(in_json);
    std::string out_str;
    CompileAndRun::Run(in_str, &out_str);
    Json::Value out_json;
    Json::Reader reader;
    reader.parse(out_str, out_json);
    std::cout<< out_str<< std::endl;
    std::cout<< out_json["status"].asInt() << std::endl;
    std::cout<< out_json["reason"].asString() << std::endl;
    std::cout<< out_json["stdout"].asString() << std::endl;
    std::cout<< out_json["stderr"].asString() << std::endl;
}

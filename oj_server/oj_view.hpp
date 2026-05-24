#pragma once
// #include "oj_model.hpp"
#include "oj_model2.hpp"
#include <ctemplate/template.h>

class View
{
    const std::string template_path = "./template_html/";
public:
    View(){}
    ~View(){}
    void AllQuestionHtml(const std::vector<Question> &questions, std::string *html)
    {
        // 题目的编号 题目的标题 题目的难度
        // 推荐使用表格显示
        // 1. 形成路径
        std::string src_html = template_path + "all_questions.html";
        // 2. 形成数字典
        ctemplate::TemplateDictionary root("all_questions");
        for (const auto& q : questions)
        {
            ctemplate::TemplateDictionary *sub = root.AddSectionDictionary("question_list");
            sub->SetValue("number", q.id);
            sub->SetValue("title", q.title);
            sub->SetValue("star", q.dificulty);
        }
        //3. 获取被渲染的html
        ctemplate::Template *tpl = ctemplate::Template::GetTemplate(src_html, ctemplate::DO_NOT_STRIP);
        //4. 开始完成渲染功能
        tpl->Expand(html, &root);
    }
    void OneQuestionHtml(const Question &q, std::string *html)
    {
        // 1. 形成路径
        std::string src_html = template_path + "one_question.html";
        // 2. 形成数字典
        ctemplate::TemplateDictionary root("one_question");
        root.SetValue("number", q.id);
        root.SetValue("title", q.title);
        root.SetValue("star", q.dificulty);
        root.SetValue("desc", q.desc);
        root.SetValue("pre_code", q.header);
        //3. 获取被渲染的html
        ctemplate::Template *tpl = ctemplate::Template::GetTemplate(src_html, ctemplate::DO_NOT_STRIP);
        //4. 开始完成渲染功能
        tpl->Expand(html, &root);
    }
    void Error(const std::string error, std::string *html)
    {
        std::string src_html = template_path + "error.html";
        ctemplate::TemplateDictionary root("error");
        root.SetValue("error", error);
        ctemplate::Template *tpl = ctemplate::Template::GetTemplate(src_html, ctemplate::DO_NOT_STRIP);
        tpl->Expand(html, &root);
    }
};
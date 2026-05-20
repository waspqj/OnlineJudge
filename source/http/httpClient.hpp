#include "./Http.hpp"
#include <string>
#include <sstream>

// 简易 HTTP POST 客户端
class HttpClient 
{
private:
    Socket _sock;
    const std::string _host;
    const uint16_t _port;
    HttpResponse resp;
public:
    HttpClient(const std::string& host, const std::string port)
        :_host(host), _port(atoi(port.c_str()))
    {
        if (!_sock.BuildTcpClient(host, atoi(port.c_str())))
        {
            LOG(ERROR) << "HttpClient: BuildTcpClient failed";
        }
    }
    HttpResponse* HttpPost(const std::string &path, const std::string &body)
    {
        // 构造 HTTP POST 请求
        std::ostringstream req;
        req << "POST " << path << " HTTP/1.1\r\n";
        req << "Host: " << _host << ":" << _port << "\r\n";
        req << "Content-Type: application/json;charset=utf-8\r\n";
        req << "Content-Length: " << body.size() << "\r\n";
        req << "Connection: close\r\n";
        req << "\r\n";
        req << body;

        std::string request = req.str();
        ssize_t n = _sock.Send(request.data(), request.size(), 0);
        if (n < 0)
        {
            LOG(ERROR) << "HttpPost: send failed";
            return nullptr;
        }

        // 接收响应（简单实现：读到对端关闭为止）
        std::string http_recv;
        char buf[4096];
        while ((n = _sock.Recv(buf, sizeof(buf), 0)) > 0)
        {
            http_recv.append(buf, n);
        }
        ParseHttpResp(http_recv);
        return &resp;
    }

    // 解析客户端收到的 HTTP 响应，填充到 HttpResponse
    void ParseHttpResp(const std::string &http_recv) {
        resp.Reset();  // 清空原有数据

        // 1. 定位状态行结束
        size_t pos = http_recv.find("\r\n");
        if (pos == std::string::npos) {
            resp._statu = -1;
            return;
        }
        std::string statusLine = http_recv.substr(0, pos);

        // 2. 解析状态码 (例如 "HTTP/1.1 200 OK")
        size_t space1 = statusLine.find(' ');
        if (space1 == std::string::npos) {
            resp._statu = -1;
            return;
        }
        size_t space2 = statusLine.find(' ', space1 + 1);
        std::string codeStr = statusLine.substr(space1 + 1, space2 == std::string::npos ? std::string::npos : space2 - space1 - 1);
        try {
            resp._statu = std::stoi(codeStr);
        } catch (...) {
            resp._statu = -1;
            return;
        }

        // 3. 头部起始和正文起始
        size_t headStart = pos + 2;
        size_t bodyStart = http_recv.find("\r\n\r\n", headStart);
        if (bodyStart != std::string::npos) {
            bodyStart += 4;  // 跳过 "\r\n\r\n"
        } else {
            bodyStart = http_recv.size();  // 没有正文（或格式异常）
        }

        // 4. 解析头部字段
        resp._heads.clear();
        std::string location;
        size_t cur = headStart;
        while (cur + 2 < bodyStart) {
            size_t lineEnd = http_recv.find("\r\n", cur);
            if (lineEnd == std::string::npos || lineEnd >= bodyStart) break;
            std::string line = http_recv.substr(cur, lineEnd - cur);
            size_t colon = line.find(':');
            if (colon != std::string::npos) {
                std::string key = line.substr(0, colon);
                std::string value = line.substr(colon + 1);
                // 去除 value 前后的空白字符
                size_t valueStart = value.find_first_not_of(" \t");
                size_t valueEnd = value.find_last_not_of(" \t");
                if (valueStart != std::string::npos)
                    value = value.substr(valueStart, valueEnd - valueStart + 1);
                else
                    value.clear();
                resp._heads.emplace(key, value);
                if (key == "Location") location = value;
            }
            cur = lineEnd + 2;
        }

        // 5. 判断是否重定向 (3xx 状态码 + Location 头部)
        if (resp._statu >= 300 && resp._statu < 400 && !location.empty()) {
            resp._is_redirect = true;
            resp._redirect_url = location;
        } else {
            resp._is_redirect = false;
            resp._redirect_url.clear();
        }

        // 6. 提取正文
        resp._body = http_recv.substr(bodyStart);
    }
};

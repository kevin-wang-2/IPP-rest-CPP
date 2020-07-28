#include "HTTPProxyHandler.h"
#include "../router/Router.h"
#include <iostream>
#include <sstream>
#include <ctime>

using namespace std;

HTTPProxyHandler::HTTPProxyHandler() = default;

bool HTTPProxyHandler::validate(const string &input) {
    return true;
}

std::string HTTPProxyHandler::parse(std::string input, IP ip) {
    // DEBUG 用, 显示完整请求
    cout << input << endl;

    HTTPRequest_t req;

    // 1. 解析HTTP请求头
    req.header = HTTPParser::parseHeader(input);
    req.header.client = ip;

    // 2. 解析请求体
    unsigned long pos;
    if((pos = input.find("\r\n\r\n")) != string::npos) { // 存在requestBody
        req.body = input.substr(pos + 4, input.length());
    }

    /// 请求解析完成,生成响应 ///

    HTTPResponse_t res;

    // 3. Router
    Router::getRouter()(req, res);

    // TODO: 用C++时间重写时间格式化
    char szDT[128];
    struct tm *newtime;
    long ltime;
    time(&ltime);
    newtime = gmtime(&ltime);
    strftime(szDT, 128,"%a, %d %b %Y %H:%M:%S GMT", newtime);

    ostringstream oss;
    oss << "HTTP/1.1 200 OK\r\nServer: 123\r\nDate:"
        << szDT
        << "\r\nAccept-Ranges: bytes\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: "
        << res.body.length()
        << "\r\nConnection: keep-alive\r\n\r\n"
        << res.body;

    return oss.str();
}
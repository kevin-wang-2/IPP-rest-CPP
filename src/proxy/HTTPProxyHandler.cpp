#include "HTTPProxyHandler.h"
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

    // 1. 解析HTTP请求
    auto header = HTTPParser::parseHeader(input);
    cout << header.path << endl;

    // TODO: 用C++时间重写时间格式化
    char szDT[128];
    struct tm *newtime;
    long ltime;
    time(&ltime);
    newtime = gmtime(&ltime);
    strftime(szDT, 128,"%a, %d %b %Y %H:%M:%S GMT", newtime);

    return string("HTTP/1.1 200 OK\r\nServer: 123\r\nDate:") + szDT +
           string("\r\nAccept-Ranges: bytes\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: 3\r\nConnection: keep-alive\r\n\r\n123");
}
#ifndef CPPREST_HTTPPROXYHANDLER_H
#define CPPREST_HTTPPROXYHANDLER_H

#include "proxyMessageLoop.h"
#include "HTTPParser.h"

struct HTTPRequest_t {
    HTTPRequestHeader_t header;
    std::string body;
};

struct HTTPResponseHeader_t {
    int status = 200;
    MIME_t contentType;
};

struct HTTPResponse_t {
    HTTPResponseHeader_t header;
    std::string body;
};

const std::map<int, const char*> HTTPReturnCode = {
        {100, "Continue"},
        {101, "Switching Protocols"},
        {102, "Processing"},
        {200, "OK"},
        {201, "Created"},
        {202, "Accepted"},
        {203, "Non-Authoritative Information"},
        {204, "No Content"},
        {205, "Reset Content"},
        {206, "Partial Content"},
        {207, "Multi-Status"},
        {300, "Multiple Choices"},
        {301, "Moved Permanently"},
        {302, "Move Temporarily"},
        {303, "See Other"},
        {304, "Not Modified"},
        {305, "Use Proxy"},
        {306, "Switch Proxy"},
        {307, "Temporary Redirect"},
        {400, "Bad Request"},
        {401, "Unauthorized"},
        {402, "Payment Required"},
        {403, "Forbidden"},
        {404, "Not found"},
        {405, "Method Not Allowed"},
        {406, "Not Acceptable"},
        {407, "Proxy Authentication Required"},
        {408, "Request Timeout"},
        {409, "Conflict"},
        {410, "Gone"},
        {411, "Length Required"},
        {412, "Precondition Failed"},
        {413, "Request Entity Too Large"},
        {414, "Request-URI Too Long"},
        {415, "Unsupported Media Type"},
        {416, "Requested Range Not Satisfiable"},
        {417, "Expectation Failed"},
        {418, "IPP Project not Finishable"},
        {421, "Misdirected Request"},
        {422, "Unprocessable Entity"},
        {423, "Locked"},
        {424, "Failed Dependency"},
        {425, "Too Early"},
        {426, "Upgrade Required"},
        {449, "Retry With"},
        {451, "Unavailable For Legal Reasons"},
        {500, "Internal Server Error"},
        {501, "Not Implemented"},
        {502, "Bad Gateway"},
        {503, "Service Unavailable"},
        {504, "Gateway Timeout"},
        {505, "HTTP Version Not Supported"},
        {506, "Variant Also Negotiates"},
        {507, "Insufficient Storage"},
        {509, "Bandwidth Limit Exceeded"},
        {510, "Not Extended"},
        {600, "Unparseable Response Headers"}
};

///// HTTP处理Handler /////

class HTTPProxyHandler : public ProxyTemplate {
public:
    HTTPProxyHandler();

    bool validate(const std::string &input) override;

    std::string parse(std::string input, IP ip) override;
};

using HTTPServer = SocketServer<proxyMsgLoop<HTTPProxyHandler>>;

#endif //CPPREST_HTTPPROXYHANDLER_H

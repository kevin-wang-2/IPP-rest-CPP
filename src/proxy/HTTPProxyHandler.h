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

///// HTTP处理Handler /////

class HTTPProxyHandler : public ProxyTemplate {
public:
    HTTPProxyHandler();

    bool validate(const std::string &input) override;

    std::string parse(std::string input, IP ip) override;
};


#endif //CPPREST_HTTPPROXYHANDLER_H

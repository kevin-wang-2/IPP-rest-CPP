#ifndef CPPREST_HTTPPROXYHANDLER_H
#define CPPREST_HTTPPROXYHANDLER_H

#include "proxyMessageLoop.h"
#include "HTTPParser.h"

///// HTTP处理Handler /////

class HTTPProxyHandler : public ProxyTemplate {
public:
    HTTPProxyHandler();

    bool validate(const std::string &input) override;

    std::string parse(std::string input, IP ip) override;
};


#endif //CPPREST_HTTPPROXYHANDLER_H

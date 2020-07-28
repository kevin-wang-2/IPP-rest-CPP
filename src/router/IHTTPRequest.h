#ifndef CPP_REST_IHTTPREQUEST_H
#define CPP_REST_IHTTPREQUEST_H

#include "../proxy/HTTPProxyHandler.h"

class Router;

class IHTTPRequest {
    explicit IHTTPRequest(const HTTPRequest_t &req);

public:
    const HTTPRequest_t &request;

    std::map<std::string, std::string> parsedBody;

    std::map<std::string, std::string> queryString;

    std::vector<std::string> params;

    friend Router;
};

#endif //CPP_REST_IHTTPREQUEST_H

#ifndef CPP_REST_ROUTERCALLABLE_H
#define CPP_REST_ROUTERCALLABLE_H

#include <memory>
#include <map>
#include <functional>
#include <utility>
#include "../proxy/HTTPProxyHandler.h"

///// 对于请求和响应作出封装 /////
class RequestEnd : public std::exception {
};

class Router;

class IHTTPResponse {
    HTTPResponse_t &response;
    bool finished = false;

    void validate() { if (finished) throw RequestEnd(); }

    explicit IHTTPResponse(HTTPResponse_t &res) : response(res), const_response(res) {}

public:
    const HTTPResponse_t &const_response;

    void status(int status) { response.header.status = status; }

    void write(const std::string &s) {
        validate();
        response.body += s;
    }

    void end(const std::string &s) {
        validate();
        response.body += s;
        finished = true;
    }

    friend Router;
};

class IHTTPRequest {
    explicit IHTTPRequest(const HTTPRequest_t &req) : request(req) {}

public:
    const HTTPRequest_t &request;

    std::map<std::string, std::string> parsedBody;

    friend Router;
};

///// 纯虚路由基类 /////

class RouterCallable {
    bool isFinished = true;
public:
    void operator()(IHTTPRequest &req, IHTTPResponse &res);

    bool finish() { return isFinished; };

    virtual void run(IHTTPRequest &req, IHTTPResponse &res, std::function<void()>) = 0;
};

///// 简单函数路由 /////

class FunctionalRouterCallable : public RouterCallable {
    std::function<bool(IHTTPRequest &, IHTTPResponse &)> fun;
public:
    explicit FunctionalRouterCallable(std::function<bool(IHTTPRequest &, IHTTPResponse &)> _fun) : fun(std::move(_fun)) {}

    void run(IHTTPRequest &req, IHTTPResponse &res, std::function<void()> next) final { if(!fun(req, res)) next(); };
};


#endif //CPP_REST_ROUTERCALLABLE_H

#ifndef CPP_REST_ROUTERCALLABLE_H
#define CPP_REST_ROUTERCALLABLE_H

#include <memory>
#include <map>
#include <functional>
#include <utility>
#include "../proxy/HTTPProxyHandler.h"
#include "IHTTPRequest.h"
#include "IHTTPResponse.h"

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
protected:
    std::function<bool(IHTTPRequest &, IHTTPResponse &)> fun;
public:
    explicit FunctionalRouterCallable(std::function<bool(IHTTPRequest &, IHTTPResponse &)> _fun) : fun(std::move(_fun)) {}

    void run(IHTTPRequest &req, IHTTPResponse &res, std::function<void()> next) override { if(!fun(req, res)) next(); };
};


#endif //CPP_REST_ROUTERCALLABLE_H

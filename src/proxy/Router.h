#ifndef CPP_REST_ROUTER_H
#define CPP_REST_ROUTER_H

#include <deque>
#include <memory>
#include "HTTPProxyHandler.h"
#include "../router/RouterCallable.h"

class Router {
    std::deque<std::shared_ptr<RouterCallable>> dqCallable {};
    Router() = default;
public:
    static Router& getRouter();

    Router& use(const std::shared_ptr<RouterCallable>& fn) {
        dqCallable.push_back(fn);
        return *this;
    }

    void operator()(const HTTPRequest_t& req, HTTPResponse_t& res);
};


#endif //CPP_REST_ROUTER_H

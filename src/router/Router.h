#ifndef CPP_REST_ROUTER_H
#define CPP_REST_ROUTER_H

#include <deque>
#include <memory>
#include "../proxy/HTTPProxyHandler.h"
#include "RouterCallable.h"

class Router {
    std::deque<std::shared_ptr<RouterCallable>> dqCallable {};
    Router() = default;
public:
    static Router& getRouter();


    Router& use(const std::shared_ptr<RouterCallable>& fn) {
        dqCallable.push_back(fn);
        return *this;
    }

    template <typename T, typename std::enable_if<std::is_base_of<RouterCallable, T>{}, int>::type = 0>
    Router& use(T&& fn) {
        dqCallable.push_back(std::make_shared<T>(fn));
        return *this;
    }

    void operator()(const HTTPRequest_t& req, HTTPResponse_t& res);
};


#endif //CPP_REST_ROUTER_H

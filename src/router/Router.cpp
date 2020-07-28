#include "Router.h"
#include <iostream>

using namespace std;

Router& Router::getRouter() {
    static unique_ptr<Router> router = nullptr;
    if(!router) router = make_unique<Router>(Router());

    return *router;
}

void Router::operator()(const HTTPRequest_t &req, HTTPResponse_t &res) {
    IHTTPRequest ireq(req);
    IHTTPResponse ires(res);

    for(auto &callable : dqCallable) {
        try {
            callable->operator()(ireq, ires);
            if (callable->finish()) return;
        } catch(...) {
            res.header.status = 500;
        }
    }

    if(!ires.finished) {
        res.header.status = 404;
    }
}
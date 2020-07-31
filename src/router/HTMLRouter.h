#ifndef CPP_REST_HTMLROUTER_H
#define CPP_REST_HTMLROUTER_H

#include <utility>
#include "RouterCallable.h"

template<HTTPMethod_t method>
class HTMLRouter : public FunctionalRouterCallable {
protected:
    std::string _path;
public:
    HTMLRouter(std::string path, const std::function<bool(IHTTPRequest &, IHTTPResponse &)>& fun) : FunctionalRouterCallable(fun) {
        if(*(path.end() - 1) != '/') _path = path;
        else _path = path.substr(0, path.length() - 1);
    }

    void run(IHTTPRequest &req, IHTTPResponse &res, std::function<void()> next) override;

    const char* getIdentity() override { return "HTML"; }
};

#include "HTMLRouter.hpp"

#endif //CPP_REST_HTMLROUTER_H

#ifndef CPP_REST_HTMLROUTER_H
#define CPP_REST_HTMLROUTER_H

#include <utility>
#include "RouterCallable.h"

template<HTTPMethod_t method>
class HTMLRouter : public FunctionalRouterCallable {
protected:
    std::string _path;
public:
    HTMLRouter(std::string path, const std::function<bool(IHTTPRequest &, IHTTPResponse &)>& fun) : FunctionalRouterCallable(fun), _path(std::move(path)) {}

    void run(IHTTPRequest &req, IHTTPResponse &res, std::function<void()> next) override;
};

///// 辅助函数，用于减少一大堆关于模板的代码 /////

template<HTTPMethod_t method>
inline std::shared_ptr<HTMLRouter<method>> createHTMLRouter(std::string path, const std::function<bool(IHTTPRequest &, IHTTPResponse &)>& fun) {
    return std::make_shared<HTMLRouter<method>>(HTMLRouter<method>(path, fun));
}

#include "HTMLRouter.hpp"

#endif //CPP_REST_HTMLROUTER_H

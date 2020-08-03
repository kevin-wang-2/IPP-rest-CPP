#ifndef CPP_REST_JSONAPI_H
#define CPP_REST_JSONAPI_H

#include <utility>

#include "../router/RouterCallable.h"
#include "../utils/URL.h"
#include "../template/JSONTemplateRoot.h"

template <HTTPMethod_t method>
class JsonAPI : public RouterCallable {
    std::string path;
    JSONTemplateRoot templateRoot;
    std::function<std::map<std::string, HybridJSONType>(IHTTPRequest &, IHTTPResponse &)> fun;
public:
    JsonAPI(std::string  _path, const char* templ, std::function<std::map<std::string, HybridJSONType>(IHTTPRequest &, IHTTPResponse &)> _fun)
    : path(std::move(_path)), templateRoot(templ), fun(std::move(_fun)) {}

    void run(IHTTPRequest &req, IHTTPResponse &res, std::function<void()> next) override;

    const char* getIdentity() override { return "JSON_RESTfulAPI"; }
};

#include "JsonAPI.hpp"


#endif //CPP_REST_JSONAPI_H

#ifndef CPP_REST_STATICRESOURCE_H
#define CPP_REST_STATICRESOURCE_H

#include <utility>

#include "../router/RouterCallable.h"
#include "../utils/URL.h"

class StaticResource : public RouterCallable {
    std::string source, dest;
public:
    StaticResource(const std::string&  src, const std::string& dst) : source(URLSimplify(src)), dest(URLSimplify(dst)) {

    }

    void run(IHTTPRequest &req, IHTTPResponse &res, std::function<void()> next) override;

    const char* getIdentity() override { return "Static"; }
};


#endif //CPP_REST_STATICRESOURCE_H

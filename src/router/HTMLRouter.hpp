#include <regex>
#include "HTMLRouter.h"

using namespace std;

template<HTTPMethod_t method>
void HTMLRouter<method>::run(IHTTPRequest &req, IHTTPResponse &res, std::function<void()> next) {
    if(((req.request.header.path != _path) && (req.request.header.path.find(_path + "/") != 0)) || (req.request.header.method != method)) next();
    else {
        // 先处理URL参数
        if(req.request.header.path != _path) {
            string subPath = req.request.header.path.substr(_path.length() + 1, req.request.header.path.find('?') - _path.length() - 1);

            regex ws_re("/");
            req.params = vector<string>(sregex_token_iterator(subPath.begin(), subPath.end(), ws_re, -1), sregex_token_iterator());
        }

        FunctionalRouterCallable::run(req, res, next);
    }
}
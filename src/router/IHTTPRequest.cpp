#include <regex>
#include "IHTTPRequest.h"

#include "../utils/URL.h"

using namespace std;

IHTTPRequest::IHTTPRequest(const HTTPRequest_t &req) : request(req) {
    unsigned long pos;

    // 1. 解析Querystring
    if((pos = req.header.path.find('?')) != string::npos) {
        string query = req.header.path.substr(pos + 1, req.header.path.length());

        regex ws_re("&");
        vector<string> queryGroup(sregex_token_iterator(query.begin(), query.end(), ws_re, -1), sregex_token_iterator());

        for(auto &item : queryGroup) {
            unsigned long eq;
            if((eq = item.find('=')) != string::npos) {
                queryString[item.substr(0, eq)] = item.substr(eq + 1, item.length());
            } else {
                queryString[item] = "true";
            }
        }
    }

    // 2. 解析Body, 方法与queryString相同
    if(!req.body.empty()) {
        regex ws_re("&");
        vector<string> queryGroup(sregex_token_iterator(req.body.begin(), req.body.end(), ws_re, -1), sregex_token_iterator());

        for(auto &item : queryGroup) {
            unsigned long eq;
            if((eq = item.find('=')) != string::npos) {
                parsedBody[item.substr(0, eq)] = UrlDecode(item.substr(eq + 1, item.length()));
            } else {
                parsedBody[item] = "true";
            }
        }
    }

    ///// 这里不解析URL参数，交给中间件处理 /////
}
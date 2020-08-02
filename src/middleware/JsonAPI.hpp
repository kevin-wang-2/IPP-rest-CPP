#include "JsonAPI.h"
#include <regex>

template <HTTPMethod_t method>
void JsonAPI<method>::run(IHTTPRequest &req, IHTTPResponse &res, std::function<void()> next) {
    // 1. 判断是不是属于该路径
    if(((req.request.header.path != path) && (req.request.header.path.find(path + "/") != 0)) || (req.request.header.method != method)) {
        next();
        return;
    }

    // 2. 处理URL参数
    if(req.request.header.path != path) {
        std::string subPath = req.request.header.path.substr(path.length() + 1, req.request.header.path.find('?') - path.length() - 1);

        std::regex ws_re("/");
        req.params = std::vector<std::string>(std::sregex_token_iterator(subPath.begin(), subPath.end(), ws_re, -1), std::sregex_token_iterator());
    }

    // 3. 处理函数返回并生成请求
    auto result = fun(req, res);
    for(auto &item : result) {
        if(item.second.type == HybridJSONType::JSON_VAL) templateRoot.setVal(item.first, item.second.val);
        else templateRoot.setArr(item.first, item.second.arr);
    }
    res.end(templateRoot.concatenate());

}

#ifndef CPP_REST_JSONTEMPLATERENDERER_H
#define CPP_REST_JSONTEMPLATERENDERER_H

#include <string>
#include "JSONTemplateNode.h"

class JSONTemplateRenderer {
    std::string templ;
public:
    JSONTemplateRenderer() = default;
    explicit JSONTemplateRenderer(const std::string& fn) { open(fn); }; // 从文件初始化

    void open(const std::string& fn); // 从文件读取模板

    ///// 设置值 /////
    void setVal(const std::string& path, int val);
    void setVal(const std::string& path, double val);
    void setVal(const std::string& path, bool val);
    void setVal(const std::string& path, const NULL_t& val);
    void setVal(const std::string& path, const char* val);
    void setVal(const std::string& path, const std::string& val);

    ///// 输出 /////
    std::string digest();
};


#endif //CPP_REST_JSONTEMPLATERENDERER_H

#ifndef CPP_REST_JSONTEMPLATENODE_H
#define CPP_REST_JSONTEMPLATENODE_H

#include <string>
#include <utility>

class NULL_t {};

/**
 * JSON模板节点虚基类
 */
class JSONTemplateNode {
public:
    virtual std::string concatenate() = 0; // 拼接旗下节点

    ///// 设置旗下节点以及自己的参数 /////
    virtual void setVal(const std::string& path, int val) {};
    virtual void setVal(const std::string& path, double val) {};
    virtual void setVal(const std::string& path, bool val) {};
    virtual void setVal(const std::string& path, const NULL_t& val) {};
    virtual void setVal(const std::string& path, const char* val) {};
    virtual void setVal(const std::string& path, const std::string& val) {};
};

class JSONTemplateString : public JSONTemplateNode {
    std::string s;
public:
    JSONTemplateString(std::string _) : s(std::move(_)) {}

    std::string concatenate() override { return s; }
};


#endif //CPP_REST_JSONTEMPLATENODE_H

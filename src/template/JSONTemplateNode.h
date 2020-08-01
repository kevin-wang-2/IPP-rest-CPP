#ifndef CPP_REST_JSONTEMPLATENODE_H
#define CPP_REST_JSONTEMPLATENODE_H

#include <string>
#include <utility>
#include <memory>

enum JSONTemplateNodeType {
    ROOT,
    VAR,
    STR,
    LOOP
};

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

    virtual const JSONTemplateNodeType getIdentity() = 0;

    virtual std::shared_ptr<JSONTemplateNode> getCopy() const = 0;
};

/**
 * JSON模板中的纯文本
 */
class JSONTemplateString : public JSONTemplateNode {
    std::string s;
public:
    JSONTemplateString(std::string _) : s(std::move(_)) {}

    std::string concatenate() override { return s; }

    const JSONTemplateNodeType getIdentity() override { return STR; }

    std::shared_ptr<JSONTemplateNode> getCopy() const override {
        return std::make_shared<JSONTemplateString>(JSONTemplateString(s));
    };
};

/**
 * 异常
 */

class InvalidTemplate : public std::exception {
public:
    const char* what() const noexcept override { return "Invalid Template Syntax"; }
};

class InvalidTemplateVariable : public InvalidTemplate {
public:
    const char* what() const noexcept override { return "Invalid Template Variable Syntax"; }
};

class ArgumentUnassigned : public std::exception {
    std::string text;
public:
    ArgumentUnassigned(const std::string& _) : text(std::string("Argument ") + _ + " Unassigned") {}

    const char* what() const noexcept override { return text.c_str(); }

};

#endif //CPP_REST_JSONTEMPLATENODE_H

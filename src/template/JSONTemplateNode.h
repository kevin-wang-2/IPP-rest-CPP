#ifndef CPP_REST_JSONTEMPLATENODE_H
#define CPP_REST_JSONTEMPLATENODE_H

#include <string>
#include <utility>
#include <memory>
#include <vector>

enum JSONTemplateNodeType {
    ROOT,
    VAR,
    STR,
    LOOP
};

class NULL_t {} JSONNULL;

class JSONAny {
    std::string val;
public:
    JSONAny(int _val) : val(std::to_string(_val)) {}
    JSONAny(double _val) : val(std::to_string(_val)) {}
    JSONAny(bool _val) : val(_val? "true" : "false") {}
    JSONAny(const NULL_t& _val) : val("null") {}
    JSONAny(const char* _val) : val(_val) {}
    JSONAny(const std::string& _val) : val(_val) {}

    void set(int _val) { val = std::to_string(_val); }
    void set(double _val) { val = std::to_string(_val); }
    void set(bool _val) { val = (_val? "true" : "false"); }
    void set(const NULL_t& _val) { val = "null"; }
    void set(const char* _val) { val = _val; }
    void set(const std::string& _val) { val = _val; }

    template <typename T>
    void operator=(T _val) { set(_val); }

    const std::string& get() const { return val; };

    ~JSONAny() = default;
};

class HybridJSONType {
public:
    const union {
        JSONAny val;
        const JSONAny* ref;
        std::vector<JSONAny> arr;
    };

    const enum {
        JSON_VAL,
        JSON_ARR,
        JSON_REF
    } type;

    HybridJSONType(const JSONAny* _ref) : ref(_ref), type(JSON_REF) {}
    HybridJSONType(JSONAny _val) : val(std::move(_val)), type(JSON_VAL) {}
    HybridJSONType(std::vector<JSONAny> _arr) : arr(std::move(_arr)), type(JSON_ARR) {}

    ~HybridJSONType() {
        switch(type) {
            case JSON_VAL:
                val.~JSONAny();
                break;
            case JSON_ARR:
                arr.clear();
            default:
                break;
        }
    }
};

/**
 * JSON模板节点虚基类
 */
class JSONTemplateNode {
public:
    virtual std::string concatenate() = 0; // 拼接旗下节点

    ///// 设置旗下节点以及自己的参数 /////
    virtual void setVal(const std::string& path, const JSONAny& val) {};
    virtual void setRef(const std::string& path, const JSONAny& ref) {};
    virtual void setArr(const std::string& path, std::vector<JSONAny> arr) {};

    virtual void set(const std::string& path, const HybridJSONType& var) {
        switch(var.type) {
            case HybridJSONType::JSON_VAL:
                setVal(path, var.val);
                break;
            case HybridJSONType::JSON_ARR:
                setArr(path, var.arr);
                break;
            case HybridJSONType::JSON_REF:
                setRef(path, *var.ref);
                break;
        }
    }

    virtual const JSONTemplateNodeType getIdentity() = 0;

    virtual std::shared_ptr<JSONTemplateNode> getCopy() const = 0;

    virtual void reset() = 0;
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

    void reset() override {};
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

#ifndef CPP_REST_JSONTEMPLATEVAR_H
#define CPP_REST_JSONTEMPLATEVAR_H

#include <utility>

#include "JSONTemplateNode.h"

class JSONTemplateVar : public JSONTemplateNode {
    std::string _path;

    std::string _val;

    bool assigned = false;
public:
    explicit JSONTemplateVar(std::string  _, std::string defaultVal = "", bool ass = false) : _path(std::move(_)), _val(std::move(defaultVal)), assigned(ass) {};

    std::string concatenate() override {
        if(assigned)
            return _val;
        else throw ArgumentUnassigned(_path);
    }

    void setVal(const std::string& path, int val) override {
        if(path == _path) {
            _val = std::to_string(val);
            assigned = true;
        }
    }

    void setVal(const std::string& path, double val) override {
        if(path == _path) {
            _val = std::to_string(val);
            assigned = true;
        }
    }

    void setVal(const std::string& path, bool val) override {
        if(path == _path) {
            _val = val ? "true" : "false";
            assigned = true;
        }
    }

    void setVal(const std::string& path, const NULL_t&) override {
        if(path == _path) {
            _val = "null";
            assigned = true;
        }
    }

    void setVal(const std::string& path, const char* val) override {
        if(path == _path) {
            _val = std::string("\"") + val + "\"";
            assigned = true;
        }
    }

    void setVal(const std::string& path, const std::string& val) override {
        if(path == _path) {
            _val = std::string("\"") + val + "\"";
            assigned = true;
        }
    }

    const JSONTemplateNodeType getIdentity() override { return VAR; }

    std::shared_ptr<JSONTemplateNode> getCopy() const override {
        return std::make_shared<JSONTemplateVar>(JSONTemplateVar(_path, _val, assigned));
    };
};

#endif //CPP_REST_JSONTEMPLATEVAR_H

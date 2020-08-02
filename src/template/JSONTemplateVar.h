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

    void setVal(const std::string& path, const JSONAny& val) override {
        if(path == _path) {
            _val = val.get();
            assigned = true;
        }
    }

    const JSONTemplateNodeType getIdentity() override { return VAR; }

    std::shared_ptr<JSONTemplateNode> getCopy() const override {
        return std::make_shared<JSONTemplateVar>(JSONTemplateVar(_path, _val, assigned));
    };

    void reset() override {
        assigned = false;
    };
};

#endif //CPP_REST_JSONTEMPLATEVAR_H

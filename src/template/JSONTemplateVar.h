#ifndef CPP_REST_JSONTEMPLATEVAR_H
#define CPP_REST_JSONTEMPLATEVAR_H

#include <utility>

#include "JSONTemplateNode.h"

class JSONTemplateVar : public JSONTemplateNode {
    std::string _path;

    std::string _val;

    const JSONAny* _ref;

    enum _var_type_t {
        val, ref
    } var_type;

    bool assigned = false;
public:
    explicit JSONTemplateVar(std::string  _, std::string defaultVal = "", const JSONAny* defaultRef = nullptr, _var_type_t t = val, bool ass = false) :
    _path(std::move(_)), _val(std::move(defaultVal)), _ref(defaultRef), var_type(t), assigned(ass) {};

    std::string concatenate() override {
        if(assigned)
            if(var_type == val)
                return _val;
            else
                return _ref->get();
        else throw ArgumentUnassigned(_path);
    }

    void setVal(const std::string& path, const JSONAny& val) override {
        if(path == _path) {
            _val = val.get();
            assigned = true;
            var_type = this->val;
        }
    }

    void setRef(const std::string& path, const JSONAny& ref) override {
        if(path == _path) {
            _ref = &ref;
            assigned = true;
            var_type = this->ref;
        }
    }

    const JSONTemplateNodeType getIdentity() override { return VAR; }

    std::shared_ptr<JSONTemplateNode> getCopy() const override {
        return std::make_shared<JSONTemplateVar>(JSONTemplateVar(_path, _val, _ref, var_type, assigned));
    };

    void reset() override {
        assigned = false;
    };
};

#endif //CPP_REST_JSONTEMPLATEVAR_H

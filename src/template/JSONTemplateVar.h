#ifndef CPP_REST_JSONTEMPLATEVAR_H
#define CPP_REST_JSONTEMPLATEVAR_H

#include <utility>

#include "JSONTemplateNode.h"

class JSONTemplateVar : public JSONTemplateNode {
    std::string path;

    std::string val;
public:
    explicit JSONTemplateVar(std::string  _) : path(std::move(_)) {};

    std::string concatenate() override {
        return val;
    }
};

#endif //CPP_REST_JSONTEMPLATEVAR_H

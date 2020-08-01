#ifndef CPP_REST_JSONTEMPLATELOOP_H
#define CPP_REST_JSONTEMPLATELOOP_H

#include "JSONTemplateNode.h"
#include <utility>
#include <vector>
#include <memory>

class JSONTemplateLoop : public JSONTemplateNode {
    std::vector<std::shared_ptr<JSONTemplateNode>> templ;
    std::vector<std::vector<std::shared_ptr<JSONTemplateNode>>> child;
public:
    explicit JSONTemplateLoop(std::vector<std::shared_ptr<JSONTemplateNode>> _) : templ(std::move(_)) {};

    std::string concatenate() override {
        std::string ret;
        for(auto &subtree : child) {
            for(auto &item : subtree) {
                ret += item->concatenate();
            }
        }
        return ret;
    }
};
#endif //CPP_REST_JSONTEMPLATELOOP_H

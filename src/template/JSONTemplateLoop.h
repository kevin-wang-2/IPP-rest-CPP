#ifndef CPP_REST_JSONTEMPLATELOOP_H
#define CPP_REST_JSONTEMPLATELOOP_H

#include "JSONTemplateNode.h"
#include <utility>
#include <vector>
#include <memory>

class JSONTemplateLoop : public JSONTemplateNode {
    std::vector<std::shared_ptr<JSONTemplateNode>> child;
    int loopCnt = 0;
public:
    explicit JSONTemplateLoop(std::vector<std::shared_ptr<JSONTemplateNode>> _) : child(std::move(_)) {};

    std::string concatenate() override {
        std::string ret;
        for(int i = 0; i < loopCnt; i++) {
            for(auto &item : child) {
                ret += item->concatenate();
            }
        }
        return ret;
    }
};
#endif //CPP_REST_JSONTEMPLATELOOP_H

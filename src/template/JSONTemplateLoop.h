#ifndef CPP_REST_JSONTEMPLATELOOP_H
#define CPP_REST_JSONTEMPLATELOOP_H

#include "JSONTemplateNode.h"
#include "JSONTemplateVar.h"
#include <utility>
#include <vector>
#include <memory>

static bool isInt(std::string s) {
    for(auto &i : s) if((i < '0') || (i > '9')) return false;
    return true;
}

template <typename T>
static inline T min(T x, T y) { return (x < y) ? x : y; }

class JSONTemplateLoop : public JSONTemplateNode {
    std::vector<std::shared_ptr<JSONTemplateNode>> templ;
    std::vector<std::vector<std::shared_ptr<JSONTemplateNode>>> child;

    explicit JSONTemplateLoop(std::vector<std::shared_ptr<JSONTemplateNode>> _t, std::vector<std::vector<std::shared_ptr<JSONTemplateNode>>> _c) : templ(_t), child(_c) {};
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

    void setVal(const std::string& path, const JSONAny& val) override {
        if(path.find('.') != std::string::npos) { // 存在.
            std::string pre = path, post, pg, temp;
            unsigned int index;
            while(true) {
                auto pos = pre.find_last_of('.');
                post = pre.substr(pos + 1, std::string::npos);
                pre = pre.substr(0, pos);

                long subpos = pre.find_last_of('.');
                if((unsigned long)subpos == std::string::npos) subpos = -1;
                temp = pre.substr(subpos + 1, std::string::npos);

                if(!isInt(temp)) {
                    index = std::stoi(post);
                    pg = pre.substr(0, subpos) + pg;
                    break;
                } else {
                    pg = "." + post + pg;
                }
            }

            while(index >= child.size()) {
                std::vector<std::shared_ptr<JSONTemplateNode>> app;
                for(auto &item : templ) {
                    app.push_back(item->getCopy());
                }
                child.push_back(app);
            }

            for(auto &item : child[index]) item->setVal(pg, val);
        }
    }

    void setArr(const std::string& path, std::vector<JSONAny> arr) override {
        // 1. 判断是否为更高层级
        if(path.find('.') != std::string::npos) {
            if(isInt(path.substr(path.find_last_of('.') + 1, std::string::npos))) {
                std::string pre = path, post, pg, temp;
                unsigned int index;
                while(true) {
                    auto pos = pre.find_last_of('.');
                    post = pre.substr(pos + 1, std::string::npos);
                    pre = pre.substr(0, pos);

                    long subpos = pre.find_last_of('.');
                    if((unsigned long)subpos == std::string::npos) subpos = -1;
                    temp = pre.substr(subpos + 1, std::string::npos);

                    if(!isInt(temp)) {
                        index = std::stoi(post);
                        pg = pre.substr(0, subpos) + pg;
                        break;
                    } else {
                        pg = "." + post + pg;
                    }
                }

                while(index >= child.size()) {
                    std::vector<std::shared_ptr<JSONTemplateNode>> app;
                    for(auto &item : templ) {
                        app.push_back(item->getCopy());
                    }
                    child.push_back(app);
                }

                for(auto &item : child[index])
                    if(item->getIdentity() == LOOP) item->setArr(pg, arr);
                return;
            }
        }

        // 2. 填充数据
        for(unsigned int i = 0; i < min(child.size(), arr.size()); i++) {
            for(auto &item : child[i]) item->setVal(path, arr[i]);
        }

        // 3. 增添子项
        if(arr.size() > child.size()) {
            unsigned int boundary = child.size();
            for(unsigned int i = boundary; i < arr.size(); i++) {
                std::vector<std::shared_ptr<JSONTemplateNode>> app;
                for (auto &item : templ) {
                    auto sub = item->getCopy();
                    sub->setVal(path, arr[i]);
                    app.push_back(sub);
                }
                child.push_back(app);
            }
        }
    }

    const JSONTemplateNodeType getIdentity() override { return LOOP; }

    std::shared_ptr<JSONTemplateNode> getCopy() const override {
        std::vector<std::shared_ptr<JSONTemplateNode>> new_templ;
        for(auto &item : templ) new_templ.push_back(item->getCopy());

        std::vector<std::vector<std::shared_ptr<JSONTemplateNode>>> new_child;
        for(auto &sub : child) {
            std::vector<std::shared_ptr<JSONTemplateNode>> subCopy;
            for(auto &item: sub) subCopy.push_back(item->getCopy());
            new_child.push_back(subCopy);
        }

        return std::make_shared<JSONTemplateLoop>(JSONTemplateLoop(new_templ, new_child));
    };

    void reset() override {
        child.clear();
    };
};
#endif //CPP_REST_JSONTEMPLATELOOP_H

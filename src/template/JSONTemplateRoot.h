#ifndef CPP_REST_JSONTEMPLATEROOT_H
#define CPP_REST_JSONTEMPLATEROOT_H

#include "JSONTemplateNode.h"
#include "JSONTemplateLoop.h"
#include "JSONTemplateVar.h"
#include <vector>
#include <memory>

class JSONTemplateRoot : public JSONTemplateNode {
    std::vector<std::shared_ptr<JSONTemplateNode>> child;
public:
    enum _DFA_STATE_t {
        _append_normal,
        _append_val,
        _append_loop,
        _normal_slash,
        _loop_slash,
    };

    explicit JSONTemplateRoot(const char* s) {
        std::string cur;
        _DFA_STATE_t state = _append_normal;

        const char* ptr = s;

        int loopCounter = 0;

        while(*ptr != 0) {
            // 获取一个C
            char c = *(ptr++);

            // DFA!
            switch(state) {
                case _append_normal: {
                    switch (c) {
                        case '\\':
                            state = _normal_slash;
                            break;
                        case '@':
                            state = _append_val;
                            child.push_back(std::make_shared<JSONTemplateString>(JSONTemplateString(cur)));
                            cur = "";
                            break;
                        case '(':
                            child.push_back(std::make_shared<JSONTemplateString>(JSONTemplateString(cur)));
                            cur = "";
                            state = _append_loop;
                            break;
                        default:
                            cur += c;
                    }
                    break;
                }
                case _normal_slash: {
                    switch(c) {
                        case '\\':
                        case '@':
                        case '(':
                            cur += c;
                            break;
                        default:
                            cur += '\\';
                            cur += c;
                    }
                    state = _append_normal;
                    break;
                }
                case _append_val: {
                    if((c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A') || (c <= '9' && c >= '1') || c == '.' || c == '_') { // 合法变量名
                        cur += c;
                    } else { // 结束，重新分析
                        ptr--;
                        child.push_back(std::make_shared<JSONTemplateVar>(JSONTemplateVar(cur.c_str())));
                        cur = "";
                        state = _append_normal;
                    }
                    break;
                }
                case _append_loop: {
                    switch(c) {
                        case '\\':
                            state = _loop_slash;
                            break;
                        case '(':
                            loopCounter++;
                            cur += '(';
                            break;
                        case ')':
                            if(loopCounter == 0) {
                                JSONTemplateRoot j(cur.c_str());
                                // 利用已有的root生成loop
                                child.push_back(std::make_shared<JSONTemplateLoop>(JSONTemplateLoop(j.child)));
                                cur = "";
                                state = _append_normal;
                            } else {
                                cur += ')';
                                loopCounter--;
                            }
                            break;
                        default:
                            cur += c;
                    }
                    break;
                }
                case _loop_slash: {
                    cur += '\\';
                    cur += c;
                    state = _append_loop;
                    break;
                }
            }
        }

        if(state != _append_normal) throw;
        child.push_back(std::make_shared<JSONTemplateString>(JSONTemplateString(cur)));
    }

    std::string concatenate() override {
        std::string ret;
        for(auto &item : child) {
            ret += item->concatenate();
        }
        return ret;
    }
};

#endif //CPP_REST_JSONTEMPLATEROOT_H

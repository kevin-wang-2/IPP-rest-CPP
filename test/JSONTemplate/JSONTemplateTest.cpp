#include "../../src/template/JSONTemplateRoot.h"

int main() {
    JSONTemplateRoot j(R"({
        "a": @num1,
        "b": [{
            "b1": "@b1",
            "b2": true
        },
        ({
            "const": "\@\\\(",
            "var": @va
        })]
        })");
    j.concatenate();
}
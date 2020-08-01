#include "../../src/template/JSONTemplateRoot.h"
#include <iostream>

using namespace std;

int main() {
    JSONTemplateRoot j(R"(
        {
            "a": @num1,
            "b": [{
                "b1": @b1,
                "b2": true
            },
        ({
            "test": @c(@f),
            "const": false
        },)]
        }
    )");
    j.setVal("num1", 1);
    j.setVal("b1", "hello");
    j.setVal("c.0", "a");
    j.setVal("c.1", "b");
    j.setVal("f.0.0", "f10");
    j.setVal("f.0.1", "f11");
    j.setVal("f.1.0", "f20");
    cout << j.concatenate() << endl;
}
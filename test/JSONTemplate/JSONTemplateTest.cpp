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
            "test": "@c(.@f)\",
            "const": false
        },)]
        }
    )");

    JSONAny num1(1);
    j.set("num1", &num1);
    j.set("b1", JSONAny("hello"));
    j.set("c", HybridJSONType({"a", "b"}));
    j.setArr("f.0", {"f10", "f11"});
    j.setVal("f.1.0", "f20");
    cout << j.concatenate() << endl;
    num1 = 2;
    cout << j.concatenate() << endl;
    j.reset();
    j.setVal("num1", 1);
    j.setVal("b1", "hello");
    j.setArr("c", {"a"});
    j.setArr("f.0", {"f10", "f11"});

    cout << j.concatenate() << endl;
}
#include "IHTTPResponse.h"
#include <fstream>

using namespace std;

void IHTTPResponse::render(const std::string &fn) {
    ifstream ifs(fn);

    if(ifs.good()) {
        end(string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>()));
    }
}
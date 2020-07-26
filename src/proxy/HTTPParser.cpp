#include "HTTPParser.h"
#include <sstream>
#include <map>

using namespace std;

///// 辅助函数 /////

string capitalize(const string & s) {
    string ret = s;
    for(auto &i : ret) {
        if((i <= 'z') && (i >= 'a')) i += ('A' - 'a');
    }
    return ret;
}

///// 解释器 /////

/**
 * 处理HTTP头
 * @param input HTTP头信息
 * @return 经过解释的头信息
 */
HTTPHeader_t HTTPParser::parseHeader(const string &input) {
    istringstream iss(input);
    HTTPHeader_t header;

    // 1. 处理头部
    string head;
    getline(iss, head);
    parseHead(head, header);

    // 2. 逐行处理
    while(!iss.eof()) {
        string line;
        getline(iss, line);
        if(line.empty()) { // 后面开启报文体
            break;
        } else { // 仍在报文头, 进行分析
            // TODO: 分析报文头
        }
    }

    if(!iss.eof()) {
        header.body = iss.str();
    }

    return header;
}

/**
 * 处理HTTP头第一行的固定信息
 * @param head
 * @param result
 */
void HTTPParser::parseHead(const std::string& head, HTTPHeader_t &result) {
    istringstream issHead(head);

    string sMethod, sVersion;
    issHead >> sMethod >> result.path >> sVersion;
    if(sMethod.empty() || result.path.empty() || sVersion.empty()) throw InvalidHeader();

    /** 处理Method **/
    // 利用MAP优雅地处理多个if的问题, C++不自带反射真的烦
   const  map<string, HTTPMethod_t> mMethod = {
            {"GET", GET},
            {"POST", POST},
            {"PUT", PUT},
            {"PATCH", PATCH},
            {"DELETE", DELETE}
    };
    map<string, HTTPMethod_t>::const_iterator itMethod;
    if((itMethod = mMethod.find(capitalize(sMethod))) != mMethod.end()) result.method = (*itMethod).second;
    else result.method = OTHER;

    /** 处理Version **/
    // TODO: 更优雅的直接提取Version中的数字而不使用map
    const map<string, HTTPVersion_t> mVersion = {
            {"HTTP/1.1", HTTP_1_1},
            {"HTTP/2", HTTP_2}
    };

    map<string, HTTPVersion_t>::const_iterator itVersion;
    if((itVersion = mVersion.find(capitalize(sVersion))) != mVersion.end()) result.version = (*itVersion).second;
    else result.version = HTTP_1_1;
}
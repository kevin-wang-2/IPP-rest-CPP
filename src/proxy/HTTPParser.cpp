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

typedef void (*processor_t)(const string&, HTTPHeader_t&);

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
    cout << ">>>inline" << endl;
    while(!iss.eof()) {
        string line;
        getline(iss, line);
        if(line.length() <= 1) { // 后面开启报文体
            break;
        } else { // 仍在报文头, 进行分析
            unsigned long pos;
            if((pos = line.find(": ")) == string::npos) throw InvalidHeader();
            else {
                string key = line.substr(0, pos);
                string value = line.substr(pos + 2, line.length());

                // 处理分歧
                // TODO: 添加对于更多头信息的分析
                const map<string, processor_t> mFun = {
                        {"Host", parseHost},
                        {"Connection", parseConnection},
                        {"UA", parseUA}
                };
                map<string, processor_t>::const_iterator itFun;
                if((itFun = mFun.find(key)) != mFun.end()) (*itFun).second(value, header);
            }
        }
    }
    cout << "<<<inline" << endl;

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

/**
 * 处理HTTP头的Host信息
 * @param host
 * @param result
 */
void HTTPParser::parseHost(const std::string &host, HTTPHeader_t &result) {
    istringstream iss(host);

    string s[4];
    getline(iss, s[0], '.');
    getline(iss, s[1], '.');
    getline(iss, s[2], '.');
    getline(iss, s[3], ':');

    if(s[0].empty() || s[1].empty() || s[2].empty() || s[3].empty()) return; // 非法的Host，但是不影响大局

    result.host.IP[0] = stoi(s[0], nullptr, 10);
    result.host.IP[1] = stoi(s[1], nullptr, 10);
    result.host.IP[2] = stoi(s[2], nullptr, 10);
    result.host.IP[3] = stoi(s[3], nullptr, 10);

    unsigned long pos = host.find(':');

    if(pos == string::npos) { // 没有":", 默认端口为80
        result.host.port = 80;
    } else {
        result.host.port = stoi(host.substr(pos + 1, host.length() - pos - 1), nullptr, 10);
    }
}

/**
 * 处理HTTP头的Connection
 * @param host
 * @param result
 */
void HTTPParser::parseConnection(const std::string &host, HTTPHeader_t &result) {
    result.keepAlive = (host.find("keep-alive") != string::npos);
}

/**
 * 处理HTTP头的useragent
 * TODO: 拆分Useragent
 * @param UA
 * @param result
 */
void HTTPParser::parseUA(const std::string &UA, HTTPHeader_t &result) {
    result.UA = UA;
}
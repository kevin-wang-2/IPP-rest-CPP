#include "HTTPParser.h"
#include <sstream>
#include <regex>
#include "../utils/URL.h"

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

typedef void (*processor_t)(const string&, HTTPRequestHeader_t&);

/**
 * 处理HTTP头
 * @param input HTTP头信息
 * @return 经过解释的头信息
 */
HTTPRequestHeader_t HTTPParser::parseHeader(const string &input) {
    istringstream iss(input);
    HTTPRequestHeader_t header;

    // 1. 处理头部
    string head;
    getline(iss, head);
    parseHead(head, header);

    // 2. 逐行处理
    while(!iss.eof()) {
        string line;
        getline(iss, line);
        if(line.length() <= 1) { // 后面开启报文体
            break;
        } else { // 仍在报文头, 进行分析
            unsigned long pos;
            if((pos = line.find(':')) == string::npos) throw InvalidHeader();
            else {
                string key = line.substr(0, pos);
                string value;
                // 注意空格为可选项
                if(line.at(pos + 1) == ' ') value = line.substr(pos + 2, line.find('\r') - pos - 2);
                else value = line.substr(pos + 1, line.find('\r') - pos - 1);

                // 处理分歧
                // TODO: 添加对于更多头信息的分析
                const map<string, processor_t> mFun = {
                        {"Host", parseHost},
                        {"Connection", parseConnection},
                        {"User-Agent", parseUA},
                        {"Referer", parseReferer},
                        {"Accept", parseMIME}
                };
                map<string, processor_t>::const_iterator itFun;
                if((itFun = mFun.find(key)) != mFun.end()) itFun->second(value, header);
                else header.other[key] = value;
            }
        }
    }

    return header;
}

/**
 * 处理HTTP头第一行的固定信息
 * @param head
 * @param result
 */
void HTTPParser::parseHead(const std::string& head, HTTPRequestHeader_t &result) {
    istringstream issHead(head);

    string sMethod, sVersion;
    issHead >> sMethod >> result.path >> sVersion;
    result.path=UrlDecode(result.path);
    if(sMethod.empty() || result.path.empty() || sVersion.empty()) throw HTTPException(400);

    /** 处理Method **/
    // 利用MAP优雅地处理多个if的问题, C++不自带反射真的烦
   const  map<string, HTTPMethod_t> mMethod = {
            {"GET", GET},
            {"POST", POST},
            {"PUT", PUT},
            {"PATCH", PATCH},
            {"DELETE", DELETE},
            {"HEAD", HEAD},
            {"OPTIONS", OPTIONS},
            {"TRACE", TRACE},

   };
    map<string, HTTPMethod_t>::const_iterator itMethod;
    if((itMethod = mMethod.find(capitalize(sMethod))) != mMethod.end()) result.method = itMethod->second;
    else throw HTTPException(400);

    /** 处理Version **/
    // TODO: 更优雅的直接提取Version中的数字而不使用map
    const map<string, HTTPVersion_t> mVersion = {
            {"HTTP/1.0", HTTP_1},
            {"HTTP/1.1", HTTP_1_1},
            {"HTTP/2.0", HTTP_2}
    };

    map<string, HTTPVersion_t>::const_iterator itVersion;
    if((itVersion = mVersion.find(capitalize(sVersion))) != mVersion.end()) result.version = itVersion->second;
    else result.version = HTTP_1_1;

    // 默认属性
    if(result.version == HTTP_1) result.keepAlive = false;
}

/**
 * 处理HTTP头的Host信息
 * @param host
 * @param result
 */
void HTTPParser::parseHost(const std::string &host, HTTPRequestHeader_t &result) {
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
 * 处理Referer项
 * @param refString
 * @param result
 */
void HTTPParser::parseReferer(const std::string &refString, HTTPRequestHeader_t &result) {
    result.referer = refString;
}

/**
 * 处理HTTP头的Connection
 * @param host
 * @param result
 */
void HTTPParser::parseConnection(const std::string &host, HTTPRequestHeader_t &result) {
    result.keepAlive = (host.find("keep-alive") != string::npos);
}

/**
 * 处理HTTP头的useragent
 * TODO: 拆分Useragent
 * @param UA
 * @param result
 */
void HTTPParser::parseUA(const std::string &UA, HTTPRequestHeader_t &result) {
    result.UA = UA;
}

/**
 * 处理MIMES信息
 * @param MIMES
 * @param result
 */
void HTTPParser::parseMIME(const std::string &MIMES, HTTPRequestHeader_t &result) {
    // 1. 串为vector
    regex ws_re(",");
    vector<string> mime(sregex_token_iterator(MIMES.begin(), MIMES.end(), ws_re, -1), sregex_token_iterator());
    double curWeight = 1;

    for(auto &item : mime) {
        weightedMIME_t mCur {MIME_ANY, "*", curWeight};
        if(item.find(';') == string::npos) { // 无附加数据
            auto pos = item.find('/'); // 分隔符位置

            // 处理MIME主类型
            const map<string, MIMEMainType_t> mMainType = {
                    {"TEXT", MIME_TEXT},
                    {"MULTIPART", MIME_MULTIPART},
                    {"APPLICATION", MIME_APPLICATION},
                    {"MESSAGE", MIME_MESSAGE},
                    {"IMAGE", MIME_IMAGE},
                    {"AUDIO", MIME_AUDIO},
                    {"VIDEO", MIME_VIDEO},
                    {"DRAWING", MIME_DRAWING},
                    {"JAVA", MIME_JAVA},
                    {"MODEL", MIME_MODEL}
            };

            map<string, MIMEMainType_t>::const_iterator itMainItem;
            if((itMainItem = mMainType.find(capitalize(item.substr(0, pos)))) != mMainType.end()) mCur.mainType = itMainItem->second;

            mCur.subType = item.substr(pos + 1, item.find('\r') - pos - 1);
        } else { // 有附加数据
            regex regex2(";");
            vector<string> params(sregex_token_iterator(item.begin(), item.end(), regex2, -1), sregex_token_iterator());

            auto pos = params[0].find('/'); // 分隔符位置

            // 处理MIME主类型
            const map<string, MIMEMainType_t> mMainType = {
                    {"TEXT", MIME_TEXT},
                    {"MULTIPART", MIME_MULTIPART},
                    {"APPLICATION", MIME_APPLICATION},
                    {"MESSAGE", MIME_MESSAGE},
                    {"IMAGE", MIME_IMAGE},
                    {"AUDIO", MIME_AUDIO},
                    {"VIDEO", MIME_VIDEO},
                    {"DRAWING", MIME_DRAWING},
                    {"JAVA", MIME_JAVA},
                    {"MODEL", MIME_MODEL}
            };

            map<string, MIMEMainType_t>::const_iterator itMainItem;
            if((itMainItem = mMainType.find(capitalize(params[0].substr(0, pos)))) != mMainType.end()) mCur.mainType = itMainItem->second;

            mCur.subType = params[0].substr(pos + 1, params[0].find('\r') - pos - 1);

            for(auto &i : params) {
                if(i != params[0]) {
                    auto eqPos = i.find('=');

                    string paramType = i.substr(0, eqPos);
                    string paramVal = i.substr(eqPos + 1, i.find('\r') - eqPos  - 1);

                    if(paramType == "q") {
                        try {
                            curWeight = mCur.weight = stod(paramVal, nullptr);
                        } catch(...) {}
                    }
                }
            }
        }
        result.acceptedMIME.emplace_back(mCur);
    }
}
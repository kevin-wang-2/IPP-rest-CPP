#include <iostream>
#include <ctime>
#include <unordered_map>
#include <memory>
#include "CPPLogger.h"

#include <sys/time.h>

using namespace std;

///// 辅助函数 /////

static string getTime() {
    char szDT[128];
    tm *newtime;
    long ltime;
    time(&ltime);
    newtime = gmtime(&ltime);
    strftime(szDT, 128,"%d %b %Y %H:%M:%S.", newtime);

    struct timeval tv {};
    struct timezone tz {};
    gettimeofday(&tv, &tz);

    return string(szDT) + to_string(tv.tv_usec);
}

///// LOGGER 单例 /////

unordered_map<string, shared_ptr<CPPLogger>> mLogger;

CPPLogger& CPPLogger::getLogger(const std::string& _) {
    string fn = _ + ".run.log";
    string err_fn = _ + ".err.log";
    if(mLogger.empty()) {
        auto logger = make_shared<CPPLogger>(fn, err_fn);
        logger->loggers[LOG] = make_shared<CPPLogLogger>(CPPLogLogger(*logger));
        logger->loggers[WARN] = make_shared<CPPWarnLogger>(CPPWarnLogger(*logger));
        logger->loggers[ERROR] = make_shared<CPPErrLogger>(CPPErrLogger(*logger));
        logger->loggers[FATAL] = make_shared<CPPFatalLogger>(CPPFatalLogger(*logger));
        logger->loggers[DEBUG] = make_shared<CPPDebugLogger>(CPPDebugLogger(*logger));
        mLogger = unordered_map<string, shared_ptr<CPPLogger>>({
            {fn, logger}
        });
    } else if(mLogger.find(fn) == mLogger.end()) {
        auto logger = make_shared<CPPLogger>(fn, err_fn);
        logger->loggers[LOG] = make_shared<CPPLogLogger>(CPPLogLogger(*logger));
        logger->loggers[WARN] = make_shared<CPPWarnLogger>(CPPWarnLogger(*logger));
        logger->loggers[ERROR] = make_shared<CPPErrLogger>(CPPErrLogger(*logger));
        logger->loggers[FATAL] = make_shared<CPPFatalLogger>(CPPFatalLogger(*logger));
        logger->loggers[DEBUG] = make_shared<CPPDebugLogger>(CPPDebugLogger(*logger));
        mLogger.insert({fn, logger});

    }

    return *mLogger[fn];
}

///// LOGGER 基类 /////

CPPLoggerBase& CPPLoggerBase::operator<<(const std::string &s) {
    while(parent.IOLock);
    parent.IOLock = true;
    log(s, begin);
    begin = false;
    parent.IOLock = false;
    return *this;
}

CPPLoggerBase &CPPLoggerBase::operator<<(ostream &(*op)(std::ostream &)) {
    while(parent.IOLock);
    parent.IOLock = true;
    log("", begin, true);
    begin = true;
    parent.IOLock = false;
    return *this;
}


///// LOGGER 实例类 /////

void CPPLogLogger::log(const std::string &s, bool begin, bool end) {
    if(begin) {
        cout.flush();
        ofs << "[LOG] - "
            << getTime()
            << " - ";
        if(parent.level() <= LOG)
            cout << "[LOG] - "
                 << getTime()
                 << " - ";
    }
    ofs << s;
    if(parent.level() <= LOG)
        cout << s;
    if(end) {
        ofs << endl;
        if(parent.level() <= LOG)
            cout << endl;
    }
}

void CPPWarnLogger::log(const std::string &s, bool begin, bool end) {
    if(begin) {
        cout.flush();
        ofs << "[WARN] - "
            << getTime()
            << " - ";
        if(parent.level() <= WARN)
            cout << "[WARN] - "
                 << getTime()
                 << " - ";
    }
    ofs << s;
    if(parent.level() <= WARN)
        cout << s;
    if(end) {
        ofs << endl;
        if(parent.level() <= WARN)
            cout << endl;
    }
}

void CPPErrLogger::log(const std::string &s, bool begin, bool end) {
    if(begin) {
        cerr.flush();
        ofs << "[ERROR] - "
            << getTime()
            << " - ";
        err_ofs << "[ERROR] - "
            << getTime()
            << " - ";
        if(parent.level() <= ERROR)
            cerr << "[ERROR] - "
                 << getTime()
                 << " - ";
    }
    ofs << s;
    err_ofs << s;
    if(parent.level() <= ERROR)
        cerr << s;
    if(end) {
        ofs << endl;
        if(parent.level() <= ERROR)
            cerr << endl;
    }

    // Print Trace
    if(end) {
        ofs << "Trace: ";
        err_ofs << "Trace: ";
        if(parent.level() <= ERROR)
            cerr << "Trace: ";
        for (auto &i : parent.getTrace()) {
            ofs << i << "/";
            err_ofs << i << "/";
            if(parent.level() <= ERROR)
                cerr << i << "/";
        }
        ofs << endl;
        err_ofs << endl;
        if(parent.level() <= ERROR)
            cerr << endl;
    }
}

void CPPFatalLogger::log(const std::string &s, bool begin, bool end) {
    if(begin) {
        cerr.flush();
        ofs << "[FATAL] - "
            << getTime()
            << " - ";
        err_ofs << "[FATAL] - "
            << getTime()
            << " - ";
        cerr << "[FATAL] - "
             << getTime()
             << " - ";
    }
    ofs << s;
    err_ofs << s;
    cerr << s;
    if(end) {
        ofs << endl;
        if(parent.level() <= ERROR)
            cerr << endl;
    }

    // Print Trace
    if(end) {
        ofs << "Trace: ";
        err_ofs << "Trace: ";
        if(parent.level() <= ERROR)
            cerr << "Trace: ";
        for (auto &i : parent.getTrace()) {
            ofs << i << "/";
            err_ofs << i << "/";
            if(parent.level() <= ERROR)
                cerr << i << "/";
        }
        ofs << endl;
        err_ofs << endl;
        if(parent.level() <= ERROR)
            cerr << endl;
    }
}

void CPPDebugLogger::log(const std::string &s, bool begin, bool end) {
    if(parent.level() != DEBUG) return;
    if(begin) {
        cout.flush();
        cout << "[DEBUG]";
    }
    cout << s;
    if(end) cout << endl;
}

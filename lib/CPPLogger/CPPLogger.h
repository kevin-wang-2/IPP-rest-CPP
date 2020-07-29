#ifndef _CPPLOGGER_H
#define _CPPLOGGER_H

#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include <map>
#include <utility>
#include <vector>

#if defined(__linux__) || defined(__linux)
#   define LINUX
#endif
#if !defined(SAG_COM) && (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
#   define WIN
#endif
#if !defined(WIN) && !defined(LINUX)
#   error "Platform not supported"
#endif

class CPPLoggerBase;

enum CPPLoggerLevel {
    DEBUG, LOG, WARN, ERROR, FATAL
};

class CPPLogger {
    std::map<CPPLoggerLevel, std::shared_ptr<CPPLoggerBase>> loggers;

    std::string fn;
    std::string err_fn;
    std::ofstream ofs;
    std::ofstream err_ofs;

    bool IOLock = false;
    CPPLoggerLevel _level = ERROR;

    std::vector<std::string> _trace;

    friend CPPLoggerBase;
public:
    explicit CPPLogger(const std::string&  _, const std::string& _err) : fn(_), err_fn(_err, std::ios_base::app), ofs(_), err_ofs(_err, std::ios_base::app) {}

    CPPLogger(CPPLogger& logger) = delete;

    const std::vector<std::string>& getTrace() const { return _trace; }

    static CPPLogger& getLogger(const std::string& _);

    CPPLoggerBase& operator()(CPPLoggerLevel level) { return *loggers[level]; }

    void enter(const std::string& _) { _trace.emplace_back(_); }

    void exit() { _trace.pop_back(); }

    void level(CPPLoggerLevel level) { _level = level; }

    CPPLoggerLevel  level() { return _level; }
};

class CPPLoggerBase {
    bool begin = true;
    std::string fn;

    friend CPPLogger;
protected:
    explicit CPPLoggerBase(CPPLogger& p) : parent(p), ofs(p.ofs), err_ofs(p.err_ofs) {}
    CPPLogger& parent;
    std::ofstream& ofs;
    std::ofstream& err_ofs;
public:
    CPPLoggerBase& operator<<(const std::string& s);
    CPPLoggerBase& operator<<(int i) { return operator<<(std::to_string(i)); }
    CPPLoggerBase& operator<<(double i) { return operator<<(std::to_string(i)); }
    CPPLoggerBase& operator<<(std::ostream& (*op) (std::ostream&));

    virtual void log(const std::string& s, bool begin = false, bool end = false) = 0;
};

class CPPLogLogger : public CPPLoggerBase {
public:
    explicit CPPLogLogger(CPPLogger& p) : CPPLoggerBase(p) {}
    void log(const std::string& s, bool begin = false, bool end = false) override;
};

class CPPWarnLogger : public CPPLoggerBase {
public:
    explicit CPPWarnLogger(CPPLogger& p) : CPPLoggerBase(p) {}
    void log(const std::string& s, bool begin = false, bool end = false) override;
};

class CPPErrLogger : public CPPLoggerBase {
public:
    explicit CPPErrLogger(CPPLogger& p) : CPPLoggerBase(p) {}
    void log(const std::string& s, bool begin = false, bool end = false) override;
};

class CPPFatalLogger : public CPPLoggerBase {
public:
    explicit CPPFatalLogger(CPPLogger& p) : CPPLoggerBase(p) {}
    void log(const std::string& s, bool begin = false, bool end = false) override;
};

class CPPDebugLogger : public CPPLoggerBase {
public:
    explicit CPPDebugLogger(CPPLogger& p) : CPPLoggerBase(p) {}
    void log(const std::string& s, bool begin = false, bool end = false) override;
};

#endif //_CPPLOGGER_H

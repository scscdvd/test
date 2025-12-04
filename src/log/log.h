#ifndef _LOG_H
#define _LOG_H

#define LOGURU_WITH_STREAMS 1

#include <iostream>
#include <fstream>
#include <sstream>
#include <mutex>
#include "loguru.hpp"



#define Verbosity_DEBUG loguru::Verbosity_1
/*日志记录*/
#define LOG_INFO     LOG_S(INFO)
#define LOG_WARNING  LOG_S(WARNING)
#define LOG_ERROR    LOG_S(ERROR)



#define DEBUG_PRINT LOG()     /*打印*/


class LOG
{
public:
    LOG();
    ~LOG();

    LOG& operator<<(const char* str)
    {
        if (debugEnable)
        {
            oss_ << (str ? str : "(null)");
        }
        return *this;
    }
    template<class T>
    LOG& operator<<(const T& value)
    {
        if(debugEnable)
        {
            oss_ << value;
        }
        return *this;
    }
    static void setDebugEnable(bool enable);   /*设置打印是否使能*/
    static void init(int argc, char* argv[]);/*日志初始化*/

public:
    /*设置日志路径*/
    static void setLogPath(const std::string& path);
    /*设置日志名*/
    static void setLogName(const std::string& fileName);

private:
    static void myLog(void* user_data,const loguru::Message& message);

    static std::string getLevel(loguru::Verbosity ver);

private:
    std::ostringstream oss_;
    static bool debugEnable;

    static std::string logPath ;
    static std::string fileName_;
    
    static std::string currentDate;

    static std::mutex mutex_;

public:
    static std::ofstream logFile;

};





#endif
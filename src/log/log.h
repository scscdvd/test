#ifndef _LOG_H
#define _LOG_H

#include "global.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

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

private:
    std::ostringstream oss_;
    static bool debugEnable;

    static const std::string logPath ;

};

#define DEBUG LOG()     /*打印*/



#endif
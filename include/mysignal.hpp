#ifndef _MYSIGNAL_HPP
#define _MYSIGNAL_HPP

#include <signal.h>
#include <iostream>
#include "system.h"

class MySignal
{
public:
    static MySignal& Instance() noexcept
    {
        static MySignal instance;
        return instance;
    }

    MySignal& operator=(const MySignal&) = delete;
    MySignal(const MySignal&) = delete;

    void setupSignalHandlers()
    {
        struct sigaction sa;
        sa.sa_handler = signalHandle;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;

        // 设置需要处理的信号
        sigaction(SIGINT, &sa, nullptr);  // 处理 Ctrl+C 信号
        sigaction(SIGTERM, &sa, nullptr); // 处理终止信号
    }

    static void signalHandle(int signum)
    {
        std::cout << "system exit!!! " << std::endl;
        System::Instance().stopSystem();
    }
protected:
    MySignal() = default;
    ~MySignal() = default;
};


#endif //_MYSIGNAL_HPP
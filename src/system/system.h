#ifndef _SYSTEM_H
#define _SYSTEM_H

#include "tcpserver.h"


class System
{
public:

    static System& Instance() noexcept
    {
        static System instance;
        return instance;
    }


    System(const System&) = delete;
    System& operator=(const System&) = delete;

    /*启动整个系统*/
    void startSystem();
    /*停止整个系统*/
    void stopSystem();

    /*系统运行状态*/
    bool isRunning() const
    {
        return running_;
    }
private:
    System();
    ~System();
private:
    tcpServer server_;

    std::atomic<bool> running_;
};

#endif
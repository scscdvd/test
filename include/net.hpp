#ifndef _NET_HPP
#define _NET_HPP

#include <iostream>
#include <atomic>

class Net
{
public:

    Net() : running_(false) {}
    virtual ~Net() = default;

    /*禁止拷贝*/
    Net(const Net&) = delete;
    Net& operator=(const Net&) = delete;
    
    /*开始网络*/
    virtual void start() = 0;

    /*停止网络*/
    virtual void stop() = 0;

    /*获取运行状态*/
    bool isRunning() const
    {
        return running_;
    }
    /*发送数据*/
    virtual int sendData(const void* data,int len) = 0;

protected:
    std::atomic<bool> running_;         /*网络是否在运行*/
};



#endif //_NET_HPP
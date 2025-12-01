#ifndef _NET_HPP
#define _NET_HPP

#include <iostream>
#include <atomic>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <netdb.h>


/*网络基类*/
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

    struct clientInfo
    {
        int socketFd_;
        socklen_t addrLen_;
        sockaddr_in addr_;
    };
    using clientInfo = struct clientInfo;
protected:
    std::atomic<bool> running_;         /*网络是否在运行*/
};



#endif //_NET_HPP
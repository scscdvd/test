#ifndef _TCPSERVER_H
#define _TCPSERVER_H
#include "global.h"


class tcpServer : public Net
{
public:
    tcpServer();
    ~tcpServer();

    bool init(const std::string& ip ,unsigned short port);

    void start() override;
    void stop() override;

protected: 

    /*服务器线程*/
    void serverThread();
private:
    int socketFd_;    /*socket文件描述符*/
    int exitFd_;    //退出通知
    std::string ip_; /*服务器IP地址*/
    unsigned short port_; /*服务器端口号*/

    std::thread serverThread_; /*服务器线程*/

    int epollFd_; /*epoll文件描述符*/  
};



#endif //_TCPSERVER_H
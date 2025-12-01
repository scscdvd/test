#ifndef _TCPSERVER_H
#define _TCPSERVER_H
#include <iostream>
#include <thread>
#include "net.hpp"
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

class tcpServer : public Net
{
public:
    tcpServer(const std::string& ip ,unsigned short port);
    ~tcpServer();

    void start() override;
    void stop() override;

protected: 
    int sendData(const void* data,int len) override;

    /*服务器线程*/
    void serverThread();
private:
    int socketFd_;    /*socket文件描述符*/
    std::string ip_; /*服务器IP地址*/
    unsigned short port_; /*服务器端口号*/

    std::thread serverThread_; /*服务器线程*/
};


#endif //_TCPSERVER_H
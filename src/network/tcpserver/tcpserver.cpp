#include "tcpserver.h"
 #include <thread>
 #include <fstream> 
#include <unistd.h> 
#include <cstdlib> 
#include <sys/epoll.h> 
#include <fcntl.h> 
#include <sys/eventfd.h>

/****************************tcpServer*******************************/

tcpServer::tcpServer() : socketFd_(-1),ip_(variableManager::Instance().ANY_IP), 
                                            port_(variableManager::Instance().TCP_SERVER_PORT),
                                            epollFd_(-1), exitFd_(-1)
{
    DEBUG << "tcpServer created" ;
}

/*服务器初始化*/
bool tcpServer::init(const std::string& ip ,unsigned short port)
{
    ip_ = ip;
    port_ = port;
    DEBUG << "tcpserver listen port: " << port_ ;
    socketFd_ = socket(AF_INET, SOCK_STREAM, 0);
    if(socketFd_ == -1)
    {
        DEBUG << "Failed to create socket" ;
        return false;
    }
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(ip_.c_str());
    serverAddr.sin_port = htons(port_);

    int res = 1;
    setsockopt(socketFd_, SOL_SOCKET, SO_REUSEADDR, &res, sizeof(res));/*端口复用*/

    int flag = fcntl(socketFd_, F_GETFL, 0);
    fcntl(socketFd_, F_SETFL, flag | O_NONBLOCK); /*设置为非阻塞*/

    if(bind(socketFd_, (const sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        DEBUG << "Bind failed" ;
        ::close(socketFd_);
        socketFd_ = -1;
        return false;
    }
    listen(socketFd_, variableManager::Instance().MAX_LISTEN);
    return true;
}
/*服务器开始运行*/
void tcpServer::start()
{
    if(running_)
    {
        return;
    }
    DEBUG << "tcpServer started" ;
    serverThread_ = std::thread(&tcpServer::serverThread , this);
    running_ = true;
}

/*epoll监听客户端连接和客户端的数据*/
void tcpServer::serverThread()
{
    DEBUG << "tcpServer serverThread running";
    epoll_event event,events[variableManager::Instance().MAX_EVENTS];
    epollFd_ = epoll_create(1);
    if(epollFd_ == -1)
    {
        DEBUG << "epoll_create1 failed" ;
        return;
    }
    exitFd_ = eventfd(0,EFD_NONBLOCK);
    if(exitFd_ < 0)
    {
        ::close(exitFd_);
        exitFd_ = -1;
        return;
    }
    /*线程通知*/
    event.events = EPOLLIN;
    event.data.fd = exitFd_;
    if(epoll_ctl(epollFd_, EPOLL_CTL_ADD, exitFd_, &event) == -1)
    {
        DEBUG << "epoll_ctl ADD exitFd_ failed" ;
        return;
    }
    /*服务器*/
    event.events = EPOLLIN;
    event.data.fd = socketFd_;
    if(epoll_ctl(epollFd_, EPOLL_CTL_ADD, socketFd_, &event) == -1)
    {
        DEBUG << "epoll_ctl ADD socketFd_ failed" ;
        return;
    }

    while (running_)
    {
        int ret = epoll_wait(epollFd_, events, variableManager::Instance().MAX_EVENTS, -1);
        if(ret == -1)
        {
            DEBUG << "epoll_wait failed" ;
            break;
        }
        for(int i = 0; i < ret; ++i)
        {
            if(events[i].data.fd == exitFd_)
            {
                DEBUG << "tcpServer serverThread received exit signal" ;
                uint64_t tmp;
                read(exitFd_,&tmp,sizeof(tmp));
                goto exitThread;
            }
            else if(events[i].data.fd == socketFd_)
            {
                clientInfo client;
                client.addrLen_ = sizeof(client.addr_);
                client.socketFd_ = accept(socketFd_, (sockaddr*)&client.addr_, &client.addrLen_);
                if(client.socketFd_ == -1)
                {
                    DEBUG << "Accept failed" ;
                    continue;
                }
                DEBUG << "New client connected, socketFd: " << client.socketFd_ ;
                DEBUG << "Client IP: " << inet_ntoa(client.addr_.sin_addr) 
                          << ", Port: " << ntohs(client.addr_.sin_port) ;
                event.events = EPOLLIN | EPOLLET;
                event.data.fd = client.socketFd_;   
                if(epoll_ctl(epollFd_, EPOLL_CTL_ADD, client.socketFd_, &event) == -1)
                {
                    DEBUG << "epoll_ctl ADD client socketFd failed" ;
                    ::close(client.socketFd_);
                    continue;
                }
            }
            else
            {
                char buffer[variableManager::Instance().BUFFER_SIZE];
                memset(buffer, 0, variableManager::Instance().BUFFER_SIZE);
                int bytesRead = read(events[i].data.fd,buffer, variableManager::Instance().BUFFER_SIZE);
                if(bytesRead <= 0)
                {
                    DEBUG << "Client disconnected, socketFd: " << events[i].data.fd ;
                    epoll_ctl(epollFd_, EPOLL_CTL_DEL, events[i].data.fd, NULL);
                    ::close(events[i].data.fd);
                }
                else
                {
                    int len = write(events[i].data.fd, buffer, bytesRead);
                }
            }
        }
    }
    exitThread:
    if(epollFd_ != -1)
    {
        ::close(epollFd_);
        epollFd_ = -1;
    }
    if(exitFd_ != -1)
    {
        ::close(exitFd_);
        exitFd_ = -1;
    }
    if(socketFd_ != -1)
    {
        ::close(socketFd_);
        socketFd_ = -1;
    }
    DEBUG << "tcpServer serverThread exiting" ;
}
/*停止服务器*/
void tcpServer::stop()
{
    
    if(!running_)
    {
        return;
    }
    if(exitFd_ != -1)
    {
        uint64_t val = 1;
        write(exitFd_,&val,sizeof(val));
    }
    if(epollFd_ != -1)
    {
        ::close(epollFd_);
        epollFd_ = -1;
    }
    if(socketFd_ != -1)
    {
        ::close(socketFd_);
        socketFd_ = -1;
    }
    running_ = false;
    
    if(serverThread_.joinable())
    {
        serverThread_.join();
    }
    DEBUG << "tcpServer stopped" ;
    
}
/*服务器对象销毁*/
tcpServer::~tcpServer()
{
    stop();
    DEBUG << "tcpServer destroyed" ;
}


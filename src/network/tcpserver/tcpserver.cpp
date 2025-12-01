#include "tcpserver.h"

/****************************tcpServer*******************************/
tcpServer::tcpServer(const std::string& ip ,unsigned short port) : socketFd_(-1),
                                                     ip_(ip), port_(port)
{
    std::cout << "class tcpServer created" << std::endl;
    std::cout << "IP: " << ip_ << ", Port: " << port_ << std::endl;
    socketFd_ = socket(AF_INET, SOCK_STREAM, 0);
    if(socketFd_ == -1)
    {
        std::cerr << "Failed to create socket" << std::endl;
        return;
    }
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(ip_.c_str());
    serverAddr.sin_port = htons(port_);

    int res = 1;
    setsockopt(socketFd_, SOL_SOCKET, SO_REUSEADDR, &res, sizeof(int));
    if(bind(socketFd_, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cerr << "Bind failed" << std::endl;
        ::close(socketFd_);
        socketFd_ = -1;
        return;
    }
    listen(socketFd_, 5);
}

void tcpServer::start()
{
    if(running_)
    {
        std::cout << "tcpServer is already running" << std::endl;
        return;
    }
    std::cout << "tcpServer started" << std::endl;
    serverThread_ = std::thread(&tcpServer::serverThread , this);
    running_ = true;
}
void tcpServer::serverThread()
{
    std::cout << "tcpServer serverThread running" << std::endl;
    while (running_)
    {
        // 模拟处理连接
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "tcpServer serverThread working..." << std::endl;
    }
    std::cout << "tcpServer serverThread exiting" << std::endl;
}
void tcpServer::stop()
{
    if(!running_)
    {
        std::cout << "tcpServer is not running" << std::endl;
        return;
    }
    running_ = false;
    if(serverThread_.joinable())
    {
        serverThread_.join();
    }
    std::cout << "tcpServer stopped" << std::endl;
    
}
int tcpServer::sendData(const void* data,int len)
{
    std::cout << "tcpServer sendData called, len: " << len << std::endl;
    return len;
}

tcpServer::~tcpServer()
{
    stop();
    std::cout << "class tcpServer destroyed" << std::endl;
}


#include "udp.h"
 #include <thread>
 #include <fstream> 
#include <unistd.h> 
#include <cstdlib> 
#include "log.h"

UDP::UDP(): udpIP_(variableManager::Instance().ANY_IP), udpPort_(variableManager::Instance().UDP_PORT), mode_(Mode::NORMAL), udpSocketFd_(-1)
{
    DEBUG_PRINT << "UDP created" ;
    LOG_INFO << "UDP created" ;
}
UDP::~UDP()
{
    stop();
    DEBUG_PRINT << "UDP destroyed" ;
    LOG_INFO << "UDP destroyed" ;
}
void UDP::setMode(Mode mode)
{
    if(running_)
    {
        return;
    }
    mode_ = mode;
}
void UDP::setUDPPort(const std::string& udpIP,unsigned short port)
{
    if(running_)
    {
        return;
    }
    udpIP_ = udpIP;
    udpPort_ = port;
}
void UDP::init()
{
    // 初始化UDP服务器的代码
    udpSocketFd_ = socket(AF_INET, SOCK_DGRAM, 0);
    if(udpSocketFd_ < 0)
    {
        DEBUG_PRINT << "Failed to create UDP socket" ;
        LOG_ERROR << "Failed to create UDP socket" ;
        return;
    }
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(udpIP_.c_str());
    serverAddr.sin_port = htons(udpPort_);

    int opt = 1;
    setsockopt(udpSocketFd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if(mode_ == Mode::BROADCAST)
    {
        int optval = 1;
        setsockopt(udpSocketFd_, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval));
    }

    if(bind(udpSocketFd_, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        DEBUG_PRINT << "Failed to bind UDP socket" ;
        LOG_ERROR << "Failed to bind UDP socket" ;
        close(udpSocketFd_);
        udpSocketFd_ = -1;
        return;
    }
     if(mode_ == Mode::BROADCAST)
    {
        DEBUG_PRINT << "udp broadcast port:" << udpPort_ ;
        LOG_INFO << "udp broadcast port:" << udpPort_ ;
    }
    else
    {
        DEBUG_PRINT << "udp port:" << udpPort_ ;
        LOG_INFO << "udp port:" << udpPort_ ;
    }
    
}

void UDP::start()
{
    if (running_)
    {
        return;
    }
    running_ = true;
    UDPThread_ = std::thread(&UDP::udpThread, this);
    DEBUG_PRINT << "UDP started" ;
    LOG_INFO << "UDP started" ;
}

void UDP::udpThread()
{
    char buffer[variableManager::Instance().BUFFER_SIZE];
    sockaddr_in broadcastAddr;
    clientInfo client;
    client.addrLen_ = sizeof(client.addr_);
    
    if(mode_ == Mode::BROADCAST)/*如果是广播，构建广播地址 */
    {
        DEBUG_PRINT << "UDP working in BROADCAST mode" ;
        LOG_INFO << "UDP working in BROADCAST mode" ;
        broadcastAddr.sin_family = AF_INET;
        broadcastAddr.sin_addr.s_addr = inet_addr(variableManager::Instance().BROADCAST_IP);
        broadcastAddr.sin_port = htons(variableManager::Instance().BROADCAST_PORT);
    }

    while (running_)
    {
        memset(buffer, 0, variableManager::Instance().BUFFER_SIZE);
        ssize_t recvLen = recvfrom(udpSocketFd_, buffer, variableManager::Instance().BUFFER_SIZE, 0,
                                    (sockaddr*)&client.addr_, &client.addrLen_);
        if (recvLen > 0)
        {
            if(mode_ == Mode::BROADCAST)/*如果是广播模式，回复广播地址*/
            {
                DEBUG_PRINT << "Broadcasting UDP message to "
                            << variableManager::Instance().BROADCAST_IP << ":"
                            << variableManager::Instance().BROADCAST_PORT << " buffer:" << buffer;
                LOG_INFO << "Broadcasting UDP message to "
                            << variableManager::Instance().BROADCAST_IP << ":"
                            << variableManager::Instance().BROADCAST_PORT << " buffer:" << buffer;
                if(std::string(buffer) == "what's your IP?")
                {
                    DEBUG_PRINT << "get local IP" ;
                    LOG_INFO << "get local IP" ;
                }
            }
            else
            {
                DEBUG_PRINT << "Replying to UDP client: "
                            << inet_ntoa(client.addr_.sin_addr) << ":"
                            << ntohs(client.addr_.sin_port) << " buffer:" << buffer;
                LOG_INFO << "Replying to UDP client: "
                            << inet_ntoa(client.addr_.sin_addr) << ":"
                            << ntohs(client.addr_.sin_port) << " buffer:" << buffer;
            }
        }
        else if (recvLen <= 0)
        {
            DEBUG_PRINT << "Socket has been closed data,exit" ;
            LOG_WARNING << "Socket has been closed data,exit" ;
            // Socket has been closed, exit the loop
            return;
        }
        
    }
}
void UDP::stop()
{
    if (!running_)
    {
        return;
    }
    running_ = false;
    if(udpSocketFd_ != -1)
    {
        shutdown(udpSocketFd_, SHUT_RD);
        ::close(udpSocketFd_);
        udpSocketFd_ = -1;
    }

    if(UDPThread_.joinable())
    {
        UDPThread_.join();
    }
    DEBUG_PRINT << "UDP stopped" ;
    LOG_INFO << "UDP stopped" ;
}

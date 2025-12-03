#include "tcpclient.h"
#include <fstream> 
#include <unistd.h> 
#include <cstdlib> 

tcpClient::tcpClient(): socketFd_(-1),
      serverIp_(variableManager::Instance().SERVER_IP),
      serverPort_(variableManager::Instance().SERVER_PORT),
      isConnected_(false)

{
    DEBUG << "tcpClient create" ;
}


void tcpClient::start()
{
    if (running_)
    {
        return;
    }
    running_ = true;
    clientThread_ = std::thread(&tcpClient::clientThread, this);
    DEBUG << "tcpClient started" ;
}

void tcpClient::setServer(const std::string& serverIp, unsigned short serverPort)
{
    serverIp_ = serverIp;
    serverPort_ = serverPort;
}
bool tcpClient::connect()
{
    socketFd_ = socket(AF_INET, SOCK_STREAM, 0);
    if(socketFd_ == -1)
    {
        DEBUG << "Failed to create socket" ;
        return isConnected_;
    }
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort_);;
    serverAddr.sin_addr.s_addr = inet_addr(serverIp_.c_str());

    int opt = 1;
    setsockopt(socketFd_, SOL_SOCKET, SO_KEEPALIVE, (void *)&opt, sizeof(opt));//保活
    int timecnt = 3;
    setsockopt(socketFd_, IPPROTO_TCP, TCP_SYNCNT, &timecnt, sizeof(timecnt));//设置connect超时等待时间

    if(::connect(socketFd_, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
    {
        DEBUG << "Failed to connect to server";
        ::close(socketFd_);
        socketFd_ = -1;
        return isConnected_;
    }
    DEBUG << "Connected to server " << serverIp_ << ":" << serverPort_ ;
    isConnected_ = true;
    return isConnected_;
}
void tcpClient::clientThread()
{
    DEBUG << "Client thread started" ;
    while (running_)
    {
        if(isConnected_ == false)
        {
            if(!connect())
            {
                 /*没有连接上*/
                DEBUG << "Retrying to connect to server..." ;
                if(socketFd_ != -1)
                {
                    ::close(socketFd_);
                    socketFd_ = -1;
                }
                std::this_thread::sleep_for(std::chrono::seconds(1));
                continue;
            }
            else
            {
                DEBUG << "Connection success" ;
                // 连接成功
                std::thread th(&tcpClient::receiveThread,this);
                th.detach();
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    DEBUG << "Client thread stopped" ;
}

void tcpClient::receiveThread()
{
    DEBUG << "Receive thread started" ;
    char buffer[variableManager::Instance().BUFFER_SIZE];
    while (running_ && isConnected_)
    {
        memset(buffer, 0, variableManager::Instance().BUFFER_SIZE);
        ssize_t bytesRead = recv(socketFd_, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead > 0)
        {
            buffer[bytesRead] = '\0';
            DEBUG << "Received: " << buffer ;
        }
        else
        {
            DEBUG << "client disconnection";
            isConnected_ = false;
            ::close(socketFd_);
            socketFd_ = -1;
        }
        
    }
    DEBUG << "Receive thread stopped";
}

void tcpClient::stop()
{
    if (!running_)
    {
        return;
    }
    running_ = false;
    isConnected_ = false;
    if(socketFd_ != -1)
    {
        ::close(socketFd_);
        socketFd_ = -1;
    }
    
    if (clientThread_.joinable())
    {
        clientThread_.join();
    }
    DEBUG << "tcpClient stopped" ;
}


tcpClient::~tcpClient()
{
    stop();
    DEBUG << "tcpClient destroy" ;
}
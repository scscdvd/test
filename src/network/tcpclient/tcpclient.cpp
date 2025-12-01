#include "tcpclient.h"

tcpClient::tcpClient(): socketFd_(-1),
      serverIp_(""),
      serverPort_(0),
      isConnected_(false)

{
    std::cout << "tcpClient create" << std::endl;
}


void tcpClient::start()
{
    if (running_)
    {
        return;
    }
    running_ = true;
    clientThread_ = std::thread(&tcpClient::clientThread, this);
    std::cout << "tcpClient started" << std::endl;
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
        std::cerr << "Failed to create socket" << std::endl;
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
        std::cerr << "Failed to connect to server" << std::endl;
        ::close(socketFd_);
        socketFd_ = -1;
        return isConnected_;
    }
    std::cout << "Connected to server " << serverIp_ << ":" << serverPort_ << std::endl;
    isConnected_ = true;
    return isConnected_;
}
void tcpClient::clientThread()
{
    std::cout << "Client thread started" << std::endl;
    while (running_)
    {
        if(isConnected_ == false)
        {
            if(!connect())
            {
                 /*没有连接上*/
                std::cout << "Retrying to connect to server..." << std::endl;
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
                std::cout << "Connection success" << std::endl;
                // 连接成功
                std::thread th(&tcpClient::receiveThread,this);
                th.detach();
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "Client thread stopped" << std::endl;
}

void tcpClient::receiveThread()
{
    std::cout << "Receive thread started" << std::endl;
    char buffer[BUFFER_SIZE];
    while (running_ && isConnected_)
    {
        ssize_t bytesRead = recv(socketFd_, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead > 0)
        {
            buffer[bytesRead] = '\0';
            std::cout << "Received: " << buffer << std::endl;
        }
        else
        {
            std::cout << "client disconnection" << std::endl;
            isConnected_ = false;
            ::close(socketFd_);
            socketFd_ = -1;
        }
    }
    std::cout << "Receive thread stopped" << std::endl;
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
    std::cout << "tcpClient stopped" << std::endl;
}


tcpClient::~tcpClient()
{
    stop();
    std::cout << "tcpClient destroy" << std::endl;
}
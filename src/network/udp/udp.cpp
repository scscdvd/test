#include "udp.h"

const unsigned short BROADCAST_PORT = 5555;
const std::string BROADCAST_IP = "255.255.255.255";
UDP::UDP(): udpIP_(""), udpPort_(0), mode_(Mode::NORMAL), udpSocketFd_(-1)
{
    std::cout << "UDP created" << std::endl;
}
UDP::~UDP()
{
    stop();
    std::cout << "UDP destroyed" << std::endl;
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
        std::cerr << "Failed to create UDP socket" << std::endl;
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
        std::cerr << "Failed to bind UDP socket" << std::endl;
        close(udpSocketFd_);
        udpSocketFd_ = -1;
        return;
    }
    std::cout << "local all IP ,port:" << udpPort_ << std::endl;
}

void UDP::start()
{
    if (running_)
    {
        return;
    }
    running_ = true;
    UDPThread_ = std::thread([this]()
    {
        char buffer[BUFFER_SIZE];
        sockaddr_in broadcastAddr;
        
        if(mode_ == Mode::BROADCAST)/*如果是广播，构建广播地址 */
        {
            std::cout << "UDP working in BROADCAST mode" << std::endl;
            broadcastAddr.sin_family = AF_INET;
            broadcastAddr.sin_addr.s_addr = inet_addr(BROADCAST_IP.c_str());
            broadcastAddr.sin_port = htons(BROADCAST_PORT);
        }

        while (running_)
        {
            clientInfo client;
            client.addrLen_ = sizeof(client.addr_);
            ssize_t recvLen = recvfrom(udpSocketFd_, buffer, BUFFER_SIZE, 0,
                                       (sockaddr*)&client.addr_, &client.addrLen_);
            if (recvLen > 0)
            {
                
                if(mode_ == Mode::BROADCAST)/*如果是广播模式，回复广播地址*/
                {
                    sendto(udpSocketFd_, buffer, recvLen, 0,
                           (sockaddr*)&broadcastAddr, sizeof(broadcastAddr));
                    std::cout << "Broadcasting UDP message to "
                              << BROADCAST_IP << ":"
                              << BROADCAST_PORT << " buffer:" << buffer << std::endl;
                    continue;
                }
                else
                {
                    std::cout << "Replying to UDP client: "
                              << inet_ntoa(client.addr_.sin_addr) << ":"
                              << ntohs(client.addr_.sin_port) << "buffer:" << buffer << std::endl;
                }
            }
        }
    });
    std::cout << "UDP started" << std::endl;
}
void UDP::stop()
{
    if (!running_)
    {
        return;
    }
    running_ = false;

    if(UDPThread_.joinable())
    {
        UDPThread_.join();
    }
    std::cout << "UDP stopped" << std::endl;
}

#ifndef _TCPCLIENT_H_
#define _TCPCLIENT_H_

#include "global.hpp"
#include "net.hpp"
 #include <thread>


class tcpClient : public Net
{
public:
    tcpClient();
    ~tcpClient();

    /*禁止拷贝*/
    tcpClient(const tcpClient&) = delete;
    tcpClient& operator=(const tcpClient&) = delete;

    /*连接服务器*/
    bool connect();

    /*设置服务器的IP和端口*/
    void setServer(const std::string& serverIp, unsigned short serverPort);

    /*开始网络*/
    void start() override;

    /*停止网络*/
    void stop() override;

protected:
    /*客户端线程*/
    void clientThread();

    /*接收线程*/
    void receiveThread();
private:
    int socketFd_;                     /*客户端socket描述符*/
    std::string serverIp_;             /*服务器IP*/
    unsigned short serverPort_;        /*服务器端口*/
    std::thread clientThread_;         /*客户端线程*/

    std::atomic<bool> isConnected_;      /*是否已连接到服务器*/
    
};


#endif // _TCPCLIENT_H_
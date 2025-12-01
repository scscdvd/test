#ifndef _UDP_H
#define _UDP_H_

#include "global.h"

enum class Mode
{
    NORMAL = 0,
    BROADCAST
};
class UDP : public Net
{
public:
    UDP();
    ~UDP();

    /*禁止拷贝*/
    UDP(const UDP&) = delete;
    UDP& operator=(const UDP&) = delete;

    void setMode(Mode mode);

    void setUDPPort(const std::string& udpIP,unsigned short port);
    /*初始化UDP服务器*/
    void init();

    /*开始网络*/
    void start() override;

    /*停止网络*/
    void stop() override;

private:
    std::string udpIP_;              /*UDP绑定的IP*/
    unsigned short udpPort_;        /*UDP绑定的端口*/
    Mode mode_;                     /*UDP工作模式*/

    int udpSocketFd_;              /*UDP socket描述符*/

    std::thread UDPThread_;        /*UDP线程*/

};


#endif
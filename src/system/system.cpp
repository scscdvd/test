#include "system.h"
#include "global.hpp"
#include "log.h"

System::System() : running_(false)
{
    DEBUG_PRINT << "System created" ;
    LOG_INFO <<  "System start" ;

    server_.init(variableManager::Instance().ANY_IP,variableManager::Instance().DEVICE_PORT);/*服务器监听端口*/

    client_.setServer(variableManager::Instance().SERVER_IP,variableManager::Instance().SERVER_PORT);/*设置要连接的服务器IP，端口*/

    /*正常通信UDP*/
    udp_.setUDPPort(variableManager::Instance().ANY_IP,variableManager::Instance().UDP_PORT);/*设置UDP绑定的IP和端口*/
    udp_.setMode(Mode::NORMAL);/*设置UDP工作模式*/
    udp_.init();/*初始化UDP*/

    /*广播*/
    udpBroadcast_.setUDPPort(variableManager::Instance().ANY_IP,variableManager::Instance().BROADCAST_PORT);/*设置UDP绑定的IP和端口*/
    udpBroadcast_.setMode(Mode::BROADCAST);/*设置UDP工作模式*/
    udpBroadcast_.init();/*初始化UDP*/
}
void System::startSystem() 
{
    if(running_)
    {
        return;
    }
    running_ = true;
    // 启动服务器
    server_.start();
    // 启动客户端
    client_.start();

    // 启动UDP
    udp_.start();

    // 启动广播UDP
    udpBroadcast_.start();
    /*系统运行中*/
    while(isRunning())
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void System::stopSystem()
{
    if(!running_)
    {
        return;
    }
    running_ = false;
    //停止服务器
    server_.stop();
    //停止客户端
    client_.stop();
    //停止UDP
    udp_.stop();
    //停止广播UDP
    udpBroadcast_.stop();

}

System::~System()
{
    stopSystem();
    DEBUG_PRINT << "System destroyed" ;
    LOG_WARNING << "System stop";
    if(LOG::logFile.is_open())
    {
        LOG::logFile.close();
    }
}
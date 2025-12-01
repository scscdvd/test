#include "system.h"


System::System() : running_(false)
{
    std::cout << "System created" << std::endl;
    
    server_.init("0.0.0.0",9547);/*服务器监听端口*/

    client_.setServer("10.10.1.251",9527);/*设置要连接的服务器IP，端口*/

    udp_.setUDPPort("0.0.0.0",8888);/*设置UDP绑定的IP和端口*/
    udp_.setMode(Mode::NORMAL);/*设置UDP工作模式*/
    udp_.init();/*初始化UDP*/
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
}

System::~System()
{
    stopSystem();
    std::cout << "System destroyed" << std::endl;
}
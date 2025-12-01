#include "system.h"


System::System() : server_("192.168.1.100",9547)
    , running_(false)
{
    std::cout << "System created" << std::endl;
}
void System::startSystem() 
{
    if(running_)
    {
        std::cout << "System is already running" << std::endl;
        return;
    }
    running_ = true;
    // 启动服务器
    server_.start();
}

void System::stopSystem()
{
    if(!running_)
    {
        std::cout << "System is not running" << std::endl;
        return;
    }
    running_ = false;
    //停止服务器
    server_.stop();
}

System::~System()
{
    stopSystem();
    std::cout << "System destroyed" << std::endl;
}
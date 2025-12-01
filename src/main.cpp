#include <iostream>
#include "tcpserver.h"
#include "system.h"
#include "version.h"

int main(int argc, char* argv[])
{
    std::cout << "Version: " << PROJECT_VERSION << std::endl;
    System::Instance().startSystem();
    while(System::Instance().isRunning())
    {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        std::cout << "Main system running..." << std::endl;
    }
    System::Instance().stopSystem();
    return 0;
}
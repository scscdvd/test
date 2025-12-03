#include <iostream>
#include "global.hpp"
#include "system.h"
#include "version.h"
#include "mysignal.hpp"
#include "log.h"
#include "loguru.hpp"

int main(int argc, char* argv[])
{
    LOG_F(INFO,"Version:%s",PROJECT_VERSION);
    LOG_F(INFO, "I'm hungry for some %.3f!", 3.14159);
    
    std::cout << "Version: " << PROJECT_VERSION << std::endl;
    // if(argc > 1 && std::string(argv[1]) == "-d")
    // {
    //     LOG::setDebugEnable(true);
    // }
    // MySignal::Instance().setupSignalHandlers();
    // System::Instance().startSystem();
    return 0;
}
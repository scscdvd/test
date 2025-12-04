#include <iostream>
#include "global.hpp"
#include "system.h"
#include "version.h"
#include "mysignal.hpp"
#include "log.h"

int main(int argc, char* argv[])
{
    
    LOG::init(argc,argv);

    LOG_INFO <<  "Version: " << PROJECT_VERSION;
    std::cout << "Version: " << PROJECT_VERSION << std::endl;

    if(argc > 1 && std::string(argv[1]) == "-d")
    {
        LOG::setDebugEnable(true);
    }
    MySignal::Instance().setupSignalHandlers();
    System::Instance().startSystem();
    return 0;
}
#include <iostream>
#include "tcpserver.h"
#include "system.h"
#include "version.h"
#include "mysignal.hpp"

int main(int argc, char* argv[])
{
    std::cout << "Version: " << PROJECT_VERSION << std::endl;
    MySignal::Instance().setupSignalHandlers();
    System::Instance().startSystem();
    return 0;
}
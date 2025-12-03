#include "log.h"

bool LOG::debugEnable = false;
const std::string LOG::logPath = "/root/log";
LOG::LOG()
{}

LOG::~LOG()
{
    if(debugEnable)
    {
        std::cout << oss_.str() << std::endl;
    }
    
}

void LOG::setDebugEnable(bool enable)
{
    debugEnable = enable;
}
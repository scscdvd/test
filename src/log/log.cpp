#include "log.h"

bool LOG::debugEnable = false;
const std::string LOG::logPath = "/root/log";
LOG::LOG()
{
    loguru::add_callback("test_log",LOG::myLog,NULL,true);
}

void LOG::myLog(void* user_data,const loguru::Message& message)
{
    DEBUG << "log callback";
}
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
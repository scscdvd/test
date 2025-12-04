#include "log.h"
#include <ctime>

bool LOG::debugEnable = false;
std::string LOG::logPath = ".";
std::string LOG::fileName_ = "undefined";
std::ofstream LOG::logFile;
std::string LOG::currentDate = "";
std::mutex LOG::mutex_;

LOG::LOG()
{
    
}
/*日志初始化*/
void LOG::init(int argc, char* argv[])
{ 
    /*设置日志不捕获信号*/
    loguru::Options options;
    options.signal_options.sigabrt = false;
    options.signal_options.sigbus = false;
    options.signal_options.sigfpe = false;
    options.signal_options.sigill = false;
    options.signal_options.sigint = false;
    options.signal_options.sigsegv = false;
    options.signal_options.sigterm = false;
    loguru::init(argc,argv,options);
    loguru::g_stderr_verbosity = loguru::Verbosity_OFF; // 关闭控制台输出
    loguru::remove_all_callbacks();

    loguru::add_callback("daily_file", LOG::myLog, NULL, loguru::Verbosity_MAX);
}
std::string LOG::getLevel(loguru::Verbosity ver)
{
    switch(ver)
    {
        case Verbosity_DEBUG:return "debug";break;
        case loguru::Verbosity_INFO:return "info";break;
        case loguru::Verbosity_ERROR:return "error";break;
        case loguru::Verbosity_WARNING:return "warning";break;
        default:break;
    }
    return "unknow";
}

void LOG::myLog(void* user_data, const loguru::Message& message)
{
    std::lock_guard<std::mutex> lock(mutex_);
    std::string nowData(message.preamble,10);

    if(currentDate != nowData)  /*时间不同，说明日期改变了，又过了一天*/
    {
        currentDate = nowData;  /*更新日期*/
        if(logFile.is_open())
        {
            logFile.close();
        }  
        std::string fileName = logPath + "/" + fileName_ + "-" + currentDate + ".log";
        logFile.open(fileName, std::ios::app);
    }

    std::string time_str(message.preamble, 23);
    std::string level_str = getLevel(message.verbosity);

    logFile<< "[" << time_str << "]" << " "
            << "[" << level_str << "]: "
            << message.message
            << std::endl;
    logFile.flush();
}
/*设置日志路径*/
void LOG::setLogPath(const std::string& path)
{
    std::lock_guard<std::mutex> lock(mutex_);
    logPath = path;
}
/*设置日志名*/
void LOG::setLogName(const std::string& fileName)
{
    std::lock_guard<std::mutex> lock(mutex_);
    fileName_ = fileName;
}
LOG::~LOG()
{
    if(debugEnable)
    {
        std::cout << oss_.str() << std::endl;
    }
    
}
/*是否开启打印*/
void LOG::setDebugEnable(bool enable)
{
    std::lock_guard<std::mutex> lock(mutex_);
    debugEnable = enable;
}

#if LOGURU_WITH_STREAMS

loguru::StreamLogger::~StreamLogger() noexcept(false)
{
    auto message = _ss.str();
    log(_verbosity, _file, _line, LOGURU_FMT(s), message.c_str());
}



#endif // LOGURU_WITH_STREAMS

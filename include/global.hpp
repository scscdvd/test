#ifndef _GLOBAL_HPP
#define _GLOBAL_HPP
#include "log.h"
#include <iostream> 
#include <cstring> 
#include "json.hpp"
#include "ini.h"
#include <string_view>
#include <functional>
#include <mutex>




/*自动写入iniReader_*/
template<typename T>
class ProxyVariable
{
public:
    using WriteCallback = std::function<void(const std::string&, const std::string&, const T&)>;

    ProxyVariable(const std::string& section,const std::string& key, T value)
        : section_(section), key_(key), value_(value) {}


    static void setWriteCallback(WriteCallback callback)
    {
        writeCallback_ = std::move(callback);
    }
    // 自动写回 iniReader_
    ProxyVariable& operator=(const T& v) 
    {
        std::lock_guard<std::mutex> lock(mutex_);
        value_ = v;
        if(writeCallback_)
        {
            writeCallback_(section_, key_, value_);
        }
        return *this;
    }

    // 隐式转换，获取值
    operator T() const 
    {
        return value_;
    }

private:
    std::string section_;
    std::string key_;
    T value_;
    std::mutex mutex_;
    
    static WriteCallback writeCallback_;


};

template<typename T>
typename ProxyVariable<T>::WriteCallback ProxyVariable<T>::writeCallback_ = NULL;

 /*变量管理*/
class variableManager
{
public:
    static variableManager& Instance() noexcept
    {
        static variableManager instance;
        return instance;
    }

    variableManager(const variableManager&) = delete;
    variableManager& operator=(const variableManager&) = delete;
    


public:

    void updateIniFile()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        iniWriter_.write(std::string(iniFilePath), iniReader_);
    }

    
public:
    /*可变参数*/
    ProxyVariable<unsigned short> DEVICE_PORT = ProxyVariable<unsigned short>("NET", "DevicePort", 9547);      /*TCP服务器监听端口*/
    ProxyVariable<unsigned short> SERVER_PORT = ProxyVariable<unsigned short>("NET", "ServerPort", 9527);       /*要连接的服务器的端口*/
    ProxyVariable<unsigned short> UDP_PORT = ProxyVariable<unsigned short>("NET", "UDPPort", 8001);          /*UDP端口*/
    ProxyVariable<std::string> SERVER_IP = ProxyVariable<std::string>("NET", "ServerIP", "10.10.1.251");          /*要连接的服务器的IP地址*/

    

    
public:
    /*不可变参数*/
    static constexpr int MAX_LISTEN  = 10;          /*最大监听数量*/
    static constexpr int MAX_EVENTS  = 100;        /*epoll最大事件数*/
    static constexpr int BUFFER_SIZE = 2048;       /*缓冲区大小*/

    static constexpr unsigned short BROADCAST_PORT = 8888;     /*广播端口*/
    static constexpr const char* BROADCAST_IP = "255.255.255.255";   /*广播地址*/

    static constexpr const char* ANY_IP = "0.0.0.0";                /*任意IP*/
    static constexpr std::string_view iniFilePath = "./setting.ini";     /*配置文件路径*/
private:
        /*加载配置文件*/
    void load()
    {

        UDP_PORT = iniReader_.Get<unsigned short>("NET", "UDPPort");
        SERVER_IP = iniReader_.Get<std::string>("NET", "ServerIP");
        DEVICE_PORT = iniReader_.Get<unsigned short>("NET", "DevicePort");
        SERVER_PORT = iniReader_.Get<unsigned short>("NET", "ServerPort");

    }

    /*设置回调并加载配置文件*/
    variableManager() : iniReader_(std::string(iniFilePath))
    {
        // 设置 ProxyVariable 回调
        ProxyVariable<unsigned short>::setWriteCallback([this](const std::string& section, const std::string& key, const unsigned short& value)
        {
            iniReader_.UpdateEntry(section, key, value);
        });
        ProxyVariable<std::string>::setWriteCallback([this](const std::string& section, const std::string& key,const std::string& value)
        {
            iniReader_.UpdateEntry(section, key, value);
        });

        load();
        LOG_INFO << "ini file load success";

        
    }
    ~variableManager() = default;
    
private:
    inih::INIReader iniReader_; /*读取配置文件*/
    inih::INIWriter iniWriter_; /*写入配置文件*/

    std::mutex mutex_;
 };



#endif //_GLOBAL_H

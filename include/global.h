#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <iostream> 
#include <thread> 
#include "net.hpp"
 #include <fstream> 
 #include <unistd.h> 
 #include <cstdlib> 
 #include <map> 
 #include <sys/epoll.h> 
 #include <fcntl.h> 
 #include <cstring> 
 #include <sys/eventfd.h> 
 #include <arpa/inet.h>

/*********************** 常量，不可改变 *******************************/

extern const int MAX_LISTEN;
extern const int MAX_EVENTS;
extern const int BUFFER_SIZE;

extern const unsigned short BROADCAST_PORT;
extern const char* BROADCAST_IP;

extern const char* ANY_IP;

/******************************* 后续可配置参数（可修改） *******************************/

extern unsigned short TCP_SERVER_PORT;
extern unsigned short SERVER_PORT;
extern unsigned short UDP_PORT;

extern std::string SERVER_IP;

#endif //_GLOBAL_H

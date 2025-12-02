#include "global.h"

/*********************** 常量，不可改变 *******************************/
const int MAX_LISTEN  = 5;          /*最大监听数量*/
const int MAX_EVENTS  = 100;        /*epoll最大事件数*/
const int BUFFER_SIZE = 1024;       /*缓冲区大小*/

const unsigned short BROADCAST_PORT = 5555;     /*广播端口*/
const char* BROADCAST_IP = "255.255.255.255";   /*广播地址*/

const char* ANY_IP = "0.0.0.0";                /*任意IP*/

/******************************* 后续可配置参数 *******************************/
unsigned short TCP_SERVER_PORT = 9547;      /*TCP服务器监听端口*/
unsigned short SERVER_PORT     = 9527;      /*要连接的服务器端口*/
unsigned short UDP_PORT        = 8001;      /*UDP端口*/

std::string SERVER_IP = "10.10.1.251";      /*要连接的服务器IP*/

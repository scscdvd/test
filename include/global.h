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

const int MAX_LISTEN = 5;
const int MAX_EVENTS = 100;
const int BUFFER_SIZE = 1024;

#endif //_GLOBAL_H
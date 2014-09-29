#ifndef LOG_MODULE
#error Please define LOG_MODULE (char[])
#endif

#ifdef WIN32
#include "sys_helper.h"

#define LOG_TRC(data)       SYS_LOG_TRC(LOG_MODULE, data)
#define LOG_DBG(data)       SYS_LOG_DBG(LOG_MODULE, data)
#define LOG_INF(data)       SYS_LOG_INF(LOG_MODULE, data)
#define LOG_WRN(data)       SYS_LOG_WRN(LOG_MODULE, data)
#define LOG_ERR(data)       SYS_LOG_ERR(LOG_MODULE, data)

#else
#include <iostream>
#define LOG_TRC(data)       std::cout <<"["<< getTime() <<"]" <<"["<< LOG_MODULE <<"]"<<" [TRC] "<< data << std::endl 
#define LOG_DBG(data)       std::cout <<"["<< getTime() <<"]" <<"["<< LOG_MODULE <<"]"<<" [DBG] "<< data << std::endl 
#define LOG_INF(data)       std::cout <<"["<< getTime() <<"]" <<"["<< LOG_MODULE <<"]"<<" [INF] "<< data << std::endl 
#define LOG_WRN(data)       std::cout <<"["<< getTime() <<"]" <<"["<< LOG_MODULE <<"]"<<" [WRN] "<< data << std::endl 
#define LOG_ERR(data)       std::cout <<"["<< getTime() <<"]" <<"["<< LOG_MODULE <<"]"<<" [ERR] "<< data << std::endl 

#endif
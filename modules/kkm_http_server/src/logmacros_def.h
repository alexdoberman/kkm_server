#ifndef LOG_MODULE
#error Please define LOG_MODULE (char[])
#endif
#include <ctime>
inline std::string getTime()
{
	std::time_t time = std::time(NULL);
    char mbstr[100];
    if (std::strftime(mbstr, sizeof(mbstr), "%c", std::localtime(&time))) 
	{
        return mbstr;
    }
	return "";
}
#ifdef WIN32
#include <iostream>
#define LOG_TRC(data)       std::cout <<"["<< getTime() <<"]" <<"["<< LOG_MODULE <<"]"<<" [TRC] "<< data << std::endl 
#define LOG_DBG(data)       std::cout <<"["<< getTime() <<"]" <<"["<< LOG_MODULE <<"]"<<" [DBG] "<< data << std::endl 
#define LOG_INF(data)       std::cout <<"["<< getTime() <<"]" <<"["<< LOG_MODULE <<"]"<<" [INF] "<< data << std::endl 
#define LOG_WRN(data)       std::cout <<"["<< getTime() <<"]" <<"["<< LOG_MODULE <<"]"<<" [WRN] "<< data << std::endl 
#define LOG_ERR(data)       std::cout <<"["<< getTime() <<"]" <<"["<< LOG_MODULE <<"]"<<" [ERR] "<< data << std::endl 

#else
#include <iostream>
#define LOG_TRC(data)       std::cout <<"["<< getTime() <<"]" <<"["<< LOG_MODULE <<"]"<<" [TRC] "<< data << std::endl 
#define LOG_DBG(data)       std::cout <<"["<< getTime() <<"]" <<"["<< LOG_MODULE <<"]"<<" [DBG] "<< data << std::endl 
#define LOG_INF(data)       std::cout <<"["<< getTime() <<"]" <<"["<< LOG_MODULE <<"]"<<" [INF] "<< data << std::endl 
#define LOG_WRN(data)       std::cout <<"["<< getTime() <<"]" <<"["<< LOG_MODULE <<"]"<<" [WRN] "<< data << std::endl 
#define LOG_ERR(data)       std::cout <<"["<< getTime() <<"]" <<"["<< LOG_MODULE <<"]"<<" [ERR] "<< data << std::endl 

#endif
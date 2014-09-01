#pragma once
#include <istream>
#include <ostream>
#include <map>
#include <memory>

#include <Poco/Mutex.h>

#include "config.h"

typedef Poco::Mutex TMutex;
class CRequestProcessor
{
public:
    explicit CRequestProcessor(TKKMConfig cfg);
    virtual ~CRequestProcessor();
public:
    void process(std::istream & ssIn, std::ostream& ssOut);
	TMutex& get_lock(std::string sPortId);
private:
	
	std::map <std::string , std::unique_ptr<TMutex> > m_mapMtx;
	TMutex                          m_mtx;        
	TKKMConfig                      m_cfg;
};
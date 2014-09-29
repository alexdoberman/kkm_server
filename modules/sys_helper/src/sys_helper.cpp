#include "sys_helper.h"

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace keywords = boost::log::keywords;


class CLog: public ILog
{

public:
	static  CLog& instance()
    {
        static CLog theSingleInstance;
        return theSingleInstance;
    }
	virtual void draw(std::string sModule, ELogLevel kLevel, std::string sMsg) 
	{
      switch ( kLevel )
      {
         case LOGLEV_ERROR:
			 BOOST_LOG_TRIVIAL(error)   <<"["<<sModule<<"] "<<" [ERR] "<< sMsg  ;
            break;
         case LOGLEV_WARNING:
			 BOOST_LOG_TRIVIAL(warning) <<"["<<sModule<<"] "  <<" [WRN] "<< sMsg  ;
            break;
         case LOGLEV_INFO:
			 BOOST_LOG_TRIVIAL(info)   <<"["<<sModule<<"] "<<" [INF] "<< sMsg  ;
            break;
         case LOGLEV_DEBUG:
			 BOOST_LOG_TRIVIAL(debug)   <<"["<<sModule<<"] "<<" [DBG] "<< sMsg  ;
            break;
         case LOGLEV_TRACE:
			 BOOST_LOG_TRIVIAL(trace)   <<"["<<sModule<<"] "<<" [TRC] "<< sMsg  ;
            break;
      }
	}
private:
	CLog()
	{
		logging::add_file_log
		(
			keywords::file_name = "log/log_%N.log",                                        /*< file name pattern >*/
			keywords::rotation_size = 10 * 1024 * 1024,                                   /*< rotate files every 10 MiB... >*/
			keywords::format = "[%TimeStamp%] [%ThreadID%] : %Message%" ,
			keywords::auto_flush = true 
		);
	
		logging::core::get()->set_filter
		(
			logging::trivial::severity >= logging::trivial::trace
		);

		logging::add_common_attributes();
	}
	CLog(const CLog& root);
    CLog& operator=(const CLog&);
};


SYS_HELPER_LIB void createLogger(ILog** ppLog)
{
	assert(ppLog);
	*ppLog = &(CLog::instance());
}
#pragma once
#if defined(_WIN32) || defined(_WIN32_)
	#if defined(SYS_HELPER_EXPORTS)
		#define SYS_HELPER_LIB __declspec(dllexport) 
	#else
		#define SYS_HELPER_LIB __declspec(dllimport)
	#endif /* KKM_DRIVER_EXPORTS */ 
#elif defined(linux) || defined(_linux)
	#define SYS_HELPER_LIB
#endif 

#include <stdint.h>
#include <string>
#include <sstream>
#include <memory>

enum ELogLevel
{
    LOGLEV_OFF     = 0,           /* system is unusable */
    LOGLEV_FATAL   = 1,           /* action must be taken immediately */
    LOGLEV_CRIT    = 2,           /* critical conditions */
    LOGLEV_ERROR   = 3,           /* error conditions */
    LOGLEV_WARNING = 4,             /* warning conditions */
    LOGLEV_INFO    = 5,            /* informational */
    LOGLEV_DEBUG   = 6,             /* debug-level messages */
    LOGLEV_TRACE   = 7              /* trace-level messages */
};

class ILog
{
public:
	virtual void draw(std::string sModule, ELogLevel kLevel, std::string sMsg)  = 0;
};

SYS_HELPER_LIB void createLogger(ILog**);

class CLogStream : public std::ostringstream
{
public:
    typedef std::ostringstream	TOutStream;

public:
    CLogStream( ILog* pLogger,const std::string  sModule, ELogLevel level ) 
        : m_pLogger(pLogger)
        , m_msgLevel( level )
        , m_module( sModule )
        { }

        ~CLogStream( )
		{ this->flush(); }

public:

    void			flush( void ) 
    { 		
		if ( m_pLogger )
			m_pLogger->draw(m_module, m_msgLevel, this->str( ).c_str());
	}

private:
    CLogStream(const CLogStream&);
    CLogStream& operator =(const CLogStream&);

private:
    ILog*                   m_pLogger;
    ELogLevel		        m_msgLevel;
    std::string             m_module;
};


#define SYS_LOG_0( module, level, data ) {                                    \
        ILog*	pLogger = nullptr;                           \
        createLogger( &pLogger ) ;            \
        CLogStream(pLogger, module, level)<< data  ;                            \
        }

#define SYS_LOG_TRC(module, data) SYS_LOG_0( module, LOGLEV_TRACE, data )
#define SYS_LOG_DBG(module, data) SYS_LOG_0( module, LOGLEV_DEBUG, data )
#define SYS_LOG_INF(module, data) SYS_LOG_0( module, LOGLEV_INFO,  data )
#define SYS_LOG_WRN(module, data) SYS_LOG_0( module, LOGLEV_WARNING, data )
#define SYS_LOG_ERR(module, data) SYS_LOG_0( module, LOGLEV_ERROR, data )



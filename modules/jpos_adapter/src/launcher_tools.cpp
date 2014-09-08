#include "launcher_tools.h"
#include "jposadapter_def.h"

#include <assert.h>

#include <boost/filesystem.hpp>
#include <boost/process.hpp>

#ifdef WIN32
#include <windows.h>
#endif

#define BOOST_PROCESS_WINDOWS_USE_NAMED_PIPE 
namespace launcher_tools
{

TResult launch_pos_tool(const std::string& sCmd, int& resultCode , std::vector<std::string> &vsStdOut)
{
	TResult nRet = kJPOSResult_Success;
    do
    {
		LOG_INF("launch cmd : '" <<sCmd<< "'");
       
#ifdef WIN32
        boost::process::context  ctx; 

        ctx.stdin_behavior  = boost::process::close_stream();
        ctx.stderr_behavior = boost::process::redirect_stream_to_stdout();  
        ctx.stdout_behavior = boost::process::capture_stream();
        ctx.environment     = boost::process::self::get_environment(); 
		

		boost::process::child childProcess = boost::process::launch_shell(sCmd, ctx);
        boost::process::pistream &is = childProcess.get_stdout();
#else

        boost::process::context ctx; 
        ctx.stdin_behavior  = boost::process::close_stream();
        ctx.stderr_behavior = boost::process::close_stream();
        ctx.stdout_behavior = boost::process::capture_stream(); 

        ctx.environment     = boost::process::self::get_environment(); 

        boost::process::child childProcess = boost::process::launch(sPath, vsArgs, ctx); 
        boost::process::pistream &is = childProcess.get_stdout(); 
#endif

        std::string sLine;
        while (std::getline(is, sLine)) 
			vsStdOut.push_back(sLine);

        boost::process::status statusProcessExit = childProcess.wait(); 
        if (statusProcessExit.exited())
        {
            resultCode = statusProcessExit.exit_status();
            LOG_INF("exit code = " <<resultCode);
        }
        else
        {
			LOG_ERR("Error, wait cmd: '" <<sCmd<<"'");
            nRet =  kJPOSResult_Fail;
            break;
        }
    }
    while (false);
    return nRet;
}


} //launcher_tools

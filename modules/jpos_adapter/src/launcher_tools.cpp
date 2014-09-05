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

TResult launch_pos_tool(const std::string& sPath, const std::vector<std::string> & vsArgs, int& resultCode , std::string &sStdOut)
{
	TResult nRet = kJPOSResult_Success;
    do
    {
		LOG_INF("launch '" <<sPath<< "' with parameters:");
        std::for_each(vsArgs.begin() , vsArgs.end() , [](const std::string & s){ LOG_INF(" "<<s); });
        
#ifdef WIN32
        //STARTUPINFOA si; 
        //::ZeroMemory(&si, sizeof(si)); 
        //si.cb = sizeof(si); 
        //si.dwFlags |= STARTF_USESHOWWINDOW;
        //si.wShowWindow = SW_HIDE;

        boost::process::context  ctx; 
//        ctx.startupinfo = &si; 

        ctx.stdin_behavior  = boost::process::close_stream();
        ctx.stderr_behavior = boost::process::close_stream();  
        ctx.stdout_behavior = boost::process::capture_stream();
        ctx.environment     = boost::process::self::get_environment(); 

        //boost::process::child childProcess = boost::process::win32_launch(sPath, vsArgs, ctx); 
		
		std::string sCmd = sPath;
		sCmd.append(" ");
		std::for_each(vsArgs.begin() , vsArgs.end() , [&sCmd](const std::string & s){ sCmd.append(s); sCmd.append(" "); });

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
        {
            sStdOut.append(sLine);
        }

        boost::process::status statusProcessExit = childProcess.wait(); 
        if (statusProcessExit.exited())
        {
            resultCode = statusProcessExit.exit_status();
            LOG_INF("exit code = " <<resultCode);
        }
        else
        {
			LOG_ERR("Error, wait process  " <<sPath);
            nRet =  kJPOSResult_Fail;
            break;
        }
    }
    while (false);
    return nRet;
}


} //launcher_tools

#include "jposadapter.h"

#include <algorithm>

#include "launcher_tools.h"
#include "jposadapter_def.h"

JPOSADAPTER_DLL TResult jpos_print(const std::string & sDevName, const std::vector<std::string>& vLines, TJPOSResult& ret)
{
    TResult nRet = kJPOSResult_Success;
	try
    {
        do
        {
            //prepare arguments
            std::vector <std::string> args; 

            args.push_back("dummy");
            args.push_back(sDevName);
			std::for_each(vLines.begin() , vLines.end() , [&args](const std::string & s){ args.push_back(s); });

			std::string sPath = "D:\\Dropbox\\svn\\java_test_2\\pos_print\\run2.bat";
			//read ansver
			int nRetCode = 0;
			std::string sStdOut;
			if ((nRet = launcher_tools::launch_pos_tool(sPath, args, nRetCode, sStdOut)) != kJPOSResult_Success)
            {
				LOG_ERR("error when launch pos.jar");
				nRet = kJPOSResult_Fail;
                break;
            }
		
			LOG_DBG("-----------------------------------------------------------------------");
			LOG_DBG(sStdOut);
			LOG_DBG("-----------------------------------------------------------------------");
			//parse error todo
			std::size_t nFound = sStdOut.find("Exception:");

			if (nFound != std::string::npos )
			{
				ret.nErr   = 1;
				ret.nErrEx = 0;
				ret.sDesk  = "fail";
			}
        }
        while (false);
    }
    catch(const std::exception & ex)
    {
		LOG_ERR("launch pos.jar exception , what = "<<ex.what());
        nRet = kJPOSResult_Fail;
    }
    catch(...)
    {
		LOG_ERR("launch pos.jar unknown exception");
        nRet = kJPOSResult_Fail;
    }
    return nRet;
}

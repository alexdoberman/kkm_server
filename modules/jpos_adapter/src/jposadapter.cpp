#include "jposadapter.h"

#include <algorithm>
#include <regex>

#include <boost/lexical_cast.hpp>

#include <base64.h>

#include "launcher_tools.h"
#include "jposadapter_def.h"

JPOSADAPTER_DLL TResult jpos_print(const std::string & sDevName, const std::vector<std::string>& vLines, TJPOSResult& ret)
{
    TResult nRet = kResult_Success;
	try
    {
			//clear out
			ret.nErr   = 0;
			ret.nErrEx = 0;
			ret.sDesk  = "";

        do
        {
			//HACK
			std::string sCmd ="run_jpos.bat ";
			if (sDevName.find("http://") != std::string::npos)
				sCmd = "run_net_send.bat ";

			
			sCmd.append(sDevName);

			std::for_each(vLines.begin() , vLines.end() , [&sCmd](const std::string & s)
				{
					sCmd.append(" "); 
					sCmd.append( base64_encode((const unsigned char*) s.c_str(), s.length())); 
			});

			//read ansver
			int nRetCode = 0;
			std::vector<std::string> vsStdOut;
			if ((nRet = launcher_tools::launch_pos_tool(sCmd, nRetCode, vsStdOut)) != kResult_Success)
            {
				LOG_ERR("error when launch cmd: '"<<sCmd<<"'");
				nRet = kResult_Fail;
                break;
            }
		
			LOG_DBG("----ANSVER----");
			for (size_t i = 0; i< vsStdOut.size(); i++)
			{
				std::string  sLine = vsStdOut[i];
				LOG_DBG(sLine);

				//PosPrinter.Exception: err = 104, errEx = 0, desc = Could not find service
				std::size_t nFound = sLine.find("PosPrinter.Exception:");
				if (nFound != std::string::npos )
				{
					std::smatch sm;
					std::regex e ("^PosPrinter.Exception: err = (\\d+), errEx = (\\d+), desc = (.*)\r");
					std::regex_match (sLine, sm, e);

					if (sm.size() == 4)
					{
						try 
						{
							ret.nErr   = boost::lexical_cast<int32_t>( sm[1] );
							ret.nErrEx = boost::lexical_cast<int32_t>( sm[2] );
							ret.sDesk  = sm[3];
						} 
						catch( boost::bad_lexical_cast const& ) 
						{
							LOG_ERR("Bad cast.");
							nRet = kResult_Fail;
							break;
						}
					}
					else
					{
						LOG_ERR("Bad ansver.");
						nRet = kResult_Fail;
						break;
					}
					break;
				}
			}
			LOG_DBG("--------------");
        }
        while (false);
    }
    catch(const std::exception & ex)
    {
		LOG_ERR("launch exception , what = "<<ex.what());
        nRet = kResult_Fail;
    }
    catch(...)
    {
		LOG_ERR("launch unknown exception");
        nRet = kResult_Fail;
    }
    return nRet;
}

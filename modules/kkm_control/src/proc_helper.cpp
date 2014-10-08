#include "proc_helper.h"


#include <Psapi.h>
#include <Shlwapi.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <StrSafe.h>

namespace proc_helper
{
	DWORD find(const wchar_t * szApp)
	{
	    HANDLE hProcessSnap;
		PROCESSENTRY32 pe32;

		// Take a snapshot of all processes in the system.
		hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
		if( hProcessSnap == INVALID_HANDLE_VALUE )
		{
			//printError( TEXT("CreateToolhelp32Snapshot (of processes)") );
			return( 0 );
		}

		// Set the size of the structure before using it.
		pe32.dwSize = sizeof( PROCESSENTRY32 );

		// Retrieve information about the first process,
		// and exit if unsuccessful
		if( !Process32First( hProcessSnap, &pe32 ) )
		{
			//printError( TEXT("Process32First") ); // show cause of failure
			CloseHandle( hProcessSnap );          // clean the snapshot object
			return( 0 );
		}

		DWORD pid = 0;

		// Now walk the snapshot of processes, and
		// display information about each process in turn
		do
		{  

			if ( StrStrI( pe32.szExeFile, szApp ) != NULL )
			{
				pid = pe32.th32ProcessID;
				break;
			}


		} while( Process32Next( hProcessSnap, &pe32 ) );

		CloseHandle( hProcessSnap );
		return( pid );
	
	}
}

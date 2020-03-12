/* ========================================================================
	Title        : QuickUSBDriverSetup.cpp
	Description  : Defines the entry point for the console application
	Notes        : 
	History      :

	Copyright © 2012 Bitwise Systems.  All rights reserved.
	This software contains confidential information and trade secrets of
	Bitwise Systems and is protected by United States and international
	copyright laws.  Use, disclosure, or reproduction is prohibited
	without the prior express written permission of Bitwise Systems,
	except as agreed in the QuickUSB license agreement.

	Use, duplication or disclosure by the U.S. Government is subject to
	restrictions as provided in DFARS 227.7202-1(a) and 227.7202-3(a)
	(1998), and FAR 12.212, as applicable.  Bitwise Systems, 6489 Calle
	Real, Suite E, Goleta, CA  93117.

	Bitwise Systems
	6489 Calle Real, Suite E
	Santa Barbara, CA  93117
	Voice: (805) 683-6469
	Fax  : (805) 683-4833
	Web  : www.bitwisesys.com
	email: support@bitwisesys.com
   ======================================================================== */
#include "stdafx.h"

//BOOL IsWow64( void );
void PrintLastError( const WCHAR* apiname );
BOOL get_module_directory(TCHAR *obuf, size_t osize);
DWORD Install_vcredist(HINSTANCE hInstance, BOOL x64);
bool FileExists(WCHAR *filename);

// DLL entry points
typedef UINT (WINAPI* GETSYSTEMWOW64DIRECTORY)(LPTSTR, UINT);
typedef UINT (WINAPI* WOW64DISABLEWOW64FSREDIRECTION)(PVOID *);
typedef UINT (WINAPI* WOW64REVERTWOW64FSREDIRECTION)(PVOID);

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	BOOL isX64, isInstall, retVal, noVCredist, onlyVCredist;
    WCHAR *X86 = TEXT("dpinstx86.exe");
	WCHAR *X64 = TEXT("dpinstx64.exe");

    WCHAR *AppName = NULL;
	WCHAR dir[_MAX_PATH];

	UINT winPathLen;
	WCHAR winPath[_MAX_PATH];
	WCHAR win32Path[_MAX_PATH], win64Path[_MAX_PATH];
	WCHAR dll32Path[_MAX_PATH], dll64Path[_MAX_PATH];
    WCHAR wow64Path[MAX_PATH];

    SHELLEXECUTEINFO shellExeInfo;
    DWORD exitCode;
	BOOL doNotReplace = FALSE;

	HMODULE hKernel32;
    GETSYSTEMWOW64DIRECTORY getSystemWow64Directory;
	WOW64DISABLEWOW64FSREDIRECTION wow64DisableWow64FsRedirection;
	WOW64REVERTWOW64FSREDIRECTION wow64RevertWow64FsRedirection;

	int len, k;

	// Get proc addresses for 64-bit only functions
	hKernel32 = GetModuleHandle(TEXT("kernel32.dll")); // There is no corresponding free function to call
	if (hKernel32 != NULL)
	{
		getSystemWow64Directory = (GETSYSTEMWOW64DIRECTORY) GetProcAddress(hKernel32, "GetSystemWow64DirectoryW");
		wow64DisableWow64FsRedirection = (WOW64DISABLEWOW64FSREDIRECTION) GetProcAddress(hKernel32, "Wow64DisableWow64FsRedirection");
		wow64RevertWow64FsRedirection  = (WOW64REVERTWOW64FSREDIRECTION) GetProcAddress(hKernel32, "Wow64RevertWow64FsRedirection");
	}

	// Determine if x86 or x64
	isX64 = TRUE;
	if (hKernel32 == NULL || getSystemWow64Directory == NULL || (getSystemWow64Directory(wow64Path, _MAX_PATH) == 0 && GetLastError() == ERROR_CALL_NOT_IMPLEMENTED))
	{
		isX64 = FALSE;
	}

	// Get the directory of the running exe and use it as the root path
	if (!get_module_directory(dir, _MAX_PATH))     
    {   
		_wgetcwd(dir, _MAX_PATH);
    }


	// Are we installing or uninstalling?
	isInstall = TRUE;
	len = (int)wcslen(lpCmdLine);
	for (k=0; k < (len-1); ++k)
	{
		if (lpCmdLine[k] == '/' && (lpCmdLine[k+1] == 'u' || lpCmdLine[k+1] == 'U'))
		{
			isInstall = FALSE;
			break;
		}
	}


	// Check if we should not install vcredist or only install vcredist (This must be the last parameter)
    // /0 - Do not install VCRedist
    // /1 - Only install VCRedist
	noVCredist = FALSE;
    onlyVCredist = FALSE;
	len = (int)wcslen(lpCmdLine);
	for (k=0; k < (len-1); ++k)
	{
		if (lpCmdLine[k] == '/' && (lpCmdLine[k+1] == '0'))
		{
            // We cannot pass this command-line parameter to DPInst, so we must truncate the parameter list
            lpCmdLine[k] = 0;
            lpCmdLine[k+1] = 0;
			noVCredist = TRUE;
			break;
		}

		if (lpCmdLine[k] == '/' && (lpCmdLine[k+1] == '1'))
		{
			onlyVCredist = TRUE;
			break;
		}
	}


	/***********************************
	  Start DPInst
	***********************************/
    if (!onlyVCredist)
    {
	    // Fill out execute information
	    ZeroMemory(&shellExeInfo, sizeof(SHELLEXECUTEINFO));
	    shellExeInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	    shellExeInfo.lpDirectory = dir;
	    shellExeInfo.lpFile = isX64 ? X64 : X86;
	    if (wcscmp(lpCmdLine, TEXT("")) == 0)
	    {
		    // If no command-line args are passed, automatically pass \EL which
		    // permits the driver to install on non-English PCs.  Also pass the
		    // \P switch which will display a prompt to the user if the driver
		    // being installed is not the best match and ask to overwrite it
		    // anyways.
		    shellExeInfo.lpParameters = TEXT("/EL /P");
	    }
	    else
	    {
		    // If any command-line parameters are passed, then none of the default
		    // command-line switches (above) get passed!
		    shellExeInfo.lpParameters = lpCmdLine;
	    }
	    shellExeInfo.nShow = SW_SHOW;
	    shellExeInfo.hInstApp = hInstance;
	    shellExeInfo.fMask = SEE_MASK_NOCLOSEPROCESS;

	    // Execute DPInst driver installer
	    ShellExecuteEx(&shellExeInfo);

	    // Check for execution error
	    if ((int)shellExeInfo.hInstApp <= 32)
	    {
		    return (int)shellExeInfo.hInstApp;
	    }
    }


	/***********************************
	  Install vcredist (in background)
	***********************************/
	// We should never uninstall vcredist, only install
	if (isInstall && !noVCredist)
	{
		// Always install the x86 version of vcredist
		exitCode = Install_vcredist(hInstance, false);
		if (exitCode)
		{
			return exitCode;
		}

		// For X64 machines, install the x64 version of vcredit as well
		if (isX64)
		{
			exitCode = Install_vcredist(hInstance, true);
			if (exitCode)
			{
				return exitCode;
			}
		}
	}


	/***********************************
	  Wait for DPInst to complete
	***********************************/
    if (!onlyVCredist)
    {
	    // Wait for the process to die
	    WaitForSingleObject(shellExeInfo.hProcess, INFINITE);

	    // Get the exit code
        GetExitCodeProcess(shellExeInfo.hProcess, &exitCode);

	    // Close the handles
	    CloseHandle(shellExeInfo.hProcess);

	    // If the driver could not be installed, don't copy the DLLs
	    if (exitCode & 0x80000000)
	    {
		    return exitCode;
	    }

	    // If the driver was copied to the DriverStore, but not installed on a
	    // device, don't copy the DLLs.  This could occur if another driver is
	    // in use or if the driver was installed without a module plugged into
	    // the computer.  To be safe, copy the DLLs over but do not replace
	    // any existing DLLs.
        //
        // BUGFIX: This actually introduces more problems then it fixes.  Leave
        //         it commented out.
	    //if (exitCode & 0x0000FF00)
	    //{
	    //	doNotReplace = TRUE;
	    //}


	    /***********************************
	      Install DLLs
	    ***********************************/
	    if (isInstall)
	    {
		    // Copy/Remove DLL files - Do this because the copy commands in the driver INF
		    // are not executed until they are loaded (ie A module is plugged in). 
		    // This is a problem when someone installs the library and runs diag/prog
		    // before plugging in a module.  The DLL is not present so they get an error

		    winPathLen = GetWindowsDirectory(winPath, _MAX_PATH);
		    if (isX64)
		    {
			    // Construct QuickUSB DLL paths
			    wcscpy_s(win32Path, _MAX_PATH, wow64Path);
			    wcscat_s(win32Path, _MAX_PATH, TEXT("\\QuickUSB.dll"));

			    wcscpy_s(win64Path, _MAX_PATH, winPath);
			    wcscat_s(win64Path, _MAX_PATH, TEXT("\\System32\\QuickUSB.dll"));

			    wcscpy_s(dll32Path, _MAX_PATH, dir);
			    wcscat_s(dll32Path, _MAX_PATH, TEXT("\\i386\\QuickUSB.dll"));

			    wcscpy_s(dll64Path, _MAX_PATH, dir);
			    wcscat_s(dll64Path, TEXT("\\amd64\\QuickUSBamd64.dll"));

			    // Copy 32-bit DLL
			    retVal = CopyFile(dll32Path, win32Path, doNotReplace);

			    // Copy 64-bit DLL (Note that a 32-bit must gile disable WOW64 redirection
			    // otherwise our copy will be redirected to SysWOW64 instead of System32)
			    if (wow64DisableWow64FsRedirection != NULL && wow64RevertWow64FsRedirection != NULL)
			    {
				    PVOID OldValue = NULL;
				    if(wow64DisableWow64FsRedirection(&OldValue))
				    {
					    retVal = CopyFile(dll64Path, win64Path, doNotReplace);

					    if (wow64RevertWow64FsRedirection(OldValue) == FALSE)
					    {
						    //  Failure to re-enable redirection should be considered
						    //  a criticial failure and execution aborted.
						    return 1;
					    }
				    }
			    }
                
			    // Construct QuickUSB Utility DLL paths
			    wcscpy_s(win32Path, _MAX_PATH, wow64Path);
			    wcscat_s(win32Path, _MAX_PATH, TEXT("\\QuickUsbUtil.dll"));

			    wcscpy_s(win64Path, _MAX_PATH, winPath);
			    wcscat_s(win64Path, _MAX_PATH, TEXT("\\System32\\QuickUsbUtil.dll"));

			    wcscpy_s(dll32Path, _MAX_PATH, dir);
			    wcscat_s(dll32Path, _MAX_PATH, TEXT("\\i386\\QuickUsbUtil.dll"));

			    wcscpy_s(dll64Path, _MAX_PATH, dir);
			    wcscat_s(dll64Path, TEXT("\\amd64\\QuickUsbUtilamd64.dll"));

			    // Copy 32-bit Utility DLL
			    retVal = CopyFile(dll32Path, win32Path, doNotReplace);

			    // Copy 64-bit Utility DLL (Note that a 32-bit file must disable WOW64 redirection
			    // otherwise our copy will be redirected to SysWOW64 instead of System32)
			    if (wow64DisableWow64FsRedirection != NULL && wow64RevertWow64FsRedirection != NULL)
			    {
				    PVOID OldValue = NULL;
				    if(wow64DisableWow64FsRedirection(&OldValue))
				    {
					    retVal = CopyFile(dll64Path, win64Path, doNotReplace);

					    if (wow64RevertWow64FsRedirection(OldValue) == FALSE)
					    {
						    //  Failure to re-enable redirection should be considered
						    //  a criticial failure and execution aborted.
						    return 1;
					    }
				    }
			    }
		    }
		    else
		    {
			    // Copy 32-bit QuickUSB.dll file
			    wcscpy_s(win32Path, _MAX_PATH, winPath);
			    wcscat_s(win32Path, _MAX_PATH, TEXT("\\System32\\QuickUSB.dll"));

			    wcscpy_s(dll32Path, _MAX_PATH, dir);
			    wcscat_s(dll32Path, _MAX_PATH, TEXT("\\i386\\QuickUSB.dll"));
    			
			    retVal = CopyFile(dll32Path, win32Path, doNotReplace);

			    // Copy 32-bit QuickUsbUtil.dll file
			    wcscpy_s(win32Path, _MAX_PATH, winPath);
			    wcscat_s(win32Path, _MAX_PATH, TEXT("\\System32\\QuickUsbUtil.dll"));

			    wcscpy_s(dll32Path, _MAX_PATH, dir);
			    wcscat_s(dll32Path, _MAX_PATH, TEXT("\\i386\\QuickUsbUtil.dll"));
    			
			    retVal = CopyFile(dll32Path, win32Path, doNotReplace);
		    }
	    }
        else 
        {
            // Never uninstall DLLs
        }
    }


	/***********************************
	  Done
	***********************************/
    return 0;
}



BOOL get_module_directory(TCHAR *obuf, size_t osize)   
{   
	if (!GetModuleFileName(0, obuf, osize) )   
	{   
		*obuf = '\0';
		return FALSE;   
    }   
    
	// Run through looking for the *last* slash in this path.   
	// if we find it, NUL it out to truncate the following   
	// filename part.   
	TCHAR *lastslash = 0;
	for (; *obuf; obuf++)   
	{   
		if (*obuf == '\\' || *obuf == '/')
		{
			lastslash = obuf;
		}
	}   

	if (lastslash)
	{
		*lastslash = '\0';
	}

	return TRUE;   
}



DWORD Install_vcredist(HINSTANCE hInstance, BOOL x64)
{
    SHELLEXECUTEINFO shellExeInfo;
    DWORD exitCode;
	WCHAR dir[_MAX_PATH];
    WCHAR *vcredistX86 = TEXT("vcredist_x86.exe");
	WCHAR *vcredistX64 = TEXT("vcredist_x64.exe");
	WCHAR filename[_MAX_PATH];

	// Get the directory of the running exe and use it as the root path
	if (!get_module_directory(dir, _MAX_PATH))     
    {   
		_wgetcwd(dir, _MAX_PATH);
    }
	if (x64)
	{
		wcscat_s(dir, _MAX_PATH, TEXT("\\amd64"));
	}
	else
	{
		wcscat_s(dir, _MAX_PATH, TEXT("\\i386"));
	}

	// If the file doesn't exist, we just want to return without error
	wcscpy_s(filename, _MAX_PATH, dir);
	wcscat_s(filename, _MAX_PATH, _T("\\"));
	wcscat_s(filename, _MAX_PATH, x64 ? vcredistX64 : vcredistX86);
	if (!FileExists(filename))
	{
		return 0;
	}

	// Fill out execute information
	ZeroMemory(&shellExeInfo, sizeof(SHELLEXECUTEINFO));
	shellExeInfo.cbSize = sizeof(SHELLEXECUTEINFO);
			
	// If no command-line args are passed, automatically pass \q which
	// quietly installs the MSVC++ Runtime
	shellExeInfo.lpDirectory = dir;
	shellExeInfo.lpFile = x64 ? vcredistX64 : vcredistX86;
	shellExeInfo.lpParameters = TEXT("/q");
	shellExeInfo.nShow = SW_SHOWNORMAL;
	shellExeInfo.hInstApp = hInstance;
	shellExeInfo.fMask = SEE_MASK_NOCLOSEPROCESS;

	// Execute DPInst driver installer
	ShellExecuteEx(&shellExeInfo);

	// Check for execution error
	if ((DWORD)shellExeInfo.hInstApp <= 32)
	{
		return (DWORD)shellExeInfo.hInstApp;
	}

	// Wait for the process to die
	WaitForSingleObject(shellExeInfo.hProcess, INFINITE);

	// Get the exit code
    GetExitCodeProcess(shellExeInfo.hProcess, &exitCode);

	// Close the handles
	CloseHandle(shellExeInfo.hProcess);

	// If vcredist could not be installed, return with failure
	//     0 =     success, no reboot required
	//     3010 =  success, reboot required
	//     other = failure
	if ((exitCode != 0) && (exitCode != 3010))
	{
		// Error 1618: Another installation is already in progress
		// Error list at http://desktopengineer.com/msierrors
		WCHAR msg[256];
		swprintf(msg, 256, _T("Error trying to install the Microsoft Visual C++ 2008 Runtime.  Failed with error code: %i"), exitCode);
		MessageBox(NULL, msg, _T("Install Error"), 0);
		return exitCode;
	}

	// Success
	return 0;
}



bool FileExists(WCHAR *filename)
{
  struct _stat fileInfo; 

  // Attempt to get the file attributes 
  return _wstat(filename, &fileInfo) == 0;
}
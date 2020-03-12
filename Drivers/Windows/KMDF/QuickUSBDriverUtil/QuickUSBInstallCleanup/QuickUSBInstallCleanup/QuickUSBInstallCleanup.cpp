// QuickUSBInstallCleanup.cpp : Defines the entry point for the co//nsole application.
//

#include "stdafx.h"

BOOL deleteDir(WCHAR *, int, BOOL);

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	if (wcslen(lpCmdLine) == 0)
	{
		return 0;
	}

	return !deleteDir(lpCmdLine, 0, TRUE);
}

// Recursive depth-first folder deletion
BOOL deleteDir(WCHAR *folder, int depth, BOOL recursive)
{
	const int PathLen = 1024;
	BOOL ok = TRUE, del;
	DWORD dwError=0;
	WIN32_FIND_DATA ffd;
	HANDLE hFind;
	size_t len;
	WCHAR folderSearch[PathLen], newPath[PathLen];
	
	// Contruct the folder search string
	wcscpy_s(folderSearch, PathLen, folder);
	len = wcslen(folder);
	if (folder[len-1] != '\\')
	{
		wcscat_s(folderSearch, PathLen, TEXT("\\"));
	}
	wcscat_s(folderSearch, PathLen, TEXT("*"));
	

	// Begin the file search
	hFind = FindFirstFile(folderSearch, &ffd);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	
	// Search for files
	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (recursive)
			{
				// If we're at the root dir level, only delete known folders
				del = FALSE;
				if (depth == 0)
				{
					if (wcsicmp(ffd.cFileName, TEXT("CAD")) == 0 ||
						wcsicmp(ffd.cFileName, TEXT("Doc")) == 0 ||
						wcsicmp(ffd.cFileName, TEXT("Library")) == 0 ||
						wcsicmp(ffd.cFileName, TEXT("QuickUsbDiag")) == 0 ||
						wcsicmp(ffd.cFileName, TEXT("QuickUsbProgrammer")) == 0 ||
						wcsicmp(ffd.cFileName, TEXT("QuickUsbCustomizer")) == 0 ||
						wcsicmp(ffd.cFileName, TEXT("Drivers")) == 0 ||
						wcsicmp(ffd.cFileName, TEXT("Samples")) == 0)
					{
						del = TRUE;
					}
				}
				// Don't try to delete . or ..
				else if (wcsicmp(ffd.cFileName, TEXT(".")) != 0 && wcsicmp(ffd.cFileName, TEXT("..")) != 0)
				{
					del = TRUE;
				}
				
				// Delete the folder if we should
				if (del)
				{
					// Create the full folder path
					wcscpy_s(newPath, PathLen, folder);
					if (folder[len-1] != '\\')
					{
						wcscat_s(newPath, PathLen, TEXT("\\"));
					}
					wcscat_s(newPath, ffd.cFileName);

					// Delete the folder, but do not delete folders in the Drivers dir
					ok &= deleteDir(newPath, depth+1, (wcsicmp(ffd.cFileName, TEXT("Drivers")) != 0));
				}
			}
		}
		else
		{
			// Create the full file path
			wcscpy_s(newPath, PathLen, folder);
			if (folder[len-1] != '\\')
			{
				wcscat_s(newPath, PathLen, TEXT("\\"));
			}
			wcscat_s(newPath, ffd.cFileName);

			// Check if the file is read-only
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
			{
				ok &= SetFileAttributes(newPath, FILE_ATTRIBUTE_NORMAL);
			}

			ok &= (BOOL) DeleteFile(newPath);
		}
	} while (FindNextFile(hFind, &ffd) != 0);dwError = GetLastError();

	// Check for errors
	if (dwError != ERROR_NO_MORE_FILES) 
	{
		ok &= FALSE;
	}

	// Close the file search
	FindClose(hFind);

	// Delete the folder
	ok &= RemoveDirectory(folder);

	return ok;
}


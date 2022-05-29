#pragma once
//============================================================================
// Copyright (C) 2013 Brett R. Jones
// Issued to MIT style license by Brett R. Jones in 2017
//
// You may use, copy, modify, merge, publish, distribute, sub-license, and/or sell this software
// provided this Copyright is not modified or removed and is included all copies or substantial portions of the Software
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// bjones.engineer@gmail.com
// http://www.nolimitconnect.org
//============================================================================
#ifdef TARGET_OS_WINDOWS

#include <CoreLib/config_corelib.h>

#include <WinSock2.h>
#include <windows.h>
#include <string>

HRESULT VxCreateShortcut(	std::wstring csLinkName, //Full path to link ( link name should not have the .lnk extention )
												//example C:\\windows\\Desktop\\MyApp
							std::wstring csPathToExe, //path to exe to run when link is run
							std::wstring csLinkDesc	); //description of link

HRESULT VxResolveShortcut(	HWND hWnd,				//handle to window of caller
							std::wstring lpszLinkFile,	//.lnk file
							std::wstring& lpszPath );		//return path to target file
#endif // TARGET_OS_WINDOWS

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
#include "config_corelib.h"

#ifdef TARGET_OS_WINDOWS

#include "VxShortcut.h"
#include "VxDebug.h"
#include "VxParse.h"

#include "winnls.h"
#include "shobjidl.h"
#include "objbase.h"
#include "objidl.h"
#include "shlguid.h"

//============================================================================
HRESULT VxCreateShortcut(	std::wstring csLinkName, //Full path to link ( link name should not have the .lnk extention )
												//example C:\\windows\\Desktop\\MyApp
							std::wstring csPathToExe, //path to exe to run when link is run
							std::wstring csLinkDesc	) //description of link
{
	HRESULT hres;
	IShellLinkW* psl = 0;

	std::wstring csLinkPath = csLinkName;

	hres = CoCreateInstance(CLSID_ShellLink, 
						NULL, 
						CLSCTX_INPROC_SERVER,
						IID_IShellLinkW, 
						(LPVOID*) &psl);
	if (SUCCEEDED(hres))
	{
		IPersistFile* ppf;

		psl->SetPath( csPathToExe.c_str() );
		psl->SetDescription( csLinkDesc.c_str() );

		hres = psl->QueryInterface( IID_IPersistFile, (LPVOID *) &ppf);

		if (SUCCEEDED(hres))
		{
			csLinkPath += L".lnk";  // Important !!!

			//WORD wsz[VX_MAX_PATH];
			//MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, csLinkPath.c_str(), -1, (LPWSTR)wsz, VX_MAX_PATH);

			//hres = ppf->Save((LPCOLESTR)wsz, TRUE);
			hres = ppf->Save((LPCOLESTR)csLinkPath.c_str(), TRUE);
			if (hres != S_OK )
				LogMsg( LOG_ERROR, "IPersistFile->Save() Error" );
			
			ppf->Release();
		}
		psl->Release();
	}
	else
	{
		if( 0x800401f0 == hres )
		{
			LogMsg( LOG_ERROR, "Must call AfxOleInit() before calling VxCreateShortcut.. Call AfxOleInit()" );
		}
		if( S_OK == hres )
		{
			LogMsg( LOG_ERROR, "An instance of the specified object class was successfully created. \n");
		}
		else if( REGDB_E_CLASSNOTREG == hres )
		{
			LogMsg( LOG_ERROR, "A specified class is not registered in the registration database. Also can indicate that the type of server you requested in the CLSCTX enumeration is not registered or the values for the server types in the registry are corrupt. \n");
		}
		else if( CLASS_E_NOAGGREGATION  == hres )
		{
			LogMsg( LOG_ERROR, "This class cannot be created as part of an aggregate. \n");
		}
		else
		{
			LogMsg( LOG_ERROR, "Unknown error\n");
		}

	}
	return hres;
}
//============================================================================
HRESULT VxResolveShortcut(	HWND hwnd, //handle to window of caller
							std::wstring pszShortcutFile, //.lnk file
							std::wstring& pszPath) //return path to target file
{ 
	HRESULT hres;
	IShellLinkW* psl;
	wchar_t szGotPath[VX_MAX_PATH];
//	char szDescription[VX_MAX_PATH];
	WIN32_FIND_DATAW wfd;
	
	pszPath = L"";   // assume failure
	
	// Get a pointer to the IShellLink interface.
	hres = CoCreateInstance(	CLSID_ShellLink, 
		NULL, 
		CLSCTX_INPROC_SERVER,
		IID_IShellLinkW, 
		(void **)&psl);
	if (SUCCEEDED(hres))
	{
		IPersistFile* ppf;
		
		// Get a pointer to the IPersistFile interface.
		hres = psl->QueryInterface(IID_IPersistFile, (void **)&ppf);
		if (SUCCEEDED(hres))
		{
			//WORD wsz[VX_MAX_PATH];
			
			// Ensure string is Unicode.
			//MultiByteToWideChar(CP_ACP, 0, pszShortcutFile, -1, (LPWSTR)wsz,
			//	VX_MAX_PATH);
			
			// Load the shell link.
			//hres = ppf->Load((LPCOLESTR)wsz, STGM_READ);
			hres = ppf->Load((LPCOLESTR)pszShortcutFile.c_str(), STGM_READ);
			if (SUCCEEDED(hres))
			{
				// Resolve the link.
				hres = psl->Resolve(hwnd, SLR_ANY_MATCH);
				
				if (SUCCEEDED(hres))
				{
					wstrcpy(szGotPath, pszShortcutFile.c_str());
					// Get the path to the link target.
					hres = psl->GetPath(szGotPath, VX_MAX_PATH, (WIN32_FIND_DATAW *)&wfd, 
						SLGP_UNCPRIORITY  );
					if( !SUCCEEDED(hres) )
					{	
						LogMsg( LOG_ERROR, "GetPath failed!\n" );
					}
					else
					{
						pszPath = szGotPath;
					}
					// works but I dont need the description
					// Get the description of the target.         
//					hres = psl->GetDescription(szDescription, VX_MAX_PATH);
//					if (!SUCCEEDED(hres))
//					{
//						AfxMessageBox("GetDescription failed!");
//					}
				}
			}
			// Release pointer to IPersistFile interface.
			ppf->Release();
		}
		// Release pointer to IShellLink interface.
		psl->Release();
	}
	else
	{
		if( 0x800401f0 == hres )
		{
			LogMsg( LOG_ERROR, "Must call AfxOleInit() before calling VxResolveShortcut.. Call AfxOleInit()" );
		}
		if( S_OK == hres )
		{
			LogMsg( LOG_ERROR, "An instance of the specified object class was successfully created. \n");
		}
		else if( REGDB_E_CLASSNOTREG == hres )
		{
			LogMsg( LOG_ERROR, "A specified class is not registered in the registration database. Also can indicate that the type of server you requested in the CLSCTX enumeration is not registered or the values for the server types in the registry are corrupt. \n");
		}
		else if( CLASS_E_NOAGGREGATION  == hres )
		{
			LogMsg( LOG_ERROR, "This class cannot be created as part of an aggregate. \n");
		}
		else
		{
			LogMsg( LOG_ERROR, "Unknown error\n");
		}
	}
	return hres;
} 

#endif // TARGET_OS_WINDOWS

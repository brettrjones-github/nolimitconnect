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
// http://www.gotvptop.com
//============================================================================
#include "config_corelib.h"

#include "VxGlobals.h"
#include "VxFileUtil.h"
#include "VxMutex.h"
#include "VxFileShredder.h"
#include "VxGUID.h"

#include <time.h>
#include <string>
#include <stdio.h>
#include <algorithm>

#ifndef TARGET_OS_WINDOWS
    #include <sys/time.h>
#endif // TARGET_OS_WINDOWS

//============================================================================
// globals
//============================================================================

#if defined(TARGET_OS_ANDROID)
# define PYTHON_RELATIVE_PATH "assets/kodi/python2.7/lib/python2.7/"
#else
# define PYTHON_RELATIVE_PATH "assets/kodi/system/Python/"
#endif // TARGET_OS_WINDOWS

namespace
{
	VxMutex				g_GlobalAccessMutex;

	uint16_t			g_u16AppVersion					= 0x104;
#ifdef DEBUG
    std::string			g_strApplicationTitle			= "GoTv PtoP";
    std::string			g_strApplicationNameNoSpaces	= "GoTvPtoP";
    std::string			g_strApplicationNameNoSpacesLowerCase	= "gotvptop";
    std::string			g_strNetworkName				= "GoTvPtoPTestNet";
	std::string			g_strCompanyWebsite				= "http://www.gotvptop.com";
    std::string			g_strCompanyDomain				= "gotvptop.com";
    std::string			g_strOrginizationName           = "gotvptop";
	bool				g_IsAppCommercial				= false;
#else
    std::string			g_strApplicationTitle			= "GoTv PtoP";
    std::string			g_strApplicationNameNoSpaces	= "GoTvPtoP";
    std::string			g_strApplicationNameNoSpacesLowerCase	= "gotvptop";
    std::string			g_strNetworkName				= "GoTvPtoPNet";
	std::string			g_strCompanyWebsite				= "http://www.gotvptop.com";
    std::string			g_strCompanyDomain				= "gotvptop.com";
    std::string			g_strOrginizationName           = "gotvptop";
	bool				g_IsAppCommercial				= false;
#endif // APP_MYP2PWEB
	// exe and app resouces paths
    std::string			g_strAppExeDir                  = "";
	std::string			g_strKodiExeDir				    = "";
    std::string			g_strExeDirPython				= "";
    std::string			g_strExeDirPythonDlls			= "";
    std::string			g_strExeDirPythonLib			= "";
    std::string			g_strExeKodiAssetsDir           = "";
	std::string			g_strExeGoTvAssetsDir           = "";

	// user writeable paths
	std::string			g_strRootDataStorageDir         = "";
	std::string			g_strAppTempDir                 = "";
	std::string			g_strAppLogsDir                 = "";
	std::string			g_strAppGoTvDataDir             = "";
	std::string			g_strAppKodiDataDir             = "";

	// user specific writable paths
	std::string			g_strRootUserDataDir            = "";

	std::string			g_strUserSpecificDataDir        = "";
	std::string			g_strUserXferDir                = "";

	std::string			g_strRootXferDir				= "";
    std::string			g_strUserProfileDir             = "";
    std::string			g_strSettingsDir				= "";

	std::string			g_strUploadsDir					= "";
	std::string			g_strDownloadsDir				= "";
	std::string			g_strIncompleteDir				= "";
	std::string			g_strPersonalRecordDir			= "";


	bool				g_bIsAppShuttingDown			= false;
	bool				g_bIsNetLoopbackAllowed			= false;
	bool				g_bIsDebugEnabled				= true;
	VxGUID				g_MyOnlineId;
	std::string			g_LclIpAddress					= "";
}

// directory structure on disk
// exe paths
// /exe/								all executables including python
// /exe/assets/kodi/system/Python/Lib	python libs path
// /exe/assets/kodi/system/Python/DLLs	python dlls path
// /exe/assets/kodi		kodi exe assets path
//            /gotv		gotv assets path

// data storage paths linux      /home/user/.local/share/gotvptop
//                    windows    C:\Users\user\AppData\Roaming\GoTvPtoP
//                    android ?
// /storage/gotvptop/temp/		temporary files path
//                  /logs/		log files path
//                  /gotv/		ShredFilesDb.db3 and app generated files
//                  /kodi/		kodi plugins and writable data directory
//                  /gotv/gui/	gui assets
//						 /shaders/ opengl shaders
//                       /profile/ profile default files
//
// user specific directories.. NOTE: hasnum is 4 digit hash of exe path and userId is user login name
//                  /storage/GoTvPtoP/hashnum/accounts/userId/settings/		databases for user settings etc
//									 /hashnum/accounts/userId/profile/		profile and story board user web pages
// user xfer directories      
//                  Documents Directory/GoTvPtoP/hashnum/userId/downloads
//																/uploads		uploading directory
//																/incomplete		not yet completed downloads
//																/me/			personal recordings
//																/contacts/		contact assets

std::string& VxGetAppDirectory(EAppDir appDir)
{
	switch (appDir)
	{
    case eAppDirAppExe:
        return g_strAppExeDir;
	case eAppDirKodiExe:
		return g_strKodiExeDir;
	case eAppDirExeKodiAssets:
		return g_strExeKodiAssetsDir;
	case eAppDirExeGoTvAssets:
		return g_strExeGoTvAssetsDir;

	case eAppDirExePython:
        return g_strExeDirPython;
	case eAppDirExePythonDlls:
        return g_strExeDirPythonDlls;
	case eAppDirExePythonLibs:
        return g_strExeDirPythonLib;

	case eAppDirRootDataStorage:
		return g_strRootDataStorageDir;
	case eAppDirAppTempData:
		return g_strAppTempDir;
	case eAppDirAppLogs:
		return g_strAppLogsDir;
	case eAppDirAppKodiData:
		return g_strAppKodiDataDir;
	case eAppDirAppGoTvData:
		return g_strAppGoTvDataDir;

	case eAppDirRootUserData:
		return g_strRootUserDataDir;

	case eAppDirUserSpecific:
		return g_strUserSpecificDataDir;
	case eAppDirSettings:
		return g_strSettingsDir;
	case eAppDirProfile:
		return g_strUserProfileDir;
	case eAppDirRootXfer:
		return g_strRootXferDir;
	case eAppDirUserXfer:
		return g_strUserXferDir;
	case eAppDirDownloads:
		return g_strDownloadsDir;
	case eAppDirUploads:
		return g_strUploadsDir;
	case eAppDirIncomplete:
		return g_strIncompleteDir;
	case eAppDirPersonalRecords:
		return g_strPersonalRecordDir;
	}

static std::string emptyStr = "";
	return emptyStr;
}


//============================================================================
void VxSetAppIsShuttingDown( bool bIsShuttingDown )
{
	g_bIsAppShuttingDown = bIsShuttingDown;
}

//============================================================================
bool VxIsAppShuttingDown( void )
{
	return g_bIsAppShuttingDown;
}

//============================================================================
void VxSetMyOnlineId( VxGUID& onlineId )
{
	g_MyOnlineId = onlineId;
}

//============================================================================
const VxGUID& VxGetMyOnlineId( void )
{
	return g_MyOnlineId;
}


//============================================================================
void VxSetLclIpAddress( const char * lclIpAddress )
{
	if( 0 != lclIpAddress )
	{
		g_LclIpAddress = lclIpAddress;
	}
	else
	{
		g_LclIpAddress = "";
	}
}

//============================================================================
std::string VxGetLclIpAddress( void )
{
	return g_LclIpAddress;
}

//============================================================================
const char * VxGetCompanyDomain( void )
{
	return g_strCompanyDomain.c_str();
}

//============================================================================
const char * VxGetOrginizationNmae( void )
{
    return g_strOrginizationName.c_str();
}

//============================================================================
const char * VxGetCompanyWebsite( void )
{
	return g_strCompanyWebsite.c_str();
}

//============================================================================
const char * VxGetApplicationTitle( void )
{
	return g_strApplicationTitle.c_str();
}

//============================================================================
//! set application name
void VxSetApplicationNameNoSpaces( const char * pAppName )
{
	g_strApplicationNameNoSpaces = pAppName;
    g_strApplicationNameNoSpacesLowerCase = pAppName;
    transform(g_strApplicationNameNoSpacesLowerCase.begin(),
              g_strApplicationNameNoSpacesLowerCase.end(),
              g_strApplicationNameNoSpacesLowerCase.begin(), ::tolower);
}

//============================================================================
const char * VxGetApplicationNameNoSpaces( void )
{
	return g_strApplicationNameNoSpaces.c_str();
}

//============================================================================
const char * VxGetApplicationNameNoSpacesLowerCase( void )
{
    return g_strApplicationNameNoSpacesLowerCase.c_str();
}

//============================================================================
void VxSetAppVersion( uint16_t u16AppVersion )
{
	g_u16AppVersion = u16AppVersion;
}

//============================================================================
uint16_t VxGetAppVersion( void )
{
	return g_u16AppVersion;
}

//============================================================================
const char * VxGetAppVersionString( void )
{
	static std::string strAppVersion;
	static bool isSet = false;
	if( false == isSet )
	{
		uint32_t u16AppVersion = VxGetAppVersion();
		char as8Buf[32];
		sprintf( as8Buf, "%d.%d",
			(u16AppVersion & 0xff00) >> 8,
			(u16AppVersion & 0x00ff)
			);
		strAppVersion = as8Buf;
	}

	return strAppVersion.c_str();
}

//============================================================================
void VxSetIsApplicationCommercial( bool isCommercial )
{
	g_IsAppCommercial = isCommercial;
}

//============================================================================
bool VxGetIsApplicationCommercial( void )
{
	return g_IsAppCommercial;
}

//============================================================================
void VxSetDebugEnable( bool enableDebug )
{
	g_bIsDebugEnabled = enableDebug;
}

//============================================================================
bool VxIsDebugEnabled( void )
{
	#ifndef	_DEBUG
		return false; // dont do logging in release mode
	#endif //	_DEBUG

	return g_bIsDebugEnabled;
}


//============================================================================
//=== miscellaneous ===//
//============================================================================

//============================================================================
const char * VxGetP2PNetworkName()
{
	return g_strNetworkName.c_str();
}

//============================================================================
void VxSetP2PNetworkName( const char * pNetName )
{
	g_strNetworkName = pNetName;
}

//============================================================================
//! set true if loop back is allowed ( default is false )
void VxSetNetworkLoopbackAllowed( bool bIsLoopbackAllowed )
{
	g_bIsNetLoopbackAllowed = bIsLoopbackAllowed;
}

//============================================================================
//! return true if loop back is allowed
bool VxIsNetworkLoopbackAllowed( void )
{
	return g_bIsNetLoopbackAllowed;
}

//============================================================================
//=== directories ===//
//============================================================================
void VxSetAppExeDirectory( const char * exeDir )
{
    g_strAppExeDir = exeDir;
}

//============================================================================
void VxSetKodiExeDirectory(const char * exeDir)
{
	g_strKodiExeDir = exeDir;
    g_strExeDirPython = exeDir;
    g_strExeDirPythonDlls = exeDir;
    g_strExeDirPythonDlls = g_strExeDirPythonDlls + PYTHON_RELATIVE_PATH;
    g_strExeDirPythonLib = g_strExeDirPythonDlls;
    g_strExeDirPythonDlls = g_strExeDirPythonDlls + "DLLs/";
    g_strExeDirPythonLib = g_strExeDirPythonLib + "Lib/";

	g_strExeKodiAssetsDir = g_strKodiExeDir + "assets/kodi/";
	g_strExeGoTvAssetsDir = g_strKodiExeDir + "assets/gotv/";
}

//============================================================================
std::string& VxGetAppExeDirectory( void ) { return g_strAppExeDir; }
std::string& VxGetKodiExeDirectory(void) { return g_strKodiExeDir; }

void VxSetExeKodiAssetsDirectory(const char * assetsDir){ g_strExeKodiAssetsDir = assetsDir; }
std::string& VxGetExeKodiAssetsDirectory( void ) { return g_strExeKodiAssetsDir; }

void VxSetExeGoTvAssetsDirectory(const char * assetsDir){ g_strExeGoTvAssetsDir = assetsDir; }
std::string& VxGetExeGoTvAssetsDirectory(void) { return g_strExeGoTvAssetsDir; }

void VxSetPythonExeDirectory( const char * pythonDir ){ g_strExeDirPython = pythonDir; }
void VxSetPythonDllDirectory( const char * pythonDir ){ g_strExeDirPythonDlls = pythonDir; }
void VxSetPythonLibDirectory( const char * pythonDir ){ g_strExeDirPythonLib = pythonDir; }

//============================================================================
void VxSetRootDataStorageDirectory(const char * rootDataDir)
{
	g_strRootDataStorageDir = rootDataDir;
	VxFileUtil::makeDirectory( rootDataDir );

	g_strAppTempDir = g_strRootDataStorageDir + "temp/";
	VxFileUtil::makeDirectory(g_strAppTempDir.c_str());

	g_strAppLogsDir = g_strRootDataStorageDir + "logs/";
	VxFileUtil::makeDirectory(g_strAppTempDir.c_str());

	g_strAppGoTvDataDir = g_strRootDataStorageDir + "gotv/";
	VxFileUtil::makeDirectory(g_strAppGoTvDataDir.c_str());

	g_strAppKodiDataDir = g_strRootDataStorageDir + "kodi/";
	VxFileUtil::makeDirectory( g_strAppKodiDataDir.c_str());

	GetVxFileShredder().initShredder( g_strAppGoTvDataDir );
}

//============================================================================
std::string& VxGetRootDataStorageDirectory(void) { return g_strRootDataStorageDir; }
std::string& VxGetAppTempDirectory(void) { return g_strAppTempDir; }
std::string& VxGetAppLogsDirectory(void) { return g_strAppLogsDir; }
std::string& VxGetAppGoTvDataDirectory(void) { return g_strAppGoTvDataDir; }
std::string& VxGetAppKodiDataDirectory(void) { return g_strAppKodiDataDir; }

//============================================================================
void VxSetRootUserDataDirectory( const char * rootUserDataDir )
{
	// basically /storage/ GoTvPtoP/hasnum/ where hashnum is hash of exe path
	g_strRootUserDataDir = rootUserDataDir;
	VxFileUtil::makeDirectory( g_strRootUserDataDir.c_str() );
}

//============================================================================
std::string& VxGetRootUserDataDirectory( void ) { return g_strRootUserDataDir; }



//============================================================================
void VxSetUserSpecificDataDirectory( const char * userDataDir  )
{ 
	g_strUserSpecificDataDir = userDataDir;
	VxFileUtil::makeDirectory( userDataDir );

	g_strSettingsDir = g_strUserSpecificDataDir + "settings/";
	VxFileUtil::makeDirectory( g_strSettingsDir.c_str() );

    g_strUserProfileDir = g_strUserSpecificDataDir + "profile/";
    VxFileUtil::makeDirectory( g_strUserProfileDir.c_str() );
}
//============================================================================
std::string& VxGetUserSpecificDataDirectory( void ) { return g_strUserSpecificDataDir; }
std::string& VxGetSettingsDirectory( void ) { return g_strSettingsDir; }
std::string& VxGetUserProfileDirectory( void ) { return g_strUserProfileDir; }

//============================================================================
void VxSetRootXferDirectory( const char * rootXferDir  )
{ 
	g_strRootXferDir = rootXferDir; 
	VxFileUtil::makeDirectory(g_strRootXferDir.c_str());
}

//============================================================================
std::string& VxGetRootXferDirectory( void ) { return g_strRootXferDir; }

//============================================================================
void VxSetUserXferDirectory( const char * userXferDir  )
{
	g_strUserXferDir		= userXferDir;
	VxFileUtil::makeDirectory( g_strUserXferDir.c_str() );

	g_strDownloadsDir		= g_strUserXferDir + "downloads/";
	VxFileUtil::makeDirectory( g_strDownloadsDir.c_str() );

	g_strUploadsDir			= g_strUserXferDir + "uploads/";
	VxFileUtil::makeDirectory( g_strUploadsDir.c_str() );

	g_strIncompleteDir		= g_strUserXferDir + "incomplete/";
	VxFileUtil::makeDirectory( g_strIncompleteDir.c_str() );

	g_strPersonalRecordDir	= g_strUserXferDir + "me/";
	VxFileUtil::makeDirectory( g_strPersonalRecordDir.c_str() );
}

//============================================================================
std::string& VxGetUserXferDirectory( void  ) { return g_strUserXferDir; }
std::string& VxGetDownloadsDirectory( void ) { return g_strDownloadsDir; }
std::string& VxGetUploadsDirectory( void ) { return g_strUploadsDir; }
std::string& VxGetIncompleteDirectory( void ) { return g_strIncompleteDir; }
std::string& VxGetPersonalRecordDirectory( void ) { return g_strPersonalRecordDir; }

//============================================================================
int VxGlobalAccessLock( void )
{
	return g_GlobalAccessMutex.lock();
}

//============================================================================
int VxGlobalAccessUnlock( void )
{
	return g_GlobalAccessMutex.unlock();
}

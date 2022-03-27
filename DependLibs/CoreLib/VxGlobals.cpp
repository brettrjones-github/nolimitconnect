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
// http://www.nolimitconnect.com
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

	uint16_t			g_u16AppVersion					= 0x101;

    std::string			g_strApplicationTitle			= "No Limit Connect";
    std::string			g_strApplicationNameNoSpaces	= "NoLimitConnect";
    std::string			g_strApplicationNameNoSpacesLowerCase	= "nolimitconnect";
	std::string			g_strCompanyWebsite				= "http://www.nolimitconnect.com";
    std::string			g_strCompanyDomain				= "nolimitconnect.com";
    std::string			g_strOrginizationName           = "nolimitconnect";
	bool				g_IsAppCommercial				= false;

    std::string			g_strNetworkHostName            = "nolimitconnect.net";
    uint16_t            g_NetworkHostPort               = 45124;
    std::string			g_strNetworkHostUrl             = "ptop://nolimitconnect.net:45124";


	// exe and app resouces paths
    std::string			g_strAppExeDir                  = "";
	std::string			g_strKodiExeDir				    = "";
    std::string			g_strExeDirPython				= "";
    std::string			g_strExeDirPythonDlls			= "";
    std::string			g_strExeDirPythonLib			= "";
    std::string			g_strExeKodiAssetsDir           = "";
	std::string			g_strExeNoLimitAssetsDir        = "";

	// user writeable paths
	std::string			g_strRootDataStorageDir         = "";
	std::string			g_strAppTempDir                 = "";
	std::string			g_strAppLogsDir                 = "";
	std::string			g_strAppNoLimitDataDir          = "";
	std::string			g_strAppKodiDataDir             = "";

	// user specific writable paths
	std::string			g_strRootUserDataDir            = "";

	std::string			g_strUserSpecificDataDir        = "";
	std::string			g_strUserXferDir                = "";

	std::string			g_strRootXferDir				= "";
    std::string			g_strAboutMePageServerDir       = "";
	std::string			g_strAboutMePageClientDir		= "";
	std::string			g_strStoryBoardPageServerDir	= "";
	std::string			g_strStoryBoardPageClientDir	= "";
    std::string			g_strSettingsDir				= "";

	std::string			g_strUploadsDir					= "";
	std::string			g_strDownloadsDir				= "";
	std::string			g_strIncompleteDir				= "";
	std::string			g_strPersonalRecordDir			= "";
    std::string			g_strAppThumbsDir               = "";
    std::string			g_strAppCamRecord               = "";


	bool				g_bIsAppShuttingDown			= false;
	bool				g_bIsNetLoopbackAllowed			= false;
	bool				g_bIsDebugEnabled				= true;
	std::string			g_LclIpAddress					= "";
}

//============================================================================
void            VxSetNetworkHostName( const char * netHostName )    { g_strNetworkHostName = netHostName; }
const char *    VxGetNetworkHostName( void )                        { return g_strNetworkHostName.c_str(); }
void            VxSetNetworkHostPort( uint16_t netHostPort )        { g_NetworkHostPort = netHostPort; }
uint16_t        VxGetNetworkHostPort( void )                        { return g_NetworkHostPort; }
void            VxSetNetworkHostUrl( const char * netHostUrl )      { g_strNetworkHostUrl = netHostUrl; }
const char *    VxGetNetworkHostUrl( void )                         { return g_strNetworkHostUrl.c_str(); }

// directory structure on disk
// exe paths
// /exe/								all executables including python
// /exe/assets/kodi/system/Python/Lib	python libs path
// /exe/assets/kodi/system/Python/DLLs	python dlls path
// /exe/assets/kodi		kodi exe assets path
//            /nolimit  no limit connect assets path

// data storage paths linux      /home/user/.local/share/nolimitconnect
//                    windows    C:\Users\user\AppData\Roaming\NoLimitConnect
//                    android ?
// /storage/nolimitconnect/temp/		temporary files path
//                  /logs/		log files path
//                  /nolimit/		ShredFilesDb.db3 and app generated files
//                  /kodi/		kodi plugins and writable data directory
//                  /nolimit/gui/	gui assets
//						 /shaders/ opengl shaders
//                       /profile/ profile default files
//                       /thumbs/ thumbnail assets directory
//
// user specific directories.. NOTE: hasnum is 4 digit hash of exe path and userId is user login name
//                  /storage/NoLimitConnect/hashnum/accounts/userId/settings/		databases for user settings etc
//									 /hashnum/accounts/userId/profile/		profile and story board user web pages
// user xfer directories      
//                  Documents Directory/NoLimitConnect/hashnum/userId/downloads
//																/uploads		uploading directory
//																/incomplete		not yet completed downloads
//																/me/			personal recordings
//																/contacts/		contact assets
//																/camrecord/		web cam recordings

//============================================================================
void VxSetAppDirectory( EAppDir appDir, const char * setDir )
{
    if( setDir )
    {
        switch (appDir)
        {
        case eAppDirAppExe:
            g_strAppExeDir = setDir;
            break;
        case eAppDirKodiExe:
            g_strKodiExeDir = setDir;
            break;
        case eAppDirExeKodiAssets:
            g_strExeKodiAssetsDir = setDir;
            break;
        case eAppDirExeNoLimitAssets:
            g_strExeNoLimitAssetsDir = setDir;
            break;
        case eAppDirExePython:
            g_strExeDirPython = setDir;
            break;
        case eAppDirExePythonDlls:
            g_strExeDirPythonDlls = setDir;
            break;
        case eAppDirExePythonLibs:
            g_strExeDirPythonLib = setDir;
            break;

        case eAppDirRootDataStorage:
            g_strRootDataStorageDir = setDir;
            break;
        case eAppDirAppTempData:
            g_strAppTempDir = setDir;
            break;
        case eAppDirAppLogs:
            g_strAppLogsDir = setDir;
            break;
        case eAppDirAppKodiData:
            g_strAppKodiDataDir = setDir;
            break;
        case eAppDirAppNoLimitData:
            g_strAppNoLimitDataDir = setDir;
            break;

        case eAppDirRootUserData:
            g_strRootUserDataDir = setDir;
            break;

        case eAppDirUserSpecific:
            g_strUserSpecificDataDir = setDir;
            break;
        case eAppDirSettings:
            g_strSettingsDir = setDir;
            break;
        case eAppDirAboutMePageServer:
            g_strAboutMePageServerDir = setDir;
            break;
		case eAppDirAboutMePageClient:
			g_strAboutMePageClientDir = setDir;
			break;
		case eAppDirStoryboardPageServer:
			g_strStoryBoardPageServerDir = setDir;
			break;
		case eAppDirStoryBoardPageClient:
			g_strStoryBoardPageClientDir = setDir;
			break;
        case eAppDirRootXfer:
            g_strRootXferDir = setDir;
            break;
        case eAppDirUserXfer:
            g_strUserXferDir = setDir;
            break;
        case eAppDirDownloads:
            g_strDownloadsDir = setDir;
            break;
        case eAppDirUploads:
            g_strUploadsDir = setDir;
            break;
        case eAppDirIncomplete:
            g_strIncompleteDir = setDir;
            break;
        case eAppDirPersonalRecords:
            g_strPersonalRecordDir = setDir;
            break;
        case eAppDirThumbs:
            g_strAppThumbsDir = setDir;
            break;
        case eAppDirCamRecord:
            g_strAppCamRecord = setDir;
            break;
        default:
            LogMsg( LOG_ERROR, "VxSetAppDirectory invalid param %d", appDir);
        }
    }
    else
    {
        LogMsg( LOG_ERROR, "VxSetAppDirectory setDir is null");
    }
}

//============================================================================
std::string& VxGetAppDirectory( EAppDir appDir )
{
	switch (appDir)
	{
    case eAppDirAppExe:
        return g_strAppExeDir;
	case eAppDirKodiExe:
		return g_strKodiExeDir;
	case eAppDirExeKodiAssets:
		return g_strExeKodiAssetsDir;
	case eAppDirExeNoLimitAssets:
		return g_strExeNoLimitAssetsDir;

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
	case eAppDirAppNoLimitData:
		return g_strAppNoLimitDataDir;

	case eAppDirRootUserData:
		return g_strRootUserDataDir;

	case eAppDirUserSpecific:
		return g_strUserSpecificDataDir;
	case eAppDirSettings:
		return g_strSettingsDir;
	case eAppDirAboutMePageServer:
		return g_strAboutMePageServerDir;
	case eAppDirAboutMePageClient:
		return g_strAboutMePageClientDir;
	case eAppDirStoryboardPageServer:
		return g_strStoryBoardPageServerDir;
	case eAppDirStoryBoardPageClient:
		return g_strStoryBoardPageClientDir;
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
    case eAppDirThumbs:
        return g_strAppThumbsDir;
    case eAppDirCamRecord:
        return g_strAppCamRecord;
    default:
        break;
	}

    LogMsg( LOG_ERROR, "VxGetAppDirectory ERROR No directory for %d", appDir );
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
const char * VxGetOrginizationName( void )
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
	VxFileUtil::assureTrailingDirectorySlash( g_strAppExeDir );
	g_strExeNoLimitAssetsDir = g_strAppExeDir + "assets/nolimit/";
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
}

//============================================================================
std::string& VxGetAppExeDirectory( void ) { return g_strAppExeDir; }
std::string& VxGetKodiExeDirectory(void) { return g_strKodiExeDir; }

void VxSetExeKodiAssetsDirectory(const char * assetsDir){ g_strExeKodiAssetsDir = assetsDir; }
std::string& VxGetExeKodiAssetsDirectory( void ) { return g_strExeKodiAssetsDir; }

void VxSetExeNoLimitAssetsDirectory(const char * assetsDir){ g_strExeNoLimitAssetsDir = assetsDir; }
std::string& VxGetExeNoLimitAssetsDirectory(void) { return g_strExeNoLimitAssetsDir; }

void VxSetPythonExeDirectory( const char * pythonDir ){ g_strExeDirPython = pythonDir; }
void VxSetPythonDllDirectory( const char * pythonDir ){ g_strExeDirPythonDlls = pythonDir; }
void VxSetPythonLibDirectory( const char * pythonDir ){ g_strExeDirPythonLib = pythonDir; }

//============================================================================
void VxSetRootDataStorageDirectory(const char * rootDataDir)
{
	g_strRootDataStorageDir = rootDataDir;
	VxFileUtil::assureTrailingDirectorySlash( g_strRootDataStorageDir );
	VxFileUtil::makeDirectory( rootDataDir );

	g_strAppTempDir = g_strRootDataStorageDir + "temp/";
	VxFileUtil::makeDirectory(g_strAppTempDir.c_str());

	g_strAppLogsDir = g_strRootDataStorageDir + "logs/";
	VxFileUtil::makeDirectory(g_strAppTempDir.c_str());

	g_strAppNoLimitDataDir = g_strRootDataStorageDir + "nolimit/";
	VxFileUtil::makeDirectory(g_strAppNoLimitDataDir.c_str());


	g_strAppKodiDataDir = g_strRootDataStorageDir + "kodi/";
	VxFileUtil::makeDirectory( g_strAppKodiDataDir.c_str());

	GetVxFileShredder().initShredder( g_strAppNoLimitDataDir );
}

//============================================================================
std::string& VxGetRootDataStorageDirectory(void) { return g_strRootDataStorageDir; }
std::string& VxGetAppTempDirectory(void) { return g_strAppTempDir; }
std::string& VxGetAppLogsDirectory(void) { return g_strAppLogsDir; }
std::string& VxGetAppNoLimitDataDirectory(void) { return g_strAppNoLimitDataDir; }
std::string& VxGetAppKodiDataDirectory(void) { return g_strAppKodiDataDir; }
std::string& VxGetAppThumbnailDirectory(void) { return g_strAppThumbsDir; }

//============================================================================
void VxSetRootUserDataDirectory( const char * rootUserDataDir )
{
	// basically /storage/ NoLimitConnect/hasnum/ where hashnum is hash of exe path
	g_strRootUserDataDir = rootUserDataDir;
	VxFileUtil::assureTrailingDirectorySlash( g_strRootUserDataDir );
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

    g_strAboutMePageServerDir = g_strUserSpecificDataDir + "aboutmepage/";
    VxFileUtil::makeDirectory( g_strAboutMePageServerDir.c_str() );

	g_strStoryBoardPageServerDir = g_strUserSpecificDataDir + "storyboardpage/";
	VxFileUtil::makeDirectory( g_strStoryBoardPageServerDir.c_str() );
}

//============================================================================
std::string& VxGetUserSpecificDataDirectory( void )			{ return g_strUserSpecificDataDir; }
std::string& VxGetSettingsDirectory( void )					{ return g_strSettingsDir; }
std::string& VxGetAboutMePageServerDirectory( void )		{ return g_strAboutMePageServerDir; }
std::string& VxGetStoryBoardPageServerDirectory( void )		{ return g_strStoryBoardPageServerDir; }

//============================================================================
std::string VxGetAboutMePageClientDirectory( VxGUID& onlineId )
{ 
	std::string clientDir = g_strAboutMePageClientDir + onlineId.toHexString();
	VxFileUtil::makeDirectory( clientDir.c_str() );

    return clientDir;
}

//============================================================================
std::string VxGetStoryBoardPageClientDirectory( VxGUID& onlineId )
{
	std::string clientDir = g_strStoryBoardPageClientDir + onlineId.toHexString();
	VxFileUtil::makeDirectory( clientDir.c_str() );

    return clientDir;
}

//============================================================================
void VxSetRootXferDirectory( const char * rootXferDir  )
{ 
	g_strRootXferDir = rootXferDir; 
	VxFileUtil::assureTrailingDirectorySlash( g_strRootXferDir );
	VxFileUtil::makeDirectory(g_strRootXferDir.c_str());
	std::string noLimitDir = g_strRootXferDir + "nolimit/";
	VxFileUtil::makeDirectory( noLimitDir.c_str() );
	g_strAppThumbsDir = noLimitDir + "thumbs/";
	VxFileUtil::makeDirectory( g_strAppThumbsDir.c_str() );
	g_strAboutMePageClientDir = noLimitDir + "aboutme_cache/";
	VxFileUtil::makeDirectory( g_strAboutMePageClientDir.c_str() );
	g_strStoryBoardPageClientDir = noLimitDir + "storyboard_cache/";
	VxFileUtil::makeDirectory( g_strStoryBoardPageClientDir.c_str() );
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

    g_strAppCamRecord       = g_strUserXferDir + "camrecord/";
    VxFileUtil::makeDirectory( g_strAppCamRecord.c_str() );

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

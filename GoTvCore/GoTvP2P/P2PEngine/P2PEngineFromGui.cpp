//============================================================================
// Copyright (C) 2009 Brett R. Jones 
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

#include "P2PEngine.h"
#include "GoTvInterface/IGoTv.h"

#include "P2PConnectList.h"
#include "../NetServices/NetServicesMgr.h"
#include <GoTvCore/GoTvP2P/Network/NetworkMgr.h>
#include <GoTvCore/GoTvP2P/Network/NetworkStateMachine.h>

#include <GoTvCore/GoTvP2P/AssetMgr/AssetInfo.h>
#include <GoTvCore/GoTvP2P/AssetMgr/AssetMgr.h>
#include <GoTvCore/GoTvP2P/HostMgr/HostTest.h>
#include <GoTvCore/GoTvP2P/NetworkTest/IsPortOpenTest.h>
#include <GoTvCore/GoTvP2P/NetworkTest/QueryHostIdTest.h>
#include <GoTvCore/GoTvP2P/MediaProcessor/MediaProcessor.h>
#include <MediaToolsLib/MediaTools.h>

#include <GoTvCore/GoTvP2P/BigListLib/BigListInfo.h>
#include <GoTvCore/GoTvP2P/Plugins/PluginBase.h>
#include <GoTvCore/GoTvP2P/Plugins/PluginServiceFileShare.h>
#include <GoTvCore/GoTvP2P/Plugins/PluginNetServices.h>
#include <GoTvCore/GoTvP2P/Plugins/PluginMgr.h>
#include <GoTvCore/GoTvP2P/Plugins/PluginServiceRelay.h>

#include <NetLib/VxGetRandomPort.h>
#include <CoreLib/VxParse.h>
#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxFileShredder.h>

#include <string.h>
#include <stdio.h>

//============================================================================
void P2PEngine::assureUserSpecificDirIsSet( const char * checkReason )
{
	if( false == m_IsUserSpecificDirSet )
	{
		LogMsg( LOG_ERROR, "P2PEngine::assureUserSpecificDirIsSet %s\n", checkReason );
		vx_assert( false );
	}
}

//============================================================================
void P2PEngine::fromGuiSetIsAppCommercial( bool isCommercial )
{
	VxSetIsApplicationCommercial( isCommercial );
}

//============================================================================
bool P2PEngine::fromGuiGetIsAppCommercial( void )
{
	return VxGetIsApplicationCommercial();
}

//============================================================================
uint16_t P2PEngine::fromGuiGetAppVersionBinary( void )
{
	return VxGetAppVersion();
}

//============================================================================
const char * P2PEngine::fromGuiGetAppVersionString( void )
{
	return VxGetAppVersionString();
}

//============================================================================
const char * P2PEngine::fromGuiGetAppName( void )
{
	return VxGetApplicationTitle();
}

//============================================================================
const char * P2PEngine::fromGuiGetAppNameNoSpaces( void )
{
	return VxGetApplicationNameNoSpaces();
}

//============================================================================
void P2PEngine::fromGuiAppStartup( const char * assetsDir, const char * rootDataDir  )
{
	VxSetAppIsShuttingDown( false );
    enableTimerThread(true);

	LogMsg( LOG_INFO, "P2PEngine::fromGuiAppStartup %s %s\n", assetsDir, rootDataDir  );
}

//============================================================================
void P2PEngine::fromGuiSetUserXferDir( const char * userXferDir  )
{
	LogMsg( LOG_INFO, "P2PEngine::fromGuiSetUserXferDir %s\n", userXferDir  );
	VxSetUserXferDirectory( userXferDir );
	std::string incompleteDir =	VxGetIncompleteDirectory();
	// delete all incomplete files from previous usage
	std::vector<std::string>	fileList;
	VxFileUtil::listFilesInDirectory( incompleteDir.c_str(), fileList );
	std::vector<std::string>::iterator iter;
	for( iter = fileList.begin(); iter != fileList.end(); ++iter )
	{
		GetVxFileShredder().shredFile( *iter );
	}
}

//============================================================================
void P2PEngine::fromGuiSetUserSpecificDir( const char * userSpecificDir  )
{
	LogMsg( LOG_INFO, "P2PEngine::fromGuiSetUserSpecificDir %s\n", userSpecificDir  );
	VxSetUserSpecificDataDirectory( userSpecificDir );

    //LogMsg( LOG_DEBUG, "IGoTv::fromGuiSetUserSpecificDir %s\n", userDir);

	std::string strDbFileName = VxGetSettingsDirectory();
	strDbFileName += "biglist.db3";
	RCODE rc = m_BigListMgr.bigListMgrStartup( strDbFileName.c_str() );
	if( rc )
	{
		LogMsg( LOG_ERROR, "P2PEngine::startupEngine error %d bigListMgrStartup\n", rc );
	}

	strDbFileName = VxGetSettingsDirectory();
	strDbFileName += "EngineSettings.db3";
	getEngineSettings().engineSettingsStartup( strDbFileName );

	strDbFileName = VxGetSettingsDirectory();
	strDbFileName += "EngineParams.db3";
	getEngineParams().engineParamsStartup( strDbFileName );

	m_IsUserSpecificDirSet = true;
	m_AppStartupCalled = true;
}

//============================================================================
uint64_t P2PEngine::fromGuiGetDiskFreeSpace( void  ) 
{
	std::string incompleteDir =	VxGetIncompleteDirectory();
	return VxFileUtil::getDiskFreeSpace( incompleteDir.c_str() );
}

//============================================================================
void P2PEngine::fromGuiAppShutdown( void )
{
	VxSetAppIsShuttingDown( true );
    enableTimerThread( false );
	shutdownEngine();
}

//============================================================================
void P2PEngine::fromGuiDebugSettings( uint32_t u32LogFlags, const char * pLogFileName )
{
	VxSetLogFlags( u32LogFlags );
	if( pLogFileName && strlen( pLogFileName ) )
	{
		VxSetLogToFile( pLogFileName );
	}
}

//============================================================================
void P2PEngine::fromGuiSendLog(	uint32_t u32LogFlags )
{
	std::vector<LogEntry> logMsgs;
	VxGetLogMessages( u32LogFlags, logMsgs );
	std::vector<LogEntry>::iterator iter;
	for( iter = logMsgs.begin(); iter != logMsgs.end(); ++iter )
	{
		LogEntry logEntry = (*iter);
		IToGui::getToGui().toGuiLog( logEntry.m_LogFlags, logEntry.m_LogText.c_str() );
	}

	char buf[ 256 ];
	sprintf( buf, "End of log messages count %zd\n", logMsgs.size() );
	IToGui::getToGui().toGuiLog( 0, buf );
}

//============================================================================
void P2PEngine::fromGuiUserLoggedOn( VxNetIdent * netIdent )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiUserLoggedOn" );
	LogMsg( LOG_INFO, "P2PEngine fromGuiUserLoggedOn\n" );
	VxSetMyOnlineId( netIdent->getMyOnlineId() );
	m_AssetMgr.fromGuiUserLoggedOn();

	//LogMsg( LOG_INFO, "P2PEngine fromGuiUserLoggedOn ident Hi 0x%llX Lo 0x%llX\n", 
	//								netIdent->getMyOnlineId().getVxGUIDHiPart(), 
	//								netIdent->getMyOnlineId().getVxGUIDLoPart() );
	
	memcpy( (VxNetIdent *)&m_PktAnn, netIdent, sizeof( VxNetIdent ));
	m_PktAnn.setSrcOnlineId( netIdent->getMyOnlineId() );
    // set network settings from saved settings

	startupEngine();
    updateFromEngineSettings( getEngineSettings() );
	m_PluginMgr.fromGuiUserLoggedOn();
	m_NetworkStateMachine.fromGuiUserLoggedOn();
	LogMsg( LOG_INFO, "P2PEngine fromGuiUserLoggedOn done\n" );
}
//============================================================================
void P2PEngine::updateFromEngineSettings( EngineSettings& engineSettings )
{
    //std::string networkKeyString;
    //std::string netServiceUrl;
    std::string netHostUrl;

    //engineSettings.getNetworkKey( networkKeyString );
    //engineSettings.getConnectTestUrl( netServiceUrl );
    engineSettings.getNetworkHostUrl( netHostUrl );
    // we need to update the globals so accessable everywhere
    std::string webHostName;
    std::string webFileName;
    uint16_t port = 0;
    VxSplitHostAndFile( netHostUrl.c_str(), webHostName, webFileName, port );
    if( !webHostName.empty() )
    {
        VxSetNetworkHostName( webHostName.c_str() );
        VxSetNetworkHostPort( port );
        VxSetNetworkHostUrl( netHostUrl.c_str() );
    }
    else
    {
        LogMsg( LOG_ERROR, "Empty Network Host Name" );
    }

    m_NetworkStateMachine.updateFromEngineSettings( engineSettings );
}

//============================================================================
void P2PEngine::fromGuiOnlineNameChanged( const char * newOnlineName )
{
	m_PktAnn.setOnlineName( newOnlineName );
	doPktAnnHasChanged( false );
}

//============================================================================
void P2PEngine::fromGuiMoodMessageChanged( const char * newMoodMessage )
{
	m_PktAnn.setOnlineDescription( newMoodMessage );
	doPktAnnHasChanged( false );
}

//============================================================================
void P2PEngine::fromGuiIdentPersonalInfoChanged( int age, int gender, int language, int preferredContent )
{
    m_PktAnn.setAge( age );
    m_PktAnn.setGender( gender );
    m_PktAnn.setPrimaryLanguage( language );
    m_PktAnn.setPreferredContent( preferredContent );
    doPktAnnHasChanged( false );
}

//============================================================================
void P2PEngine::fromGuiSetUserHasProfilePicture( bool haveProfilePick )
{
	if( m_PktAnn.hasProfilePicture() != haveProfilePick )
	{
		m_PktAnn.setHasProfilePicture( haveProfilePick );
		doPktAnnHasChanged( false );
	}
}

//============================================================================
void P2PEngine::fromGuiUpdateMyIdent( VxNetIdent * netIdent, bool permissionAndStatsOnly )
{
	if( permissionAndStatsOnly )
	{
		memcpy( m_PktAnn.getPluginPermissions(), netIdent->getPluginPermissions(), PERMISSION_ARRAY_SIZE );
		m_PktAnn.setDareCount( netIdent->getDareCount() );
		m_PktAnn.setTruthCount( netIdent->getTruthCount() );
		m_PktAnn.setRejectedTruthCount( netIdent->getRejectedTruthCount() );
		m_PktAnn.setRejectedDareCount( netIdent->getRejectedDareCount() );
	}
	else
	{
		memcpy( (VxNetIdent *)&m_PktAnn, netIdent, sizeof( VxNetIdent ));
	}

	doPktAnnHasChanged( false );
}

//============================================================================
void P2PEngine::fromGuiSetIdentHasTextOffers( VxGUID& onlineId, bool hasTextOffers )
{
	BigListInfo * bigListInfo = m_BigListMgr.findBigListInfo( onlineId );
	if( bigListInfo )
	{
		if( bigListInfo->getHasTextOffers() != hasTextOffers )
		{
			bigListInfo->setHasTextOffers( hasTextOffers );
			m_BigListMgr.updateBigListDatabase( bigListInfo, getNetworkMgr().getNetworkKey() );
		}
	}
}

//============================================================================
void P2PEngine::fromGuiQueryMyIdent( VxNetIdent * poRetIdent )
{
	memcpy( poRetIdent, (VxNetIdent *)&m_PktAnn, sizeof( VxNetIdent ) );
}

//============================================================================
void P2PEngine::fromGuiAppPause( void )
{
	m_AppIsPaused = true;
	m_PluginMgr.fromGuiAppPause();
}

//============================================================================
void P2PEngine::fromGuiAppResume( void )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiAppResume" );
	m_AppIsPaused = false;
	m_PluginMgr.fromGuiAppResume();
}

//============================================================================
//! idle
void P2PEngine::fromGuiAppIdle( void )
{
	static int64_t lclSysTime = 0;
	int64_t curMs = GetGmtTimeMs( );
	if( false == m_IsUserSpecificDirSet )
	{
		// wait until some things are started
		return;	
	}

	if( lclSysTime / 1000 != curMs / 1000 )
	{
		lclSysTime = curMs;
		onOncePerSecond();
	}
	//else // NOTE: currently have no plugins use fromGuiAppIdle
	//{
	//	m_PluginMgr.fromGuiAppIdle();
	//}
}

//============================================================================
void P2PEngine::fromGuiNetworkAvailable( const char * lclIp, bool isCellularNetwork )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiNetworkAvailable" );
	m_NetworkStateMachine.fromGuiNetworkAvailable( lclIp, isCellularNetwork );
}

//============================================================================
void P2PEngine::fromGuiNetworkLost( void )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiNetworkLost" );
	m_NetworkStateMachine.fromGuiNetworkLost();
}

//============================================================================
ENetLayerState P2PEngine::fromGuiGetNetLayerState( ENetLayerType netLayer )
{
    return m_NetworkStateMachine.fromGuiGetNetLayerState( netLayer );
}

//============================================================================
void P2PEngine::fromGuiNetworkSettingsChanged( void )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiNetworkSettingsChanged" );
	m_NetworkStateMachine.fromGuiNetworkSettingsChanged();
}

//============================================================================
//=== user gui input actions ===//
//============================================================================
//! called after processed HandleOrientationEvent for derived classes to override
bool P2PEngine::fromGuiOrientationEvent( float f32RotX, float f32RotY, float f32RotZ  )
{
	return false;
}

//============================================================================
//! called after processed HandleMouseEvent for derived classes to override
bool P2PEngine::fromGuiMouseEvent( EMouseButtonType eMouseButType, EMouseEventType eMouseEventType, int iMouseXPos, int iMouseYPos  )
{
	return false;
}

//============================================================================
//! called after processed HandleMouseWheel for derived classes to override
bool P2PEngine::fromGuiMouseWheel( float f32MouseWheelDist )
{
	return false;
}

//============================================================================
//! called after processed HandleKeyEvent for derived classes to override
bool P2PEngine::fromGuiKeyEvent( EKeyEventType eKeyEventType, EKeyCode eKey, int iFlags )
{
	return false;
}

//============================================================================
void P2PEngine::fromGuiNativeGlInit( void )
{
}

//============================================================================
//! resize window
void P2PEngine::fromGuiNativeGlResize( int w, int h )
{
}

//============================================================================
//! called to render the next frame
int  P2PEngine::fromGuiNativeGlRender( void )
{
	return 0;
}

//============================================================================
//! called to pause the render loop
void P2PEngine::fromGuiNativeGlPauseRender( void )
{
}

//============================================================================
//! called to resume the render loop
void P2PEngine::fromGuiNativeGlResumeRender( void )
{
}

//============================================================================
//! called when game window is being destroyed
void P2PEngine::fromGuiNativeGlDestroy( void )
{
}

//============================================================================
void P2PEngine::fromGuiMicrophoneDataWithInfo( int16_t * pcmData, int pcmSampleCnt, bool isSilence, int totalDelayTimeMs, int clockDrift )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	m_MediaProcessor.fromGuiMicrophoneDataWithInfo( pcmData, pcmSampleCnt, isSilence, totalDelayTimeMs, clockDrift );
}

//============================================================================
void P2PEngine::fromGuiAudioOutSpaceAvail( int freeSpaceLen )
{
	m_MediaProcessor.fromGuiAudioOutSpaceAvail( freeSpaceLen );
}

//============================================================================
void P2PEngine::fromGuiVideoData( uint32_t u32FourCc, uint8_t * pu8VidDataIn, int iWidth, int iHeight, uint32_t u32VidDataLen, int iRotation )
{
	m_MediaProcessor.fromGuiVideoData( u32FourCc, pu8VidDataIn, iWidth, iHeight, u32VidDataLen, iRotation );
}

//============================================================================
bool P2PEngine::fromGuiSndRecord( ESndRecordState eRecState, VxGUID& feedId, const char * fileName )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiSndRecord" );
	return m_MediaProcessor.getMediaTools().fromGuiSndRecord( eRecState, feedId, fileName );
}

//============================================================================
bool P2PEngine::fromGuiAssetAction( EAssetAction assetAction, AssetInfo& assetInfo, int pos0to100000 )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiAssetAction" );
	if( eAssetActionAddToAssetMgr == assetAction )
	{
		return m_AssetMgr.addAsset( assetInfo );
	}
	else if( eAssetActionAddAssetAndSend == assetAction )
	{
		assetInfo.setAssetSendState( eAssetSendStateTxProgress );
		bool result = m_AssetMgr.addAsset( assetInfo );
        if( false == result )
        {
            LogMsg( LOG_ERROR, "PEngine::fromGuiAssetAction failed to add asset\n" );
        }

		fromGuiSendAsset( assetInfo );
	}
	else if( eAssetActionAssetResend == assetAction )
	{
		assetInfo.setAssetSendState( eAssetSendStateTxProgress );
		IToGui::getToGui().toGuiAssetAction( eAssetActionTxBegin, assetInfo.getAssetUniqueId(), 0 );
		fromGuiSendAsset( assetInfo );
	}
	else if( eAssetActionShreadFile == assetAction )
	{
		bool isFileAsset = assetInfo.isAudioAsset() || assetInfo.isPhotoAsset() || assetInfo.isVideoAsset();
		std::string fileName = assetInfo.getAssetName();
		if( isFileAsset )
		{
			fromGuiSetFileIsShared( fileName.c_str(), false );
			fromGuiAddFileToLibrary( fileName.c_str(), false );	
		}

		m_AssetMgr.removeAsset( assetInfo.getAssetUniqueId() );
		if( isFileAsset )
		{
			GetVxFileShredder().shredFile( fileName );
		}

		return true;
	}
	else if( eAssetActionRemoveFromAssetMgr == assetAction )
	{
		m_AssetMgr.removeAsset( assetInfo.getAssetUniqueId() );
		return true;
	}

	return m_MediaProcessor.getMediaTools().fromGuiAssetAction( assetInfo, assetAction, pos0to100000 );
}

//============================================================================
bool P2PEngine::fromGuiAssetAction( EAssetAction assetAction, VxGUID& assetId, int pos0to100000 )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiAssetAction" );
	if( eAssetActionAddToAssetMgr == assetAction )
	{
		LogMsg( LOG_ERROR, "fromGuiAssetAction Insufficient asset info to add asset\n" );
		return false;
	}

	//TODO figure out lock protection here... should we make copy? would be slow
	AssetInfo * assetInfo = m_AssetMgr.findAsset( assetId );
	if( assetInfo )
	{
		return fromGuiAssetAction( assetAction, *assetInfo, pos0to100000 );
	}

	return false;
}

//============================================================================
bool P2PEngine::fromGuiVideoRecord( EVideoRecordState eRecState, VxGUID& feedId, const char * fileName   )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiVideoRecord" );
	return m_MediaProcessor.getMediaTools().fromGuiVideoRecord( eRecState, feedId, fileName );
}

//! play video or audio file
//============================================================================
bool P2PEngine::fromGuiPlayLocalMedia( const char *  fileName, uint64_t fileLen, uint8_t fileType, int pos0to100000  )
{
    std::string fileNameStr = fileName;
    bool result = true;
    if( !fileNameStr.empty() &&  fileLen && fileType )
    {
        AssetInfo * assetInfo = getAssetMgr().findAsset( fileNameStr );
        if( 0 == assetInfo )
        {
            assetInfo = getAssetMgr().addAssetFile( fileName, fileLen, fileType );
            assetInfo->setPlayPosition( pos0to100000 );
        }

        if( 0 == assetInfo )
        {
            LogMsg( LOG_ERROR, "P2PEngine::fromGuiPlayLocalMedia INVALID PARAM\n" );
            result = false;
        }
        else
        {
            if( eAssetTypeVideo == assetInfo->getAssetType() )
            {
                if( fromGuiIsMyP2PWebVideoFile( fileName ) )
                {
                    fromGuiAssetAction( eAssetActionPlayBegin, assetInfo->getAssetUniqueId(), assetInfo->getPlayPosition() );
                }
                else
                {
                    IToGui::getToGui().playGoTvMedia( assetInfo );
                }
            }
            else if( eAssetTypeAudio == assetInfo->getAssetType() )
            {
                if( fromGuiIsMyP2PWebAudioFile( fileName ) ) 
                {
                    fromGuiAssetAction( eAssetActionPlayBegin, assetInfo->getAssetUniqueId(), assetInfo->getPlayPosition() );
                }
                else
                {
                    IToGui::getToGui().playGoTvMedia( assetInfo );
                }
            }
        }
    }
    else
    {
        LogMsg( LOG_ERROR, "P2PEngine::fromGuiPlayLocalMedia INVALID PARAM\n" );
        result = false;
    }

    return result;
}

//============================================================================
//! update web page profile
void P2PEngine::fromGuiUpdateWebPageProfile(	const char *	pProfileDir,	// directory containing user profile	
												const char *	pGreeting,		// greeting text
												const char *	pAboutMe,		// about me text
												const char *	url1,			// favorite url 1
												const char *	url2,			// favorite url 2
												const char *	url3, 			// favorite url 3
                                                const char *	donation )	    // donation information
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiUpdateWebPageProfile" );
	std::string strWebPageHdr = "";
	StdStringFormat( strWebPageHdr, "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"><html>\
<head><meta http-equiv=\"Content-Type\" content=\"text/html;charset=iso-8859-1\" name=\"description\" content=\"%s :: About Me Web Page - Share introduction information about yourself in a personal web page served on your own device with access allowed or denied by friendship permission level\">\
<FONT COLOR=\"#0000FF\"><title>%s :: About Me</title></FONT></head><body bgcolor=\"#E3EFFF\"><h2><p align=\"center\"><font color=\"#007F00\">%s - %s - About Me</font></p></h2>",
		VxGetApplicationTitle(), VxGetApplicationTitle(), VxGetApplicationNameNoSpaces(), m_PktAnn.getOnlineName() );

	std::string strGreeting = "";
	if( strlen( pGreeting ))
	{
		StdStringFormat( strGreeting, "<h4><p align=\"center\">%s</p></h4>", pGreeting );
	}

	std::string strAbout		= "";
	std::string strPicLabel		= "<h3><p align=\"center\">My Picture</p></h3>";
	std::string strPicture		= "<p align=\"center\"><IMG SRC = \"me.jpg\" width=\"320\" height=\"240\"><br></p>";
	std::string strFavWebsites	= "</h2><p align=\"center\"><font color=\"#007F00\">My Favorite Web Sites</font><br></p></h2>";
	std::string strUrl1			= "";
	std::string strUrl2			= "";
	std::string strUrl3			= "";
    std::string strDonationInfo = "</h2><p align=\"center\"><font color=\"#007F00\">Donation Information</font><br></p></h2>";
    std::string strDonation = "";

	if( 0 !=  strlen( pGreeting ) )
	{
		StdStringFormat( strGreeting, "<h4><p align=\"center\">%s</p></h4>", pGreeting );    		
	}

	if( 0 !=  strlen( pAboutMe ) )
	{
		StdStringFormat( strAbout,  "<h4><p align=\"center\">%s</p></h4>", pAboutMe );
	}

	if( 0 !=  strlen( url1 ) )
	{
		StdStringFormat( strUrl1, "<h5><p align=\"center\"><font color=\"#0000FF\"><a href=\"%s\">%s</a></font><br></p><h5>", url1, url1 );
	}

	if( 0 !=  strlen( url2 ) )
	{
		StdStringFormat( strUrl2, "<h5><p align=\"center\"><font color=\"#0000FF\"><a href=\"%s\">%s</a></font><br></p><h5>", url2, url2 );
	}

	if( 0 !=  strlen( url3 ) )
	{
		StdStringFormat( strUrl3, "<h5><p align=\"center\"><font color=\"#0000FF\"><a href=\"%s\">%s</a></font><br></p><h5>", url3, url3 );
	}

    if( 0 != strlen( donation ) )
    {
        StdStringFormat( strDonation, "<h4><p align=\"center\">%s</p></h4>", donation );
    }


	std::string strTrailer = "</body></html>\r\r\r";

	std::string strWebPage;
    strWebPage = strWebPageHdr + strGreeting + strAbout + strPicLabel + strPicture + strFavWebsites + strUrl1 + strUrl2 + strUrl3 + strDonation + strTrailer;

	std::string strWebFile = pProfileDir;
	strWebFile += "index.htm";

	VxFileUtil::writeWholeFile( strWebFile.c_str(), (void *)strWebPage.c_str(), (uint32_t)(strWebPage.length() + 1) );
}

//============================================================================
void P2PEngine::fromGuiStartPluginSession( EPluginType ePluginType, VxGUID& oOnlineId, int pvUserData, VxGUID lclSessionId )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiStartPluginSession" );
	m_PluginMgr.fromGuiStartPluginSession( ePluginType, oOnlineId, pvUserData, lclSessionId );
}

//============================================================================
void P2PEngine::fromGuiStopPluginSession( EPluginType ePluginType, VxGUID& oOnlineId, int pvUserData, VxGUID lclSessionId )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiStopPluginSession" );
	m_PluginMgr.fromGuiStopPluginSession( ePluginType, oOnlineId, pvUserData, lclSessionId );
}

//============================================================================
bool P2PEngine::fromGuiIsPluginInSession( EPluginType ePluginType, VxNetIdent * netIdent, int pvUserData, VxGUID lclSessionId )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiIsPluginInSession" );
	if( ( false == m_IsUserSpecificDirSet ) || VxIsAppShuttingDown() )
	{
		// wait until some things are started
		return false;	
	}

	return m_PluginMgr.fromGuiIsPluginInSession( ePluginType, netIdent, pvUserData, lclSessionId );
}

//============================================================================
void P2PEngine::fromGuiSetPluginPermission( EPluginType ePluginType, EFriendState eFriendState )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiSetPluginPermission" );
	EFriendState eCurFriendState = m_PktAnn.getPluginPermission( ePluginType );
	if( eCurFriendState != eFriendState )
	{
		m_PluginMgr.setPluginPermission( ePluginType, eFriendState );
		m_PktAnn.setPluginPermission( ePluginType, eFriendState );
		doPktAnnHasChanged( false );
	}
}

//============================================================================
int P2PEngine::fromGuiGetPluginPermission( EPluginType ePluginType )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiGetPluginPermission" );
	return m_PktAnn.getPluginPermission( ePluginType );
}

//============================================================================
EPluginServerState P2PEngine::fromGuiGetPluginServerState( EPluginType ePluginType )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiGetPluginServerState" );
	if( eFriendStateIgnore == m_PktAnn.getPluginPermission( ePluginType ) )
	{
		return ePluginServerStateDisabled;
	}

	return m_PluginMgr.fromGuiIsPluginInSession( ePluginType ) ? ePluginServerStateStarted : ePluginServerStateStopped;
}

//============================================================================
//! called with offer to create session.. return false if cannot connect
bool P2PEngine::fromGuiMakePluginOffer(	EPluginType		ePluginType, 
										VxGUID&			oOnlineId, 
										int				pvUserData,
										const char *	pOfferMsg, 
										const char *	pFileName,
										uint8_t *			fileHashId,
										VxGUID			lclSessionId )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiMakePluginOffer" );
	BigListInfo * poInfo = m_BigListMgr.findBigListInfo( oOnlineId );
	if( poInfo )
	{
		PluginBase * poPlugin = m_PluginMgr.getPlugin( ePluginType );
		if( 0 != poPlugin )
		{
			return poPlugin->fromGuiMakePluginOffer( poInfo, pvUserData, pOfferMsg, pFileName, fileHashId, lclSessionId );
		}
		else
		{
			LogMsg( LOG_ERROR, "ERROR P2PEngine::fromGuiMakePluginOffer invalid plugin\n" );
			return false;
		}
	}
	else
	{
		std::string onlineId;
		oOnlineId.toHexString(onlineId);
		LogMsg(LOG_ERROR, "P2PEngine::fromGuiMakePluginOffer: poInfo not found VxGUID %s\n", onlineId.c_str());
	}

	return false;
}

//============================================================================
//! handle reply to offer
bool P2PEngine::fromGuiToPluginOfferReply(	EPluginType		ePluginType,
											VxGUID&			oOnlineId, 
											int				pvUserData,
											int				iOfferResponse,
											VxGUID			lclSessionId )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiToPluginOfferReply" );
	if( VxIsAppShuttingDown() )
	{
		return false;
	}

	BigListInfo * poInfo = m_BigListMgr.findBigListInfo( oOnlineId );
	PluginBase * poPlugin = m_PluginMgr.getPlugin( ePluginType );
	if( ( 0 != poPlugin )
		&& ( 0 != poInfo ) )
	{
		return poPlugin->fromGuiOfferReply( poInfo, pvUserData, (EOfferResponse)iOfferResponse, lclSessionId );
	}
	else
	{
		LogMsg( LOG_ERROR, "ERROR P2PEngine::fromGuiToPluginOfferReply invalid plugin or info\n" );
		return false;
	}
}

//============================================================================
int P2PEngine::fromGuiPluginControl(	EPluginType		ePluginType, 
										VxGUID&			oOnlineId, 
										const char *	pControl, 
										const char *	pAction,
										uint32_t				u32ActionData,
										VxGUID&			lclSessionId,
										uint8_t *			fileHashId )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiPluginControl" );
	BigListInfo * poInfo = m_BigListMgr.findBigListInfo( oOnlineId );
	PluginBase * poPlugin = m_PluginMgr.getPlugin( ePluginType );
	if( 0 != poPlugin )
	{
		return poPlugin->fromGuiPluginControl( poInfo, pControl, pAction, u32ActionData, lclSessionId, fileHashId );
	}
	else
	{
		LogMsg( LOG_ERROR, "ERROR P2PEngine::fromGuiPluginControl invalid plugin\n" );
		return 0;
	}
}

//============================================================================
bool P2PEngine::fromGuiInstMsg(	EPluginType		ePluginType, 
								VxGUID&			oOnlineId, 
								const char *	pMsg )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiInstMsg" );
	BigListInfo * poInfo = m_BigListMgr.findBigListInfo( oOnlineId );
	PluginBase * poPlugin = m_PluginMgr.getPlugin( ePluginType );
	if( 0 != poPlugin )
	{
		return poPlugin->fromGuiInstMsg( poInfo, pMsg );
	}
	else
	{
		LogMsg( LOG_ERROR, "ERROR P2PEngine::fromGuiInstMsg invalid plugin\n" );
		return false;
	}
}

//============================================================================
bool P2PEngine::isSystemPlugin( EPluginType	ePluginType )
{
	//assureUserSpecificDirIsSet( "P2PEngine::isSystemPlugin" );
	if( ( ePluginTypeInvalid < ePluginType ) && ( eMaxUserPluginType > ePluginType ) )
	{
		return false;
	}

	return true;
}

//============================================================================
bool P2PEngine::isP2POnline( void )
{ 
	//assureUserSpecificDirIsSet( "P2PEngine::isP2POnline" );
	return m_NetworkStateMachine.isP2POnline(); 
}

//============================================================================
bool P2PEngine::getHasHostService( EHostServiceType hostService )
{
    switch( hostService )
    {
    case eHostServiceNetworkHost: return m_PktAnn.getPluginPermission( ePluginTypeHostNetwork ) != eFriendStateIgnore;
    case eHostServiceGroupListing: return m_PktAnn.getPluginPermission( ePluginTypeHostNetwork ) != eFriendStateIgnore;
    case eHostServiceGroup: return m_PktAnn.getPluginPermission( ePluginTypeHostNetwork ) != eFriendStateIgnore;
    case eHostServiceRelay: return m_PktAnn.getPluginPermission( ePluginTypeHostNetwork ) != eFriendStateIgnore;
    case eHostServiceRandomConnect: return m_PktAnn.getPluginPermission( ePluginTypeHostNetwork ) != eFriendStateIgnore;
    case eHostServiceRandomConnectRelay: return m_PktAnn.getPluginPermission( ePluginTypeHostNetwork ) != eFriendStateIgnore;
    case eHostServiceConnectTest: return m_PktAnn.getPluginPermission( ePluginTypeHostNetwork ) != eFriendStateIgnore;
    default:
        break;
    }

    return false;
}

//============================================================================
void P2PEngine::fromGuiMuteMicrophone(	bool muteMic )
{
	m_MediaProcessor.muteMicrophone( muteMic );
}

//============================================================================
bool P2PEngine::fromGuiIsMicrophoneMuted( void )
{
	return m_MediaProcessor.isMicrophoneMuted();
}

//============================================================================
void P2PEngine::fromGuiMuteSpeaker(	bool muteSpeaker )
{
	m_MediaProcessor.muteSpeaker( muteSpeaker );
}

//============================================================================
bool P2PEngine::fromGuiIsSpeakerMuted( void )
{
	return m_MediaProcessor.isSpeakerMuted();
}

//============================================================================
void P2PEngine::fromGuiEchoCancelEnable( bool enableEchoCancel )
{
	m_MediaProcessor.fromGuiEchoCancelEnable( enableEchoCancel );
}

//============================================================================
bool P2PEngine::fromGuiIsEchoCancelEnabled( void )
{
	return m_MediaProcessor.fromGuiIsEchoCancelEnabled();
}

//============================================================================
void P2PEngine::fromGuiWantMediaInput( EMediaInputType mediaType, MediaCallbackInterface * callback, void * userData, bool wantInput )
{
	if( false == VxIsAppShuttingDown() )
	{
		m_MediaProcessor.wantMediaInput( mediaType, callback, userData, wantInput );
	}
}

//============================================================================
void P2PEngine::fromGuiWantMediaInput( VxGUID& onlineId, EMediaInputType mediaType, bool wantInput )
{
	if( false == VxIsAppShuttingDown() )
	{
		BigListInfo * poInfo = m_BigListMgr.findBigListInfo( onlineId );
		if( poInfo )
		{
			if( ( eMediaInputVideoJpgSmall !=  mediaType ) // no need to activate cam if requesting other person's video feed
				&& ( eMediaInputVideoJpgBig !=  mediaType ) )
			{			
				m_MediaProcessor.wantMediaInput( mediaType, this, (VxNetIdent *)poInfo, wantInput );
			}
		}
		else
		{
			m_MediaProcessor.wantMediaInput( mediaType, this, (VxNetIdent *)&m_PktAnn, wantInput );
		}
	}
}

//============================================================================
bool P2PEngine::fromGuiChangeMyFriendshipToHim(	VxGUID&			oOnlineId, 
											   EFriendState		eMyFriendshipToHim,
											   EFriendState		eHisFriendshipToMe )
{
	if( false == VxIsAppShuttingDown() )
	{
		//assureUserSpecificDirIsSet( "P2PEngine::fromGuiChangeMyFriendshipToHim" );
		BigListInfo * poInfo = m_BigListMgr.findBigListInfo( oOnlineId );
		if( poInfo )
		{
			EFriendState eOldFriendship = poInfo->getMyFriendshipToHim();
			poInfo->setMyFriendshipToHim( eMyFriendshipToHim );
			m_BigListMgr.updateVectorList( eOldFriendship, poInfo );
			m_BigListMgr.updateBigListDatabase( poInfo, getNetworkMgr().getNetworkKey() );
			LogMsg(LOG_INFO, "P2PEngine::fromGuiChangeMyFriendshipToHim: SUCCESS changed %s friendship to %s\n", 
				poInfo->getOnlineName(),
				poInfo->describeMyFriendshipToHim());
			m_ConnectionList.fromGuiChangeMyFriendshipToHim(	oOnlineId, 
																eMyFriendshipToHim,
																eHisFriendshipToMe );
			return true;
		}
		else
		{
			LogMsg(LOG_INFO, "P2PEngine::fromGuiChangeMyFriendshipToHim: FAILED find friend 0x%llX 0x%llX\n",
				oOnlineId.getVxGUIDHiPart(),
				oOnlineId.getVxGUIDLoPart() );
		}
	}

	return false;
}

//============================================================================
//! network test failed.. requires proxy service from friend
void P2PEngine::fromGuiRequireRelay( bool bRequireRelay )
{
	if( false == VxIsAppShuttingDown() )
	{
		//assureUserSpecificDirIsSet( "P2PEngine::fromGuiRequireRelay" );
		if( m_PktAnn.requiresRelay() != bRequireRelay )
		{
			 m_PktAnn.setRequiresRelay( bRequireRelay );
			 doPktAnnHasChanged( false );
		}
	}
}

//============================================================================
//! request proxy service from this contact
void P2PEngine::fromGuiUseRelay( VxGUID& oOnlineId, bool bUseAsRelay ) 
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiUseRelay" );
	BigListInfo * poInfo = m_BigListMgr.findBigListInfo( oOnlineId );
	if( poInfo )
	{
		poInfo->setIsMyPreferedRelay( bUseAsRelay );
		m_BigListMgr.updateBigListDatabase( poInfo, getNetworkMgr().getNetworkKey() );
	}
}

//============================================================================
void P2PEngine::fromGuiRelayPermissionCount( int userPermittedCount, int anonymousCount )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiRelayPermissionCount" );
	m_PluginMgr.fromGuiRelayPermissionCount( userPermittedCount, anonymousCount );
}

//============================================================================
InetAddress P2PEngine::fromGuiGetMyIPv4Address( void )
{
	return VxGetDefaultIPv4Address();
}

//============================================================================
InetAddress P2PEngine::fromGuiGetMyIPv6Address( void )
{
	return VxGetMyGlobalIPv6Address();
}

//============================================================================
void P2PEngine::fromGuiUserModifiedStoryboard( void )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiUserModifiedStoryboard" );
	m_PktAnn.setHasModifiedStoryboard( true );
	doPktAnnHasChanged( false );
}

//============================================================================
void P2PEngine::fromGuiCancelDownload( VxGUID& fileInstance )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiCancelDownload" );
	m_PluginMgr.getPlugin(ePluginTypeFileXfer)->fromGuiCancelDownload( fileInstance );
	m_PluginMgr.getPlugin(ePluginTypeFileServer)->fromGuiCancelDownload( fileInstance );
}

//============================================================================
void P2PEngine::fromGuiCancelUpload( VxGUID& fileInstance )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiCancelUpload" );
	m_PluginMgr.getPlugin(ePluginTypeFileXfer)->fromGuiCancelUpload( fileInstance );
	m_PluginMgr.getPlugin(ePluginTypeFileServer)->fromGuiCancelUpload( fileInstance );
}

//============================================================================
void P2PEngine::fromGuiGetNetSettings( NetSettings& netSettings )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiGetNetSettings" );
	m_EngineSettings.getNetSettings( netSettings );
}

//============================================================================
void P2PEngine::fromGuiSetNetSettings( NetSettings& netSettings )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiSetNetSettings" );
	m_EngineSettings.setNetSettings( netSettings );
	PluginBase * pluginBase = m_PluginMgr.findPlugin( ePluginTypeRelay );
	if( pluginBase )
	{
		((PluginServiceRelay *)pluginBase)->fromGuiRelayPermissionCount( netSettings.getUserRelayPermissionCount(), netSettings.getSystemRelayPermissionCount() ); 
	}
}

//============================================================================
void P2PEngine::fromGuiSetRelaySettings( int userRelayMaxCnt, int systemRelayMaxCnt )
{
	m_EngineSettings.setMaxRelaysInUse( userRelayMaxCnt, systemRelayMaxCnt );
	PluginBase * pluginBase = m_PluginMgr.findPlugin( ePluginTypeRelay );
	if( pluginBase )
	{
		((PluginServiceRelay *)pluginBase)->fromGuiRelayPermissionCount( userRelayMaxCnt, systemRelayMaxCnt ); 
	}
}

//============================================================================
void P2PEngine::fromGuiGetFileShareSettings( FileShareSettings& fileShareSettings )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiGetFileShareSettings" );
	m_PluginMgr.getPlugin(ePluginTypeFileServer)->fromGuiGetFileShareSettings( fileShareSettings );
}

//============================================================================
void P2PEngine::fromGuiSetFileShareSettings( FileShareSettings& fileShareSettings )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiSetFileShareSettings" );
	m_PluginMgr.getPlugin(ePluginTypeFileServer)->fromGuiSetFileShareSettings( fileShareSettings );
}

//============================================================================
bool P2PEngine::fromGuiSetGameValueVar(	EPluginType	ePluginType, 
										VxGUID&		onlineId, 
										int32_t			varId, 
										int32_t			varValue )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiSetGameValueVar" );
	PluginBase * poPlugin = m_PluginMgr.getPlugin( ePluginType );
	BigListInfo * poInfo = m_BigListMgr.findBigListInfo( onlineId );
	if( poInfo )
	{
		return poPlugin->fromGuiSetGameValueVar( poInfo, varId, varValue );
	}
	else
	{
		LogMsg( LOG_ERROR, "P2PEngine::fromGuiSetGameValueVar: could not locate idenitiy\n");
	}

	return false;
}

//============================================================================
bool P2PEngine::fromGuiSetGameActionVar(	EPluginType	ePluginType, 
											VxGUID&		onlineId, 
											int32_t			varId, 
											int32_t			varValue )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiSetGameActionVar" );
	PluginBase * poPlugin = m_PluginMgr.getPlugin( ePluginType );
	BigListInfo * poInfo = m_BigListMgr.findBigListInfo( onlineId );
	if( poInfo )
	{
		return poPlugin->fromGuiSetGameActionVar( poInfo, varId, varValue );
	}
	else
	{
		LogMsg( LOG_ERROR, "P2PEngine::fromGuiSetGameActionVar: could not locate idenitiy\n");
	}

	return false;
}

//============================================================================
bool P2PEngine::fromGuiTestCmd(	ETestParam1		eTestParam1, 
								int				testParam2, 
								const char *	testParam3 )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiTestCmd" );
	bool result = false;
	switch( eTestParam1 )
	{
	case eTestParam1FullNetTest1:
		{
			//NetSettings netSettings;
			//m_EngineSettings.getNetSettings( netSettings );
			//fromGuiTestNetwork( netSettings );
		}
		break;

	case eTestParam1WhatsMyIp:
		{
			//void						queryWhatsMyIp( void );
		}
		break;

	case eTestParam1IsMyPortOpen:
		{
			//m_PluginNetServices.testIsMyPortOpen();
		}
		break;

	case eTestParam1AnnounceNow:
		{
			m_NetServicesMgr.announceToHost( m_NetworkStateMachine.getHostIp(), m_NetworkStateMachine.getHostPort() );
		}
		break;

	case eTestParamSoundDelayTest:
		{
			m_MediaProcessor.fromGuiSoundDelayTest();
		}
		break;

	default:
		LogMsg( LOG_INFO, "Unknown eTestParam1 %d\n", eTestParam1 );
	}

	return result;
}

//============================================================================
uint16_t P2PEngine::fromGuiGetRandomTcpPort( void )
{
	return VxGetRandomTcpPort();
}

/// Get url for this node
//============================================================================
void P2PEngine::fromGuiGetNodeUrl( std::string& nodeUrl )
{
    return getNetStatusAccum().getNodeUrl( nodeUrl );
}

//============================================================================
/// Get internet status
EInternetStatus P2PEngine::fromGuiGetInternetStatus( void )
{
    return getNetStatusAccum().getInternetStatus();
}

//============================================================================
/// Get network status
ENetAvailStatus P2PEngine::fromGuiGetNetAvailStatus( void )
{
    return getNetStatusAccum().getNetAvailStatus();
}

//============================================================================
#ifdef TARGET_OS_ANDROID
int P2PEngine::fromGuiMulitcastPkt( unsigned char * data, int len )
{
	LogMsg( LOG_INFO, "fromGuiMulitcastPkt len %d\n", len );
	return 0;
}
#endif // TARGET_OS_ANDROID

//============================================================================
void P2PEngine::fromGuiVerifyNetHostSettings( void )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiVerifyNetHostSettings" );
	m_HostTest.fromGuiVerifyNetHostSettings();
}

//============================================================================
void P2PEngine::fromGuiRunIsPortOpenTest( uint16_t port )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiRunIsPortOpenTest" );
	m_IsPortOpenTest.fromGuiRunIsPortOpenTest( port );
}

//============================================================================
void P2PEngine::fromGuiRunQueryHostIdTest( void )
{
    //assureUserSpecificDirIsSet( "P2PEngine::fromGuiRunIsPortOpenTest" );
    m_QueryHostIdTest.fromGuiRunQueryHostIdTest();
}

//============================================================================
bool P2PEngine::fromGuiBrowseFiles(	const char * dir, bool lookupShareStatus, uint8_t fileFilterMask )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiBrowseFiles" );
	return getPluginServiceFileShare().fromGuiBrowseFiles(	dir, lookupShareStatus, fileFilterMask );
}

//============================================================================
bool P2PEngine::fromGuiGetSharedFiles( uint8_t fileTypeFilter )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiGetSharedFiles" );
	return getPluginServiceFileShare().fromGuiGetSharedFiles( fileTypeFilter );
}

//============================================================================
bool P2PEngine::fromGuiSetFileIsShared( const char * fileName, bool isShared, uint8_t * fileHashId )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiSetFileIsShared" );
	return getPluginServiceFileShare().fromGuiSetFileIsShared(	fileName, isShared, fileHashId );
}

//============================================================================
bool P2PEngine::fromGuiGetIsFileShared( const char * fileName )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiGetIsFileShared" );
	return getPluginServiceFileShare().fromGuiGetIsFileShared(	fileName );
}

//============================================================================
// returns -1 if unknown else percent downloaded
int P2PEngine::fromGuiGetFileDownloadState( uint8_t * fileHashId )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiGetFileDownloadState" );
	return getPluginServiceFileShare().fromGuiGetFileDownloadState( fileHashId );
}

//============================================================================
bool P2PEngine::fromGuiAddFileToLibrary( const char * fileName, bool addFile, uint8_t * fileHashId )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiAddFileToLibrary" );
	return getPluginServiceFileShare().fromGuiAddFileToLibrary( fileName, addFile, fileHashId );
}

//============================================================================
void P2PEngine::fromGuiGetFileLibraryList( uint8_t fileTypeFilter )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiGetFileLibraryList" );
	getPluginServiceFileShare().fromGuiGetFileLibraryList(	fileTypeFilter );
}

//============================================================================
bool P2PEngine::fromGuiGetIsFileInLibrary( const char * fileName )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiGetIsFileInLibrary" );
	return getPluginServiceFileShare().fromGuiGetIsFileInLibrary( fileName );
}

//============================================================================
bool P2PEngine::fromGuiIsMyP2PWebVideoFile( const char * fileName )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiIsMyP2PWebVideoFile" );
	return m_MediaProcessor.getMediaTools().fromGuiIsMyP2PWebVideoFile( fileName );
}

//============================================================================
bool P2PEngine::fromGuiIsMyP2PWebAudioFile( const char * fileName )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiIsMyP2PWebAudioFile" );
	return m_MediaProcessor.getMediaTools().fromGuiIsMyP2PWebAudioFile( fileName );
}

//============================================================================
int P2PEngine::fromGuiDeleteFile( const char * fileName, bool shredFile )
{
	int result = 0;
	FILE * fileHandle = fopen( fileName, "rb+" );
	if( fileHandle )
	{
		fclose( fileHandle );
	}
	else
	{
		result = VxGetLastError();
		if( 0 == result )
		{
			result = -1;
		}
	}

	m_PluginMgr.fromGuiDeleteFile( fileName, shredFile );

	getPluginServiceFileShare().deleteFile( fileName, shredFile );	

	return result;
}

//============================================================================
void P2PEngine::fromGuiQuerySessionHistory( VxGUID& historyId )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiQuerySessionHistory" );
	m_AssetMgr.queryHistoryAssets( historyId );
}

//============================================================================
bool P2PEngine::fromGuiSendAsset( AssetInfo& assetInfo )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiSendAsset" );
	m_PluginMgr.fromGuiSendAsset( assetInfo );
	return true;
}

//============================================================================
bool P2PEngine::fromGuiMultiSessionAction( EMSessionAction mSessionAction, VxGUID& onlineId, int pos0to100000, VxGUID lclSessionId )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiMultiSessionAction" );
	return m_PluginMgr.fromGuiMultiSessionAction( mSessionAction, onlineId, pos0to100000, lclSessionId );
}


//============================================================================
// Copyright (C) 2022 Brett R. Jones
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

#include "GuiPlayerMgr.h"

#include "ActivityBase.h"
#include "AppCommon.h"
#include "GuiHelpers.h"
#include "GuiParams.h"
#include "GuiPlayerCallback.h"

#include <ptop_src/ptop_engine_src/AssetMgr/AssetInfo.h>

#include <CoreLib/VxFileUtil.h>

//============================================================================
GuiPlayerMgr::GuiPlayerMgr( AppCommon& app )
	: QObject( &app )
	, m_MyApp( app )
{
}

//============================================================================
void GuiPlayerMgr::playerMgrStartup( void )
{
	connect( this, SIGNAL( signalInternalPlayVideoFrame(VxGUID,QImage) ), this, SLOT( slotInternalPlayVideoFrame(VxGUID,QImage) ), Qt::QueuedConnection );
	connect( this, SIGNAL( signalInternalPlayMotionVideoFrame(VxGUID,QImage,int) ), this, SLOT( slotInternalPlayMotionVideoFrame(VxGUID,QImage,int) ), Qt::QueuedConnection );
}

//============================================================================
void GuiPlayerMgr::wantPlayVideoCallbacks( GuiPlayerCallback* client, bool enable )
{
	if( m_VideoPlayClientsBusy )
	{
		LogMsg( LOG_ERROR, "GuiPlayerMgr::wantPlayVideoCallbacks do NOT call while busy" );
	}

	for( auto iter = m_VideoPlayClients.begin(); iter != m_VideoPlayClients.end(); ++iter )
	{
		if( *iter == client )
		{
			m_VideoPlayClients.erase( iter );
			break;
		}
	}

	if( enable )
	{
		m_VideoPlayClients.push_back( client );
	}
}

//============================================================================
void GuiPlayerMgr::toGuiPlayVideoFrame( VxGUID& feedOnlineId, uint8_t* pu8Jpg, uint32_t u32JpgDataLen, int motion0To100000 )
{
	// there seems to be an issue where a QImage may get replaced if there is already one in a queued signal/slot 
	// only allow 1 at a time
	int behindFramCnt = m_BehindFrameCnt;
	if( behindFramCnt )
	{
		return;
	}

	QImage vidFrame;
	if( behindFramCnt < 4 && vidFrame.loadFromData( pu8Jpg, u32JpgDataLen, "JPG" ) )
	{
		m_BehindFrameCnt++;
		emit signalInternalPlayMotionVideoFrame( feedOnlineId, vidFrame, motion0To100000 );
	}
	else
	{
		LogMsg( LOG_WARNING, "GuiPlayerMgr::toGuiPlayVideoFrame skipping frame %d", behindFramCnt );
	}
}

//============================================================================
void GuiPlayerMgr::slotInternalPlayMotionVideoFrame( VxGUID feedOnlineId, QImage vidFrame, int motion0To100000 )
{
	m_BehindFrameCnt--;
	m_VideoPlayClientsBusy = true;
	for( auto guiVidCallback : m_VideoPlayClients )
	{
		guiVidCallback->callbackGuiPlayMotionVideoFrame( feedOnlineId, vidFrame, motion0To100000 );
	}

	m_VideoPlayClientsBusy = false;
}

//============================================================================
int GuiPlayerMgr::toGuiPlayVideoFrame( VxGUID& feedOnlineId, uint8_t* picBuf, uint32_t picBufLen, int picWidth, int picHeight )
{
	// there seems to be an issue where a QImage may get replaced if there is already one in a queued signal/slot 
// only allow 1 at a time
	int behindFramCnt = m_BehindFrameCnt;
	if( behindFramCnt )
	{
		return behindFramCnt;
	}

	QImage frameImage;
	if( behindFramCnt < 4 && m_VideoPlayClients.size() )
	{
		if( picBuf
			&& (picWidth > 10)
			&& (picHeight > 10)
			&& (picBufLen = picWidth * picHeight * 4) )
		{
			QImage::Format imageFormat = QImage::Format_ARGB32;
			//if( picBufLen = ( picWidth * picHeight * 4) )
			//{
			//    imageFormat =  QImage::Format_ARGB32_Premultiplied;
			//}

			QImage	frameImage( picBuf, picWidth, picHeight, imageFormat );
			if( !frameImage.isNull() )
			{
				int behindFrameCnt = m_BehindFrameCnt;
				LogMsg( LOG_INFO, " GuiPlayerMgr::playVideoFrame len %d behind %d", picBufLen, behindFrameCnt );
				m_BehindFrameCnt++;
				emit signalInternalPlayVideoFrame( feedOnlineId, frameImage );
			}
			else
			{
				LogMsg( LOG_ERROR, " GuiPlayerMgr::playVideoFrame invalid image data" );
			}
		}
		else
		{
			LogMsg( LOG_ERROR, " GuiPlayerMgr::playVideoFrame invalid data params" );
		}

	}
	else
	{
		LogMsg( LOG_WARNING, "GuiPlayerMgr::toGuiPlayVideoFrame2 skipping frame %d", behindFramCnt );
	}

	return behindFramCnt;
}

//============================================================================
void GuiPlayerMgr::slotInternalPlayVideoFrame( VxGUID feedOnlineId, QImage vidFrame )
{
	m_BehindFrameCnt--;
	m_VideoPlayClientsBusy = true;
	for( auto guiVidCallback : m_VideoPlayClients )
	{
		guiVidCallback->callbackGuiPlayVideoFrame( feedOnlineId, vidFrame );
	}

	m_VideoPlayClientsBusy = false;
}

//============================================================================
bool GuiPlayerMgr::playFile( QString fullFileName, int pos0to100000 )
{
	if( fullFileName.isEmpty() )
	{
		LogMsg( LOG_WARNING, "GuiPlayerMgr::playFile Empty File Name" );
		return false;
	}

	uint8_t fileType;
	uint64_t fileLen;
	if( !VxFileUtil::getFileTypeAndLength( fullFileName.toUtf8().constData(), fileLen, fileType ) )
	{
		LogMsg( LOG_WARNING, "File no longer available %s", fullFileName.toUtf8().constData() );
		return false;
	}

	AssetInfo newAsset( GuiParams::fileTypeToAssetType( fileType ), fullFileName.toUtf8().constData(), fileLen );
	return playMedia( newAsset );
}

//============================================================================
bool GuiPlayerMgr::playMedia( AssetBaseInfo& assetInfo, int pos0to100000 )
{
	if( eAssetTypeExe == assetInfo.getAssetType() )
	{
		QMessageBox::warning( m_MyApp.getCentralWidget(), QObject::tr( "Attempted to play an executable which is not allowed" ), QString(assetInfo.getAssetName().c_str()) );
		return false;
	}

	if( eAssetTypeArchives == assetInfo.getAssetType() )
	{
		QMessageBox::warning( m_MyApp.getCentralWidget(), QObject::tr( "Attempted to open an archive file which is not allowed" ), QString( assetInfo.getAssetName().c_str() ) );
		return false;
	}

	EApplet appletType = GuiHelpers::getAppletThatPlaysFile( m_MyApp, assetInfo );
	if( appletType != eAppletUnknown )
	{
		// launch the applet that plays this file
		ActivityBase* applet = m_MyApp.launchApplet( appletType, m_MyApp.getCentralWidget(), "", assetInfo.getAssetUniqueId() );
		if( applet && applet->playMedia( assetInfo, pos0to100000 ) )
		{
			return true;
		}
	}

#ifdef TARGET_OS_WINDOWS
	ShellExecuteA( 0, 0, assetInfo.getAssetName().c_str(), 0, 0, SW_SHOW );
#else
    QDesktopServices::openUrl( QUrl::fromLocalFile( assetInfo.getAssetName().c_str() ) );
#endif // TARGET_OS_WINDOWS
	return true;
}

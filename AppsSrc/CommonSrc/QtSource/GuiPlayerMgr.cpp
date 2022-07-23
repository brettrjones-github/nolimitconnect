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
#include "GuiPlayerCallback.h"

#include "AppCommon.h"

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
	QImage vidFrame;
    int behindFramCnt = m_BehindFrameCnt;
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
	for( auto guiVidCallback : m_VideoPlayClients )
	{
		guiVidCallback->callbackGuiPlayMotionVideoFrame( feedOnlineId, vidFrame, motion0To100000 );
	}
}

//============================================================================
int GuiPlayerMgr::toGuiPlayVideoFrame( VxGUID& feedOnlineId, uint8_t* picBuf, uint32_t picBufLen, int picWidth, int picHeight )
{
	QImage frameImage;
	int behindFramCnt = m_BehindFrameCnt;
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
	for( auto guiVidCallback : m_VideoPlayClients )
	{
		guiVidCallback->callbackGuiPlayVideoFrame( feedOnlineId, vidFrame );
	}
}

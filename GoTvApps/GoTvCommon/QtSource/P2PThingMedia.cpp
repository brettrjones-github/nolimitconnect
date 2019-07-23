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
// http://www.gotvptop.com
//============================================================================

#include "AppCommon.h"
#include "AppSettings.h"

#include "ActivityScanWebCams.h"
#include "ToGuiActivityInterface.h"
#include "ToGuiHardwareControlInterface.h"

#include <VxVideoLib/VxVideoLib.h>
#include <CoreLib/VxGlobals.h>

#include <QTimer>
#include <QMessageBox>

//============================================================================
void AppCommon::toGuiWantVideoCapture( bool wantVidCapture )
{
	LogMsg( LOG_INFO, "#### AppCommon::slotEnableVideoCapture %d\n", wantVidCapture );
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	emit signalEnableVideoCapture( wantVidCapture );
}

//============================================================================
void AppCommon::slotEnableVideoCapture( bool enableVidCapture )
{
	if( enableVidCapture )
	{
		static bool bFirstTimeVideoCaptureStarted = true;
		if( bFirstTimeVideoCaptureStarted )
		{
			if( NULL == m_VidCapTimer )
			{
				m_VidCapTimer = new QTimer(this);
				m_VidCapTimer->setInterval(50); // we want about 20 frames per second 1000/20 = 50ms
				connect(m_VidCapTimer, SIGNAL(timeout()), this, SLOT(onVidCapTimer()));
#ifdef TARGET_OS_WINDOWS
				m_VidCap = VxGetVidCapInterface();
#endif // TARGET_OS_WINDOWS
			}

			uint32_t iDevices = m_VidCap->startupVidCap();
			if( 0 == iDevices )
			{
				QMessageBox::warning(this, tr("Web Cam Video"), tr("No Video Capture Devices Found" ) );
				return;
			}
			
			m_CamSourceId = m_AppSettings.getCamSourceId();
			if( m_CamSourceId > iDevices )
			{
				m_CamSourceId = 1;
				m_AppSettings.setCamSourceId( m_CamSourceId );
			}

			setCamCaptureRotation( m_AppSettings.getCamRotation( m_CamSourceId ) );

			bFirstTimeVideoCaptureStarted = false;
		}

		if( false == m_VidCapTimer->isActive() )
		{
			LogMsg( LOG_INFO, "AppCommon::slotEnableVideoCapture start capture\n" );
			m_VidCapTimer->start();
		}
	}
	else
	{
		if( m_VidCapTimer &&
			m_VidCapTimer->isActive() )
		{
			LogMsg( LOG_INFO, "AppCommon::slotEnableVideoCapture stopping capture\n" );
			m_VidCapTimer->stop();
		}
	}

	m_VidCaptureEnabled = enableVidCapture;
	std::vector<ToGuiHardwareCtrlClient>::iterator hardwareIter;
	for( hardwareIter = m_ToGuiHardwareCtrlList.begin(); hardwareIter != m_ToGuiHardwareCtrlList.end(); ++hardwareIter )
	{
		ToGuiHardwareCtrlClient& toGuiClient = *hardwareIter;
		toGuiClient.m_Callback->doGuiWantVideoCapture( enableVidCapture );
	}
}

//============================================================================
//! time to send out new video frame
void AppCommon::onVidCapTimer( void )
{
#ifdef TARGET_OS_WINDOWS
	uint32_t u32ImageLen = 0;
	uint32_t u32Format = 0;
	uint8_t * pu8VidBmp  = m_VidCap->takeSnapShot( 0, u32ImageLen, u32Format );
	if( pu8VidBmp )
	{
		BITMAPINFO * poBitmap = (BITMAPINFO *)pu8VidBmp;
		if( (0 == u32Format) &&
			(0 != poBitmap->bmiHeader.biSizeImage ) &&
			(poBitmap->bmiHeader.biSizeImage == (poBitmap->bmiHeader.biWidth * poBitmap->bmiHeader.biHeight * 3)))
		{
			// some cams do not place format in bitmap even though data is valid ie (Microsoft LifeCam VX-3000)
			u32Format = FOURCC_RGB;
		}

		// send out video frame as bitmap
		m_Engine.fromGuiVideoData(	u32Format, // format
									(uint8_t *)&pu8VidBmp[poBitmap->bmiHeader.biSize], 
									poBitmap->bmiHeader.biWidth,
									poBitmap->bmiHeader.biHeight,
									poBitmap->bmiHeader.biSizeImage,
									m_CamCaptureRotation );
		delete pu8VidBmp;
	}
#endif // TARGET_OS_WINDOWS
}

//============================================================================
//! show jpeg for video playback
void AppCommon::toGuiPlayVideoFrame(  VxGUID& onlineId, uint8_t * pu8Jpg, uint32_t u32JpgDataLen,int motion0To100000 )
{
	if( false == VxIsAppShuttingDown() )
	{
		//LogMsg( LOG_INFO, "toGuiPlayVideoFrame %d len %d\n", ePluginType, u32JpgDataLen );

		std::vector<ToGuiActivityClient>::iterator iter;
#ifdef DEBUG_TOGUI_CLIENT_MUTEX
		LogMsg( LOG_INFO, "toGuiPlayVideoFrame: toGuiActivityClientsLock\n" );
#endif // DEBUG_TOGUI_CLIENT_MUTEX
		toGuiActivityClientsLock();
		for( iter = m_ToGuiActivityClientList.begin(); iter != m_ToGuiActivityClientList.end(); ++iter )
		{
			ToGuiActivityClient& toGuiClient = *iter;
			toGuiClient.m_Callback->toGuiClientPlayVideoFrame(	toGuiClient.m_UserData, 
																onlineId,
																pu8Jpg,
																u32JpgDataLen,
																motion0To100000 );
		}

		toGuiActivityClientsUnlock();
	}
}

//============================================================================
//! show jpeg for video playback
int AppCommon::toGuiPlayVideoFrame( VxGUID& onlineId, uint8_t * picBuf, uint32_t picBufLen, int picWidth, int picHeight )
{
    int behindFrameCnt = 0;
    if( false == VxIsAppShuttingDown() )
    {
        //LogMsg( LOG_INFO, "toGuiPlayVideoFrame %d len %d\n", ePluginType, u32JpgDataLen );

        std::vector<ToGuiActivityClient>::iterator iter;
#ifdef DEBUG_TOGUI_CLIENT_MUTEX
        LogMsg( LOG_INFO, "toGuiPlayVideoFrame: toGuiActivityClientsLock\n" );
#endif // DEBUG_TOGUI_CLIENT_MUTEX
        toGuiActivityClientsLock();
        for( iter = m_ToGuiActivityClientList.begin(); iter != m_ToGuiActivityClientList.end(); ++iter )
        {
            ToGuiActivityClient& toGuiClient = *iter;
            behindFrameCnt += toGuiClient.m_Callback->toGuiClientPlayVideoFrame( toGuiClient.m_UserData,
                                                                                onlineId,
                                                                                picBuf,
                                                                                picBufLen,
                                                                                picWidth,
                                                                                picHeight );
        }

        toGuiActivityClientsUnlock();
    }

    return behindFrameCnt;
}



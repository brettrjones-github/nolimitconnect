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
    if( m_VidCaptureEnabled != enableVidCapture )
    {
        m_VidCaptureEnabled = enableVidCapture;
        m_CamLogic.toGuiWantVideoCapture( enableVidCapture );

        if( enableVidCapture )
        {
            static bool bFirstTimeVideoCaptureStarted = true;
            if( bFirstTimeVideoCaptureStarted )
            {
                if( !m_CamLogic.isCamCaptureRunning() )
                {
                    QMessageBox::warning( this, QObject::tr( "Web Cam Video" ), QObject::tr( "No Video Capture Devices Found" ) );
                    return;
                }

                m_CamSourceId = m_CamLogic.getCamSourceId();

                setCamCaptureRotation( m_AppSettings.getCamRotation( m_CamSourceId ) );

                bFirstTimeVideoCaptureStarted = false;
            }
        }
        else
        {
            LogMsg( LOG_INFO, "AppCommon::slotEnableVideoCapture stopping capture\n" );
        }
    }
	
	for( auto hardwareIter = m_ToGuiHardwareCtrlList.begin(); hardwareIter != m_ToGuiHardwareCtrlList.end(); ++hardwareIter )
	{
		ToGuiHardwareControlInterface* toGuiClient = *hardwareIter;
		toGuiClient->toGuiWantVideoCapture( enableVidCapture );
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
		//LogMsg( LOG_INFO, "toGuiPlayVideoFrame %d len %d", ePluginType, u32JpgDataLen );

		for( ToGuiActivityInterface* toGuiClient : m_ToGuiActivityInterfaceList )
		{
			toGuiClient->toGuiClientPlayVideoFrame(	onlineId,
													pu8Jpg,
													u32JpgDataLen,
	     											motion0To100000 );
		}
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

        for( auto iter = m_ToGuiActivityInterfaceList.begin(); iter != m_ToGuiActivityInterfaceList.end(); ++iter )
        {
            ToGuiActivityInterface* toGuiClient = *iter;
            behindFrameCnt += toGuiClient->toGuiClientPlayVideoFrame( onlineId,
																		picBuf,
																		picBufLen,
																		picWidth,
																		picHeight );
        }
    }

    return behindFrameCnt;
}



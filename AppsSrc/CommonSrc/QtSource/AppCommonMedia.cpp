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
// http://www.nolimitconnect.org
//============================================================================


#include "AppCommon.h"
#include "AppSettings.h"
#include "GuiPlayerMgr.h"

#include "ActivityScanWebCams.h"
#include "ToGuiActivityInterface.h"
#include "ToGuiHardwareControlInterface.h"

#include <VxVideoLib/VxVideoLib.h>
#include <CoreLib/VxGlobals.h>

#include <QTimer>
#include <QMessageBox>

//============================================================================
void AppCommon::toGuiPlayNlcMedia( AssetBaseInfo* assetInfo )
{
	LogMsg( LOG_INFO, "#### AppCommon::toGuiPlayNlcMedia %s", assetInfo->getAssetName().c_str() );
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	emit signalInternalPlayNlcMedia( *assetInfo );
}

//============================================================================
void AppCommon::slotInternalPlayNlcMedia( AssetBaseInfo assetInfo )
{
	m_PlayerMgr.playMedia( assetInfo );
}

//============================================================================
void AppCommon::toGuiWantVideoCapture( EAppModule appModule, bool wantVidCapture )
{
	LogMsg( LOG_INFO, "#### AppCommon::toGuiWantVideoCapture %d", wantVidCapture );
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	emit signalInternalWantVideoCapture( appModule, wantVidCapture );
}

//============================================================================
void AppCommon::slotInternalWantVideoCapture( EAppModule appModule, bool enableVidCapture )
{
	bool wasCamEnabled = m_CamLogic.isCamCaptureRunning();
	m_CamLogic.toGuiWantVideoCapture( appModule, enableVidCapture );
	bool isCamEnabled = m_CamLogic.isCamCaptureRunning();

    if( wasCamEnabled != isCamEnabled )
    {
        if( isCamEnabled )
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
            LogMsg( LOG_INFO, "AppCommon::slotEnableVideoCapture stopping capture" );
        }

		m_ToGuiHardwareCtrlBusy = true;
		for( auto hardwareIter = m_ToGuiHardwareCtrlList.begin(); hardwareIter != m_ToGuiHardwareCtrlList.end(); ++hardwareIter )
		{
			ToGuiHardwareControlInterface* toGuiClient = *hardwareIter;
			toGuiClient->callbackToGuiWantVideoCapture( enableVidCapture );
		}

		m_ToGuiHardwareCtrlBusy = false;
    }
}

//============================================================================
void AppCommon::toGuiPlayVideoFrame( VxGUID& feedOnlineId, uint8_t* pu8Jpg, uint32_t u32JpgDataLen, int motion0To100000 )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	m_PlayerMgr.toGuiPlayVideoFrame( feedOnlineId, pu8Jpg, u32JpgDataLen, motion0To100000 );
}

//============================================================================
int AppCommon::toGuiPlayVideoFrame( VxGUID& feedOnlineId, uint8_t* picBuf, uint32_t picBufLen, int picWidth, int picHeight )
{
	if( VxIsAppShuttingDown() )
	{
		return 0;
	}

	return m_PlayerMgr.toGuiPlayVideoFrame( feedOnlineId, picBuf, picBufLen, picWidth, picHeight );
}

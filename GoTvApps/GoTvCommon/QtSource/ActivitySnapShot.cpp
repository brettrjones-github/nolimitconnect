//============================================================================
// Copyright (C) 2010 Brett R. Jones
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
#include <app_precompiled_hdr.h>
#include "ActivitySnapShot.h"
#include "AppCommon.h"

#include <QMessageBox>
#include <QTimer>

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <VxVideoLib/VxVideoLib.h>

//============================================================================
ActivitySnapShot::ActivitySnapShot( AppCommon& app, QWidget * parent )
: ActivityBase( OBJNAME_ACTIVITY_SNAP_SHOT, app, parent, eAppletSnapShot, true, false, true )
, m_CloseDlgTimer( new QTimer( this ) )
{
	ui.setupUi(this);
    connectBarWidgets();
    setTitleBarText( QObject::tr( "Take Snapshot" ) );

	connect( ui.snapshotButton, SIGNAL(clicked()), this, SLOT(onSnapShotButClick()));
	connect( ui.cancelButton, SIGNAL(clicked()), this, SLOT(onCancelButClick()));
    #ifdef TARGET_OS_WINDOWS
	    m_VidCap = VxGetVidCapInterface();
    #endif // TARGET_OS_WINDOWS
	if( !m_VidCap || 0 == m_VidCap->startupVidCap() )
	{
		QMessageBox::warning(this, QObject::tr("Camera Capture"), QObject::tr("No Camera Source Available.") );
        connect( m_CloseDlgTimer, SIGNAL( timeout() ), this, SLOT( onCancelButClick() ) );
        m_CloseDlgTimer->setSingleShot( true );
        m_CloseDlgTimer->start(300);
	}
	else
	{
		// start preview
		QRect oWndRect = ui.imageScreen->geometry();
        m_VidCap->startPreview(	(void *)ui.imageScreen->winId(),
									0,
									oWndRect.left(),
									oWndRect.top(),
									oWndRect.width(),
									oWndRect.height() );
		
	}
}

//============================================================================
//! take picture for me
void ActivitySnapShot::onSnapShotButClick( void )
{
#ifdef TARGET_OS_WINDOWS
	uint32_t u32Format;
    m_pu8BitmapData = m_VidCap->takeSnapShot( 0, m_u32BitmapLen, u32Format );
	m_VidCap->shutdownVidCap();
    BITMAPINFO * poBitmap = (BITMAPINFO *)m_pu8BitmapData;
	if( poBitmap  )
    {
		if( (0 == u32Format) &&
			(0 != poBitmap->bmiHeader.biSizeImage ) &&
			(poBitmap->bmiHeader.biSizeImage == (poBitmap->bmiHeader.biWidth * poBitmap->bmiHeader.biHeight * 3)))
		{
			// some cams do not place format in bitmap even though data is valid ie (Microsoft LifeCam VX-3000)
			u32Format = FOURCC_RGB;
		}
		uint32_t u32JpgDataLen = 0;
		uint8_t * pu8JpgData = VxConvertToJpg(	u32Format, // format
										(uint8_t *)&m_pu8BitmapData[poBitmap->bmiHeader.biSize], 
										poBitmap->bmiHeader.biSizeImage,
										poBitmap->bmiHeader.biWidth,
										poBitmap->bmiHeader.biHeight,
										u32JpgDataLen,
										320,
										240 );
		emit signalJpgSnapshot( pu8JpgData, u32JpgDataLen, 320, 240 );
		delete pu8JpgData;
		delete m_pu8BitmapData;
		m_pu8BitmapData = NULL; 
	}

#endif // TARGET_OS_WINDOWS
    close();
}

//============================================================================
//! Implement the OnClickListener callback    
void ActivitySnapShot::onCancelButClick( void ) 
{
	m_VidCap->shutdownVidCap();

    close();
}


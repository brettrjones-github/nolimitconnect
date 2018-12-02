#pragma once
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
// brett.jones@engineer.com
// http://www.gotvptop.net
//============================================================================

#include <PktLib/VxCommon.h>

#include <QDialog>
#include <QPixmap>
#include "ui_ActivityVideoOptions.h"
#include "ActivityToFriendBase.h"
#include "VidWidget.h"

class AppCommon;
class P2PEngine;
class VxNetIdent;

class ActivityVideoOptions : public ActivityToFriendBase
{
	Q_OBJECT
public:

	ActivityVideoOptions(	AppCommon&			app,
							VxNetIdent *			netIdent, 
							bool					bIsMyself,
							QWidget *				parent = NULL );

	virtual ~ActivityVideoOptions();

	void						playVideoFrame( VxGUID& feedId, unsigned char * pu8Jpg, unsigned long u32JpgLen, int motion0To100000 );

	void						webCamSourceOffline();

signals:
	void						signalWebCamClientDlgClosed( VxNetIdent * netIdent );
	void						signalPlayVideoFrame( QPixmap oPicBitmap, int iRotate );
	void						signalPlayAudio( unsigned short * pu16PcmData, unsigned short u16PcmDataLen );

public slots:
	void						onStopButClick( void ); 
	void						onRotateButtonClick( void ); 

protected:
	void						resizeBitmapToFitScreen( QLabel * VideoScreen, QImage& oPicBitmap );

	//=== vars ===//
	Ui::VideoOptionsDlgClass	ui;
	bool						m_bIsMyself;
	//VxNetIdent *				m_Ident;
};


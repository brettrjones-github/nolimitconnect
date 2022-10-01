#pragma once
//============================================================================
// Copyright (C) 2015 Brett R. Jones
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

#include "InputBaseWidget.h"

#include "ui_InputPhotoWidget.h"

class InputPhotoWidget : public InputBaseWidget
{
    Q_OBJECT

public:
    InputPhotoWidget( QWidget* parent=0);

	void						setCanSend( bool canSend ) {};

	void						callbackGuiPlayMotionVideoFrame( VxGUID& feedOnlineId, QImage& vidFrame, int motion0To100000 );

protected slots:
    void						slotSnapShotButtonClicked( void );
	void						slotRotateCamButtonClicked( void );
	void						slotSelectCamButtonClicked( void );
	void						slotCancelPhotoButtonClicked( void );
    void						slotVideoFrameBitmap( QImage picBitmap, int iRotate );
    void                        slotExitPhotoWidget( void );

protected:
	void						hideEvent( QHideEvent* hideEvent );
	void						showEvent( QShowEvent* showEvent );

	Ui::InputPhotoWidget		ui;
	QPixmap						m_PicImage;
};

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
// http://www.nolimitconnect.com
//============================================================================
#include <app_precompiled_hdr.h>
#include "InputAllWidget.h"
#include "AppCommon.h"
#include "GuiParams.h"

//============================================================================
InputAllWidget::InputAllWidget( QWidget * parent )
: InputBaseWidget( GetAppInstance(), parent )
{
	ui.setupUi( this );

	ui.m_SelectFaceAllButton->setIcons( eMyIconFaceSelectNormal );
	ui.m_CameraAllButton->setIcons( eMyIconCameraNormal );
	ui.m_GalleryAllButton->setIcons( eMyIconGalleryNormal );
	ui.m_VideoAllButton->setIcons( eMyIconCamcorderNormal );
	ui.m_MicAllButton->setIcons( eMyIconMicrophoneOn );

    ui.m_SelectFaceAllButton->setSquareButtonSize( eButtonSizeTiny );
    ui.m_CameraAllButton->setSquareButtonSize( eButtonSizeTiny );
    ui.m_GalleryAllButton->setSquareButtonSize( eButtonSizeTiny );
    ui.m_VideoAllButton->setSquareButtonSize( eButtonSizeTiny );
    ui.m_MicAllButton->setSquareButtonSize( eButtonSizeTiny );

	connect( ui.m_TextAllButton, SIGNAL(clicked()), this, SLOT(slotTextButtonClicked()) );
	connect( ui.m_SelectFaceAllButton, SIGNAL(clicked()), this, SLOT(slotEmoteButtonClicked()) );
	connect( ui.m_CameraAllButton, SIGNAL(clicked()), this, SLOT(slotCameraButtonClicked()) );
	connect( ui.m_GalleryAllButton, SIGNAL(clicked()), this, SLOT(slotGalleryButtonClicked()) );
	connect( ui.m_VideoAllButton, SIGNAL(clicked()), this, SLOT(slotVideoButtonClicked()) );
	connect( ui.m_MicAllButton, SIGNAL(pressed()), this, SLOT(slotMicButtonPressed()) );
	connect( ui.m_MicAllButton, SIGNAL(released()), this, SLOT(slotMicButtonReleased()) );
}

//============================================================================
void InputAllWidget::setCanSend( bool canSend )
{
	ui.m_TextAllButton->setEnabled( canSend );
	ui.m_SelectFaceAllButton->setEnabled( canSend );
	ui.m_CameraAllButton->setEnabled( canSend );
	ui.m_GalleryAllButton->setEnabled( canSend );
	ui.m_VideoAllButton->setEnabled( canSend );
	ui.m_MicAllButton->setEnabled( canSend );
}

//============================================================================
void InputAllWidget::slotTextButtonClicked( void )
{
	emit signalUserInputButtonClicked();
	emit signalAllTextButtonClicked();
}

//============================================================================
void InputAllWidget::slotEmoteButtonClicked( void )
{
	emit signalUserInputButtonClicked();
	emit signalAllEmoteButtonClicked();
}

//============================================================================
void InputAllWidget::slotVideoButtonClicked( void )
{
	emit signalUserInputButtonClicked();
	emit signalAllVideoButtonClicked();
}

//============================================================================
void InputAllWidget::slotCameraButtonClicked( void )
{
	emit signalUserInputButtonClicked();
	emit signalAllCameraButtonClicked();
}

//============================================================================
void InputAllWidget::slotGalleryButtonClicked( void )
{
	emit signalUserInputButtonClicked();
	emit signalAllGalleryButtonClicked();
}

//============================================================================
void InputAllWidget::slotMicButtonPressed( void )
{
	emit signalUserInputButtonClicked();
	emit signalAllMicButtonPressed();
}

//============================================================================
void InputAllWidget::slotMicButtonReleased( void )
{
	emit signalAllMicButtonReleased();
}

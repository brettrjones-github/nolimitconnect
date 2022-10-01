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

#include "ui_InputAllWidget.h"

class InputAllWidget : public InputBaseWidget
{
    Q_OBJECT

public:
    InputAllWidget( QWidget* parent = 0 );

	void						setCanSend( bool canSend );

signals:
	void						signalUserInputButtonClicked( void );

	void						signalAllTextButtonClicked( void );
	void						signalAllEmoteButtonClicked( void );
	void						signalAllVideoButtonClicked( void );
	void						signalAllCameraButtonClicked( void );
	void						signalAllGalleryButtonClicked( void );
	void						signalAllMicButtonPressed( void );
	void						signalAllMicButtonReleased( void );

private slots:
	void						slotTextButtonClicked( void );
	void						slotEmoteButtonClicked( void );
	void						slotVideoButtonClicked( void );
	void						slotCameraButtonClicked( void );
	void						slotGalleryButtonClicked( void );
	void						slotMicButtonPressed( void );
	void						slotMicButtonReleased( void );


private:
	Ui::InputAllWidget			ui;
};

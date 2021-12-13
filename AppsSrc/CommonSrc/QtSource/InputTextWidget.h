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
// http://www.nolimitconnect.com
//============================================================================

#include "InputBaseWidget.h"

#include "ui_InputTextWidget.h"

class InputTextWidget : public InputBaseWidget
{
    Q_OBJECT

public:
    InputTextWidget( QWidget * parent=0);

	void						setCanSend( bool canSend ) {};

private slots:
    void						slotCancelButtonClicked( void );
	void						slotSendButtonClicked( void );

private:
	Ui::InputTextWidgetClass	ui;
};

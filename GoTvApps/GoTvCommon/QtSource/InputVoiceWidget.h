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
// brett.jones@engineer.com
// http://www.gotvptop.net
//============================================================================

#include "InputBaseWidget.h"

#include "ui_InputVoiceWidget.h"

class InputVoiceWidget : public InputBaseWidget
{
    Q_OBJECT

public:
    InputVoiceWidget( QWidget * parent=0);

	void						setCanSend( bool canSend ) {};

private slots:
    void						slotBeginRecord( void );
	void						slotEndRecord( void );
	void						slotRecVoiceCancel( void );
	void						slotShredComplete( void );

private:
	Ui::InputVoiceWidget		ui;
	bool						m_IsRecording;
};

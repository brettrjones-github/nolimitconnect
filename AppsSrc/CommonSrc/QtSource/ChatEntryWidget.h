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
// bjones.engineer@gmail.com
// http://www.nolimitconnect.org
//============================================================================

#include "ui_ChatEntryWidget.h"

#include <CoreLib/AssetDefs.h>

class VxNetIdent;

class ChatEntryWidget : public QWidget
{
    Q_OBJECT

public:
    ChatEntryWidget( QWidget *parent = 0, EAssetType inputMode = eAssetTypeUnknown );
	virtual ~ChatEntryWidget() = default;

	void						setIdents( GuiUser * myIdent, GuiUser * hisIdent );
	void						setEntryMode( EAssetType inputMode );
	void						setIsPersonalRecorder( bool isPersonal );
	void						setCanSend( bool canSend );

	void						playVideoFrame( VxGUID& onlineId, unsigned char * pu8Jpg, unsigned long u32JpgLen, int motion0To100000 );

signals:
	void						signalUserInputButtonClicked( void );

private slots:
	void						slotAllTextButtonClicked( void );
	void						slotAllEmoteButtonClicked( void );
	void						slotAllVideoButtonClicked( void );
	void						slotAllCameraButtonClicked( void );
	void						slotAllGalleryButtonClicked( void );
	void						slotAllMicButtonPressed( void );
	void						slotAllMicButtonReleased( void );
	void						slotInputCompleted( void );

private:
	Ui::ChatEntryWidget			ui;
	EAssetType					m_InputMode;
    GuiUser *				    m_MyIdent;
    GuiUser *				    m_HisIdent;
};

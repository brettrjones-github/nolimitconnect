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

#include "ui_SessionWidget.h"

#include <CoreLib/AssetDefs.h>

class GuiUser;

class SessionWidget : public QWidget
{
	Q_OBJECT

public:
	SessionWidget( QWidget *parent = 0, EAssetType inputMode = eAssetTypeUnknown );
    virtual ~SessionWidget() = default;

	void						setIdents( GuiUser * myIdent, GuiUser * hisIdent );
	void						setEntryMode( EAssetType inputMode );
	void						setIsPersonalRecorder( bool isPersonal );
	void						setCanSend( bool canSend );

	void						playVideoFrame( VxGUID& onlineId, unsigned char * pu8Jpg, unsigned long u32JpgLen, int motion0To100000 );

	void						onActivityStop( void );

    QLabel *                    getSessionStatusLabel( void ) { return ui.m_StatusLabel; }
    HistoryListWidget *         getSessionHistoryList( void ) { return ui.m_HistoryList; }
    ChatEntryWidget *           getSessionChatEntry( void ) { return ui.m_ChatEntry; }

signals:
	void						signalUserInputButtonClicked( void );

protected slots:
    void                        slotStatusMsg( QString );

protected:
	Ui::SessionWidgetClass		ui;
    AppCommon&                  m_MyApp;
	EAssetType					m_InputMode;
	bool						m_IsInitialized;
};

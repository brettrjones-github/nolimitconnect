#pragma once
//============================================================================
// Copyright (C) 2019 Brett R. Jones
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

#include "AppletClientBase.h"

#include <GoTvCore/GoTvP2P/PluginSettings/PluginSetting.h>

#include "ui_AppletChatRoomJoin.h"

class AppletChatRoomJoin : public AppletClientBase
{
	Q_OBJECT
public:
    AppletChatRoomJoin( AppCommon& app, QWidget * parent );
	virtual ~AppletChatRoomJoin();

    void                        infoMsg( const char * infoMsg, ... );

    void                        toGuiInfoMsg( char * logMsg );

signals:
    void                        signalInfoMsg( const QString& logMsg );

protected slots:
    void                        slotInfoMsg( const QString& text );
    void						slotHostAnnounceStatus( EHostType hostType, VxGUID sessionId, EHostAnnounceStatus hostStatus, QString strMsg );
    void						slotHostJoinStatus( EHostType hostType, VxGUID sessionId, EHostJoinStatus hostStatus, QString strMsg );
    void						slotHostSearchStatus( EHostType hostType, VxGUID sessionId, EHostSearchStatus hostStatus, QString strMsg );
    void                        slotHostSearchResult( EHostType hostType, VxGUID sessionId, VxNetIdent hostIdent, PluginSetting pluginSetting );

    void                        slotJoinDefaultChatRoom( void );
    void                        slotCopyResultToClipboardClicked( void );

protected:
    void						setupApplet( void );
    QPlainTextEdit *            getInfoEdit( void )     { return ui.m_InfoPlainTextEdit; }

    //=== vars ===//
    Ui::ChatRoomJoinUi          ui;
};



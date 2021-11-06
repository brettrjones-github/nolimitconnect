#pragma once
//============================================================================
// Copyright (C) 2020 Brett R. Jones
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

#include "AppletBase.h"
#include "ui_AppletInviteAccept.h"

class VxUrl;

class AppletInviteAccept : public AppletBase
{
    Q_OBJECT
public:
    AppletInviteAccept( AppCommon& app, QWidget * parent );
    virtual ~AppletInviteAccept();

    void                        infoMsg( const char * infoMsg, ... );
    void                        toGuiInfoMsg( char * logMsg );

signals:
    void                        signalInfoMsg( const QString& logMsg );

protected slots:
    void                        slotCopyInviteFromClipboardButtonClicked( void );
    void                        slotAcceptInviteButtonClicked( void );
    void                        slotRejectInviteButtonClicked( void );

protected:
    void						setupApplet( void );
    QPlainTextEdit *            getInviteEdit( void )     { return ui.m_InviteTextEdit; }
    void                        updateInvite( void );

    Ui::AppletInviteAcceptUi    ui;

};



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
// http://www.nolimitconnect.org
//============================================================================

#include "AppletBase.h"
#include "ui_AppletInviteCreate.h"

class AppletInviteCreate : public AppletBase
{
    Q_OBJECT
public:
    AppletInviteCreate( AppCommon& app, QWidget* parent );
    virtual ~AppletInviteCreate();

    void                        infoMsg( const char* infoMsg, ... );
    void                        toGuiInfoMsg( char * logMsg );

signals:
    void                        signalInfoMsg( const QString& logMsg );

protected slots:
    void                        slotCopyInviteButtonClicked( void );
    void                        slotNetworkSettingsInfoButtonClicked( void );
    void                        slotSelectGroupHostButtonClicked( void );
    void                        slotGroupHostSelected( QString hostUrl );

    void                        slotUpdateInvite( void );

protected:
    QPlainTextEdit *            getInviteMessageEdit( void )    { return ui.m_InviteMessageTextEdit; }
    QPlainTextEdit*             getInviteTextEdit( void )       { return ui.m_InviteTextEdit; }
    void                        addInviteText( QString text );
    void                        updateUrls( void );
    bool                        populateNetSettingUrl( EHostType hostType, std::string& ptopUrl );

    Ui::AppletInviteCreateUi    ui;
};



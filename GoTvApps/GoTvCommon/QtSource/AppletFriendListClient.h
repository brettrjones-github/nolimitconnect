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
// http://www.nolimitconnect.com
//============================================================================

#include "AppletClientBase.h"

#include <GuiInterface/IDefs.h>
#include <ptop_src/ptop_engine_src/PluginSettings/PluginSetting.h>

#include <QString>
#include <QDialog>

#include "ui_AppletFriendListClient.h"

class VxNetIdent;
class GuiHostSession;

class AppletFriendListClient : public AppletClientBase
{
	Q_OBJECT
public:
	AppletFriendListClient(	AppCommon&		    app, 
							QWidget *			parent = NULL );
	virtual ~AppletFriendListClient() = default;

    void                        infoMsg( const char * infoMsg, ... );
    void                        toGuiInfoMsg( char * logMsg );

    void						setStatusLabel( QString strMsg );
    void                        setInfoLabel( QString strMsg );

    void						addPluginSettingToList( EHostType hostType, VxGUID& sessionId, VxNetIdent& hostIdent, PluginSetting& pluginSetting );
    void                        clearPluginSettingToList( void );
    void                        clearStatus( void );

signals:
    void						signalSearchResult( VxNetIdent * netIdent );
    void						signalSearchComplete( void );
    void                        signalLogMsg( const QString& logMsg );
    void                        signalInfoMsg( const QString& logMsg );

private slots:
    void                        slotInfoMsg( const QString& text );
    void						slotSearchComplete( void );
    void						slotHomeButtonClicked( void ) override;
    void						slotStartSearchState( bool startSearch );
    void						slotHostAnnounceStatus( EHostType hostType, VxGUID sessionId, EHostAnnounceStatus hostStatus, QString strMsg );
    void						slotHostJoinStatus( EHostType hostType, VxGUID sessionId, EHostJoinStatus hostStatus, QString strMsg );
    void						slotHostSearchStatus( EHostType hostType, VxGUID sessionId, EHostSearchStatus hostStatus, QString strMsg );
    void                        slotHostSearchResult( EHostType hostType, VxGUID sessionId, VxNetIdent hostIdent, PluginSetting pluginSetting );

    //virtual void                slotIconButtonClicked( GuiHostSession* hostSession, HostListItem* hostItem );
    //virtual void                slotMenuButtonClicked( GuiHostSession* hostSession, HostListItem* hostItem );
    //virtual void                slotJoinButtonClicked( GuiHostSession* hostSession, HostListItem* hostItem );

    void                        onShowFriendList( void );
    void                        onShowIgnoreList( void );

protected:
    void						showEvent( QShowEvent * ev ) override;
    void						hideEvent( QHideEvent * ev ) override;

    //=== vars ===//
    Ui::AppletFriendListClientUi ui;
    bool                        m_SearchStarted{ false };
};

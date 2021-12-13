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
#include "GuiUserMgrGuiUserUpdateInterface.h"

#include <GuiInterface/IDefs.h>
#include <ptop_src/ptop_engine_src/PluginSettings/PluginSetting.h>

#include <QString>
#include <QDialog>

#include "ui_AppletFriendListClient.h"

class VxNetIdent;
class GuiHostSession;

class AppletFriendListClient : public AppletClientBase, public GuiUserMgrGuiUserUpdateInterface
{
	Q_OBJECT
public:
	AppletFriendListClient(	AppCommon&		    app, 
							QWidget *			parent = NULL );
	virtual ~AppletFriendListClient() override;

    void                        infoMsg( const char * infoMsg, ... );
    void                        toGuiInfoMsg( char * logMsg );

    void						setStatusLabel( QString strMsg );

    EUserViewType             getListType( void )             { return m_FriendListType; }
    void                        clearList( void );
    void                        clearStatus( void );

signals:
    void						signalSearchResult( VxNetIdent * netIdent );
    void						signalSearchComplete( void );
    void                        signalLogMsg( const QString& logMsg );
    void                        signalInfoMsg( const QString& logMsg );

private slots:

    void                        slotFriendsButtonClicked( void );
    void                        slotFriendsInfoButtonClicked( void );
    void                        slotIgnoredButtonClicked( void );
    void                        slotIgnoredInfoButtonClicked( void );
    void                        slotNearbyButtonClicked( void );
    void                        slotNearbyInfoButtonClicked( void );

    void                        onShowFriendTypeChanged( void );
    void                        onShowFriendList( void );
    void                        onShowIgnoreList( void );
    void                        onShowNearbyList( void );

protected:
    virtual void				callbackIndentListUpdate( EUserViewType listType, VxGUID& onlineId, uint64_t timestamp ) override;
    virtual void				callbackIndentListRemove( EUserViewType listType, VxGUID& onlineId ) override;

    virtual void				callbackOnUserAdded( GuiUser* guiUser ) override;
    virtual void				callbackOnUserUpdated( GuiUser* guiUser ) override;
    virtual void				callbackOnUserRemoved( VxGUID& onlineId ) override;

    void						showEvent( QShowEvent * ev ) override;
    void						hideEvent( QHideEvent * ev ) override;

    void                        updateUser( EUserViewType listType, VxGUID& onlineId );
    void                        updateUser( GuiUser* guiUser );
    void                        removeUser( VxGUID& onlineId );

    void                        updateFriendList( EUserViewType listType, std::vector<std::pair<VxGUID, int64_t>> idList );

    //=== vars ===//
    Ui::AppletFriendListClientUi ui;
    EUserViewType             m_FriendListType{ eUserViewTypeFriends };
};

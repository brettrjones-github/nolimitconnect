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

#include "AppletFriendListClient.h"
#include "ActivityInformation.h"

#include "AppCommon.h"
#include "AppGlobals.h"

#include "ActivityMessageBox.h"
#include "GuiHostSession.h"
#include "GuiParams.h"
#include "GuiHelpers.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <CoreLib/VxGlobals.h>

namespace
{
    const int MAX_INFO_MSG_SIZE = 2048;
}

//============================================================================
AppletFriendListClient::AppletFriendListClient(	AppCommon&		    app, 
												QWidget *			parent )
: AppletClientBase( OBJNAME_APPLET_FRIEND_LIST_CLIENT, app, parent )
{
    setAppletType( eAppletFriendListClient );
    setHostType( eHostTypeGroup );
    ui.setupUi( getContentItemsFrame() );
    setTitleBarText( DescribeApplet( m_EAppletType ) );

    ui.m_FriendsButton->setFixedSize( eButtonSizeSmall );
    ui.m_FriendsButton->setIcon( eMyIconFriend );
    ui.m_FriendsInfoButton->setFixedSize( eButtonSizeSmall );
    ui.m_FriendsInfoButton->setIcon( eMyIconInformation );

    ui.m_IgnoredButton->setFixedSize( eButtonSizeSmall );
    ui.m_IgnoredButton->setIcon( eMyIconIgnored );
    ui.m_IgnoredInfoButton->setFixedSize( eButtonSizeSmall );
    ui.m_IgnoredInfoButton->setIcon( eMyIconInformation );

    ui.m_NearbyButton->setFixedSize( eButtonSizeSmall );
    ui.m_NearbyButton->setIcon( eMyIconFriendBroadcast );
    ui.m_NearbyInfoButton->setFixedSize( eButtonSizeSmall );
    ui.m_NearbyInfoButton->setIcon( eMyIconInformation );

    setSearchType( eSearchGroupHost );

    connectBarWidgets();

    connect( this,					    SIGNAL(finished(int)),						this, SLOT(slotHomeButtonClicked()) );
    //connect( ui.m_SearchsParamWidget,	SIGNAL(signalSearchState(bool)),		    this, SLOT(slotStartSearchState(bool)) );
    //connect( this,					    SIGNAL(signalSearchComplete()),				this, SLOT(slotSearchComplete()) );
    //connect( this,					    SIGNAL(signalSearchResult(VxNetIdent*)),	this, SLOT(slotSearchResult(VxNetIdent*)) );

    //connect( this, SIGNAL( signalLogMsg( const QString& ) ), this, SLOT( slotInfoMsg( const QString& ) ) );
    //connect( this, SIGNAL( signalInfoMsg( const QString& ) ), this, SLOT( slotInfoMsg( const QString& ) ) );

    /*
    connect( &m_MyApp, SIGNAL(signalHostJoinStatus( EHostType, VxGUID, EHostJoinStatus, QString )),
        this, SLOT(slotHostJoinStatus( EHostType, VxGUID, EHostJoinStatus, QString )) );
    connect( &m_MyApp, SIGNAL(signalHostSearchStatus( EHostType, VxGUID, EHostSearchStatus, QString )),
        this, SLOT(slotHostSearchStatus( EHostType, VxGUID, EHostSearchStatus, QString )) );

    connect( &m_MyApp, SIGNAL(signalHostSearchResult( EHostType, VxGUID, VxNetIdent, PluginSetting  )),
        this, SLOT(slotHostSearchResult( EHostType, VxGUID, VxNetIdent, PluginSetting  )) );

    connect( ui.m_FriendListWidget,      SIGNAL( signalIconButtonClicked( GuiHostSession*, FriendListItem* ) ),  this, SLOT( slotIconButtonClicked( GuiHostSession*, FriendListItem* ) ) );
    connect( ui.m_FriendListWidget,      SIGNAL( signalMenuButtonClicked( GuiHostSession*, FriendListItem* ) ),  this, SLOT( slotMenuButtonClicked( GuiHostSession*, FriendListItem* ) ) );
    connect( ui.m_FriendListWidget,      SIGNAL( signalJoinButtonClicked( GuiHostSession*, FriendListItem* ) ),  this, SLOT( slotJoinButtonClicked( GuiHostSession*, FriendListItem* ) ) );
    */

    connect( ui.m_FriendsButton, SIGNAL( clicked() ), this, SLOT( slotFriendsButtonClicked() ) );
    connect( ui.m_FriendsInfoButton, SIGNAL( clicked() ), this, SLOT( slotFriendsInfoButtonClicked() ) );
    connect( ui.m_IgnoredButton, SIGNAL( clicked() ), this, SLOT( slotIgnoredButtonClicked() ) );
    connect( ui.m_IgnoredInfoButton, SIGNAL( clicked() ), this, SLOT( slotIgnoredInfoButtonClicked() ) );
    connect( ui.m_NearbyButton, SIGNAL( clicked() ), this, SLOT( slotNearbyButtonClicked() ) );
    connect( ui.m_NearbyInfoButton, SIGNAL( clicked() ), this, SLOT( slotNearbyInfoButtonClicked() ) );

    m_MyApp.activityStateChange( this, true );
    m_UserMgr.wantGuiUserMgrGuiUserUpdateCallbacks( this, true );
    onShowFriendList();
}

//============================================================================
AppletFriendListClient::~AppletFriendListClient()
{
    m_Engine.fromGuiNearbyBroadcastEnable( false );
    m_UserMgr.wantGuiUserMgrGuiUserUpdateCallbacks( this, false );
    m_MyApp.activityStateChange( this, false );
}

//============================================================================
void AppletFriendListClient::setStatusLabel( QString strMsg )
{
    ui.m_StatusLabel->setText( strMsg );
}

//============================================================================
void AppletFriendListClient::showEvent( QShowEvent * ev )
{
    ActivityBase::showEvent( ev );
    m_MyApp.wantToGuiActivityCallbacks( this, this, true );
}

//============================================================================
void AppletFriendListClient::hideEvent( QHideEvent * ev )
{
    m_MyApp.wantToGuiActivityCallbacks( this, this, false );
    ActivityBase::hideEvent( ev );
}

//============================================================================
void AppletFriendListClient::toGuiInfoMsg( char * infoMsg )
{
    QString infoStr( infoMsg );
#if QT_VERSION > QT_VERSION_CHECK(6,0,0)
    infoStr.remove(QRegularExpression("[\\n\\r]"));
#else
    infoStr.remove(QRegExp("[\\n\\r]"));
#endif // QT_VERSION > QT_VERSION_CHECK(6,0,0)
    emit signalInfoMsg( infoStr );
}

//============================================================================
void AppletFriendListClient::infoMsg( const char* errMsg, ... )
{
    char as8Buf[ MAX_INFO_MSG_SIZE ];
    va_list argList;
    va_start( argList, errMsg );
    vsnprintf( as8Buf, sizeof( as8Buf ), errMsg, argList );
    as8Buf[ sizeof( as8Buf ) - 1 ] = 0;
    va_end( argList );

    toGuiInfoMsg( as8Buf );
}

//============================================================================
void AppletFriendListClient::clearList( void )
{
    ui.m_FriendListWidget->clear();
    setStatusLabel( GuiParams::describeFriendListType( m_ShowFriendType ) + QObject::tr( "List" ) );
}

//============================================================================
void AppletFriendListClient::clearStatus( void )
{
    setStatusLabel( "" );
}

//============================================================================
void AppletFriendListClient::onShowFriendList( void )
{
    clearList();

    std::vector<std::pair<VxGUID, int64_t>> friendList;
    FriendListMgr& friendMgr = m_Engine.getFriendListMgr();
    // make a copy to avoid pausing engine and to avoid threading issues
    friendMgr.lockList();
    friendList = friendMgr.getIdentList();
    friendMgr.unlockList();
    
    // for each see if we already have that ident as gui user else request it
    for( auto identTime : friendList )
    {
        GuiUser* guiUser = m_MyApp.getUserMgr().getOrQueryUser( identTime.first );
        if( guiUser->isAdmin() || guiUser->isFriend() )
        {
            updateUser( guiUser );
        }
    }
}

//============================================================================
void AppletFriendListClient::onShowIgnoreList( void )
{
    clearList();
    std::vector<std::pair<VxGUID, int64_t>> ignoreList;
    IgnoreListMgr& ignoreMgr = m_Engine.getIgnoreListMgr();
    // make a copy to avoid pausing engine and to avoid threading issues
    ignoreMgr.lockList();
    ignoreList = ignoreMgr.getIdentList();
    ignoreMgr.unlockList();

    // for each see if we already have that ident as gui user else request it
    for( auto identTime : ignoreList )
    {
        GuiUser* guiUser = m_MyApp.getUserMgr().getOrQueryUser( identTime.first );
        if( guiUser->isAdmin() || guiUser->isFriend() )
        {
            updateUser( guiUser );
        }
    }
}

//============================================================================
void AppletFriendListClient::onShowNearbyList( void )
{
    clearList();
    /*
    std::vector<std::pair<VxGUID, int64_t>> ignoreList;
    IgnoreListMgr& ignoreMgr = m_Engine.getIgnoreListMgr();
    // make a copy to avoid pausing engine and to avoid threading issues
    ignoreMgr.lockList();
    ignoreList = ignoreMgr.getIdentList();
    ignoreMgr.unlockList();

    // for each see if we already have that ident as gui user else request it
    for( auto identTime : friendList )
    {
        GuiUser* guiUser = m_MyApp.getUserMgr().getOrQueryUser( identTime.first );
        if( guiUser->isAdmin() || guiUser->isFriend() )
        {
            updateUser( guiUser );
        }
    }
    */
}

//============================================================================
void AppletFriendListClient::callbackOnUserAdded( GuiUser* guiUser )
{
    updateUser( guiUser );
}

//============================================================================
void AppletFriendListClient::callbackOnUserUpdated( GuiUser* guiUser )
{
    updateUser( guiUser );
}

//============================================================================
void AppletFriendListClient::callbackOnUserRemoved( VxGUID& onlineId )
{
    removeUser( onlineId );
}

//============================================================================
void AppletFriendListClient::onShowFriendTypeChanged( void )
{
    switch( m_ShowFriendType )
    {
    case eFriendListTypeIgnore:
        onShowIgnoreList();
        break;

    case eFriendListTypeNearby:
        onShowNearbyList();
        break;

    case eFriendListTypeFriend:
    default:
        onShowFriendList();
        break;
    }
}

//============================================================================
void AppletFriendListClient::slotFriendsButtonClicked( void )
{
    m_Engine.fromGuiNearbyBroadcastEnable( false );
    if( m_ShowFriendType != eFriendListTypeFriend )
    {
        m_ShowFriendType = eFriendListTypeFriend;
        onShowFriendTypeChanged();
    }
}

//============================================================================
void AppletFriendListClient::slotIgnoredButtonClicked( void )
{
    m_Engine.fromGuiNearbyBroadcastEnable( false );
    if( m_ShowFriendType != eFriendListTypeIgnore )
    {
        m_ShowFriendType = eFriendListTypeIgnore;
        onShowFriendTypeChanged();
    }
}

//============================================================================
void AppletFriendListClient::slotNearbyButtonClicked( void )
{
    m_Engine.fromGuiNearbyBroadcastEnable( true );
    if( m_ShowFriendType != eFriendListTypeNearby )
    {
        m_ShowFriendType = eFriendListTypeNearby;
        onShowFriendTypeChanged();
    }
}

//============================================================================
void AppletFriendListClient::slotIgnoredInfoButtonClicked( void )
{
    ActivityInformation* activityInfo = new ActivityInformation( m_MyApp, this, eInfoTypeIgnoredList );
    if( activityInfo )
    {
        activityInfo->show();
    }
}

//============================================================================
void AppletFriendListClient::slotFriendsInfoButtonClicked( void )
{
    ActivityInformation* activityInfo = new ActivityInformation( m_MyApp, this, eInfoTypeFriendsList );
    if( activityInfo )
    {
        activityInfo->show();
    }
}

//============================================================================
void AppletFriendListClient::slotNearbyInfoButtonClicked( void )
{
    ActivityInformation* activityInfo = new ActivityInformation( m_MyApp, this, eInfoTypeNearbyList );
    if( activityInfo )
    {
        activityInfo->show();
    }
}

//============================================================================
void AppletFriendListClient::updateUser( GuiUser* guiUser )
{
    ui.m_FriendListWidget->updateFriend( guiUser );
}

//============================================================================
void AppletFriendListClient::removeUser( VxGUID& onlineId )
{
    ui.m_FriendListWidget->removeFriend( onlineId );
}

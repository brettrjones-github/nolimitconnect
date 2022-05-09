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

#include "AppletNearbyListClient.h"
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
AppletNearbyListClient::AppletNearbyListClient(	AppCommon&		    app, 
												QWidget *			parent )
: AppletClientBase( OBJNAME_APPLET_NEARBY_LIST_CLIENT, app, parent )
{
    setAppletType( eAppletNearbyListClient );
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

    connect( ui.m_FriendsButton, SIGNAL(clicked()), this, SLOT(slotFriendsButtonClicked()) );
    connect( ui.m_FriendsInfoButton, SIGNAL(clicked()), this, SLOT(slotFriendsInfoButtonClicked()) );
    connect( ui.m_IgnoredButton, SIGNAL(clicked()), this, SLOT(slotIgnoredButtonClicked()) );
    connect( ui.m_IgnoredInfoButton, SIGNAL(clicked()), this, SLOT(slotIgnoredInfoButtonClicked()) );
    connect( ui.m_NearbyButton, SIGNAL(clicked()), this, SLOT(slotNearbyButtonClicked()) );
    connect( ui.m_NearbyInfoButton, SIGNAL(clicked()), this, SLOT(slotNearbyInfoButtonClicked()) );

    m_MyApp.activityStateChange( this, true );
    m_UserMgr.wantGuiUserUpdateCallbacks( this, true );
    if( !GuiHelpers::checkUserPermission( "android.permission.CHANGE_WIFI_MULTICAST_STATE" ) )
    {
        okMessageBox( QObject::tr( "Broadcast Permission" ), QObject::tr( "Cannot discover nearby users without Broadcast Permission" ) );
    }

    m_Engine.fromGuiNearbyBroadcastEnable( true );
    if( m_FriendListType != eUserViewTypeNearby )
    {
        m_FriendListType = eUserViewTypeNearby;
        onShowFriendTypeChanged();
    }
}

//============================================================================
AppletNearbyListClient::~AppletNearbyListClient()
{
    m_Engine.fromGuiNearbyBroadcastEnable( false );
    m_UserMgr.wantGuiUserUpdateCallbacks( this, false );
    m_MyApp.activityStateChange( this, false );
}

//============================================================================
void AppletNearbyListClient::setStatusLabel( QString strMsg )
{
    ui.m_StatusLabel->setText( strMsg );
}

//============================================================================
void AppletNearbyListClient::showEvent( QShowEvent * ev )
{
    ActivityBase::showEvent( ev );
    m_MyApp.wantToGuiActivityCallbacks( this, true );
}

//============================================================================
void AppletNearbyListClient::hideEvent( QHideEvent * ev )
{
    m_MyApp.wantToGuiActivityCallbacks( this, false );
    ActivityBase::hideEvent( ev );
}

//============================================================================
void AppletNearbyListClient::toGuiInfoMsg( char * infoMsg )
{
    QString infoStr( infoMsg );
#if QT_VERSION > QT_VERSION_CHECK(6,0,0)
    static QRegularExpression removeExpress( "[\\n\\r]" );
    infoStr.remove( removeExpress );
#else
    infoStr.remove(QRegExp("[\\n\\r]"));
#endif // QT_VERSION > QT_VERSION_CHECK(6,0,0)
    emit signalInfoMsg( infoStr );
}

//============================================================================
void AppletNearbyListClient::infoMsg( const char* errMsg, ... )
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
void AppletNearbyListClient::clearList( void )
{
    ui.m_UserListWidget->clear();
    setStatusLabel( GuiParams::describeUserViewType( m_FriendListType ) + QObject::tr( "List" ) );
}

//============================================================================
void AppletNearbyListClient::clearStatus( void )
{
    setStatusLabel( "" );
}

//============================================================================
void AppletNearbyListClient::onShowFriendList( void )
{
    std::vector<std::pair<VxGUID, int64_t>> friendList;
    FriendListMgr& friendMgr = m_Engine.getFriendListMgr();

    friendMgr.lockList();
    friendList = friendMgr.getIdentList();
    friendMgr.unlockList();
    
    updateFriendList( eUserViewTypeFriends, friendList );
}

//============================================================================
void AppletNearbyListClient::onShowIgnoreList( void )
{
    std::vector<std::pair<VxGUID, int64_t>> ignoreList;
    IgnoreListMgr& ignoreMgr = m_Engine.getIgnoreListMgr();

    ignoreMgr.lockList();
    ignoreList = ignoreMgr.getIdentList();
    ignoreMgr.unlockList();

    updateFriendList( eUserViewTypeIgnored, ignoreList );
}

//============================================================================
void AppletNearbyListClient::onShowNearbyList( void )
{
    std::vector<std::pair<VxGUID, int64_t>> nearbyList;
    NearbyListMgr& nearbyMgr = m_Engine.getNearbyListMgr();

    nearbyMgr.lockList();
    nearbyList = nearbyMgr.getIdentList();
    nearbyMgr.unlockList();

    updateFriendList( eUserViewTypeNearby, nearbyList );
}

//============================================================================
void AppletNearbyListClient::callbackIndentListUpdate( EUserViewType listType, VxGUID& onlineId, uint64_t timestamp )
{
}

//============================================================================
void AppletNearbyListClient::callbackIndentListRemove( EUserViewType listType, VxGUID& onlineId )
{
    if( listType == getListType() )
    {
        removeUser( onlineId );
    }
}

//============================================================================
void AppletNearbyListClient::callbackOnUserAdded( GuiUser* guiUser )
{
    updateUser( guiUser );
}

//============================================================================
void AppletNearbyListClient::callbackOnUserUpdated( GuiUser* guiUser )
{
    updateUser( guiUser );
}

//============================================================================
void AppletNearbyListClient::callbackOnUserRemoved( VxGUID& onlineId )
{
    removeUser( onlineId );
}

//============================================================================
void AppletNearbyListClient::onShowFriendTypeChanged( void )
{
    switch( m_FriendListType )
    {
    case eUserViewTypeIgnored:
        onShowIgnoreList();
        break;

    case eUserViewTypeNearby:
        onShowNearbyList();
        break;

    case eUserViewTypeFriends:
    default:
        onShowFriendList();
        break;
    }
}

//============================================================================
void AppletNearbyListClient::slotFriendsButtonClicked( void )
{
    m_Engine.fromGuiNearbyBroadcastEnable( false );
    if( m_FriendListType != eUserViewTypeFriends )
    {
        m_FriendListType = eUserViewTypeFriends;
        onShowFriendTypeChanged();
    }
}

//============================================================================
void AppletNearbyListClient::slotIgnoredButtonClicked( void )
{
    m_Engine.fromGuiNearbyBroadcastEnable( false );
    if( m_FriendListType != eUserViewTypeIgnored )
    {
        m_FriendListType = eUserViewTypeIgnored;
        onShowFriendTypeChanged();
    }
}

//============================================================================
void AppletNearbyListClient::slotNearbyButtonClicked( void )
{
    if( !GuiHelpers::checkUserPermission("android.permission.CHANGE_WIFI_MULTICAST_STATE") )
    {
        okMessageBox(QObject::tr("Broadcast Permission"), QObject::tr("Cannot discover nearby users without Broadcast Permission"));
        ui.m_FriendsButton->setFocus();
        slotFriendsButtonClicked();
        return;
    }

    if( m_FriendListType != eUserViewTypeNearby )
    {
        m_FriendListType = eUserViewTypeNearby;
        onShowFriendTypeChanged();
    }
}

//============================================================================
void AppletNearbyListClient::slotIgnoredInfoButtonClicked( void )
{
    ActivityInformation* activityInfo = new ActivityInformation( m_MyApp, this, eInfoTypeIgnoredList );
    if( activityInfo )
    {
        activityInfo->show();
    }
}

//============================================================================
void AppletNearbyListClient::slotFriendsInfoButtonClicked( void )
{
    ActivityInformation* activityInfo = new ActivityInformation( m_MyApp, this, eInfoTypeFriendsList );
    if( activityInfo )
    {
        activityInfo->show();
    }
}

//============================================================================
void AppletNearbyListClient::slotNearbyInfoButtonClicked( void )
{
    ActivityInformation* activityInfo = new ActivityInformation( m_MyApp, this, eInfoTypeNearbyList );
    if( activityInfo )
    {
        activityInfo->show();
    }
}

//============================================================================
void AppletNearbyListClient::updateUser( EUserViewType listType, VxGUID& onlineId )
{
    GuiUser* user = m_MyApp.getUserMgr().getOrQueryUser( onlineId );
    if( user )
    {
        updateUser( user );
    }  
}

//============================================================================
void AppletNearbyListClient::updateUser( GuiUser* guiUser )
{
    if( guiUser )
    {
        if( ( eUserViewTypeFriends == m_FriendListType && ( guiUser->isFriend() || guiUser->isAdmin() ) )
            || ( eUserViewTypeIgnored == m_FriendListType && guiUser->isIgnored() )
            || ( eUserViewTypeNearby == m_FriendListType && guiUser->isNearby() ) )
        {
            ui.m_UserListWidget->updateUser( guiUser );
        }
    }
}

//============================================================================
void AppletNearbyListClient::removeUser( VxGUID& onlineId )
{
    ui.m_UserListWidget->removeUser( onlineId );
}

//============================================================================
void AppletNearbyListClient::updateFriendList( EUserViewType listType, std::vector<std::pair<VxGUID, int64_t>> idList )
{
    clearList();
    m_FriendListType = listType;
    ui.m_UserListWidget->setUserViewType( listType );

    // for each see if we already have that ident as gui user else request it
    for( auto identTime : idList )
    {
        updateUser( listType, identTime.first );
    }
}

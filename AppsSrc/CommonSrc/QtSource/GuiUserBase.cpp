//============================================================================
// Copyright (C) 2021 Brett R. Jones
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

#include "GuiUserBase.h"

#include "AppCommon.h"
#include "GuiUserMgr.h"
#include "GuiParams.h"

//============================================================================
GuiUserBase::GuiUserBase( AppCommon& app )
    : QWidget( &app )
    , m_MyApp( app )
{
}

//============================================================================
GuiUserBase::GuiUserBase( AppCommon& app, VxNetIdent* netIdent, VxGUID& sessionId, bool online )
    : QWidget( &app )
    , m_MyApp( app )
    , m_NetIdent( *netIdent )
    , m_OnlineId( netIdent->getMyOnlineId() )
    , m_SessionId( sessionId )
{
    setOnlineStatus( online );
}


//============================================================================
GuiUserBase::GuiUserBase( const GuiUserBase& rhs )
    : QWidget()
    , m_MyApp( rhs.m_MyApp )
    , m_NetIdent( rhs.m_NetIdent )
    , m_OnlineId( rhs.m_OnlineId )
    , m_SessionId( rhs.m_SessionId )
    , m_IsOnline( rhs.m_IsOnline )
    , m_IsRelayed( rhs.m_IsRelayed )
    , m_HostSet( rhs.m_HostSet )
{
}

//============================================================================
bool GuiUserBase::isMyself( void )
{
    return getMyOnlineId() == m_MyApp.getMyOnlineId();
}

//============================================================================
bool GuiUserBase::updateIsRelayed( void )
{
    bool isRelayed = false;
    if( isMyself() )
    {
        isRelayed = false;
    }
    else
    {
        isRelayed = m_MyApp.getConnectIdListMgr().isRelayed( getMyOnlineId() );
    }

    // some functions use the net ident as isOnline
    m_NetIdent.setIsRelayed( isRelayed );
    m_IsRelayed = isRelayed;
    return isRelayed;
}

//============================================================================
bool GuiUserBase::updateIsOnline( void )
{
    bool isOnline = false;
    if( isMyself() )
    {
        isOnline = true;
    }
    else
    {
        isOnline = m_MyApp.getConnectIdListMgr().isOnline( getMyOnlineId() );
    }

    // some functions use the net ident as isOnline
    m_NetIdent.setIsOnline( isOnline );
    m_IsOnline = isOnline;
    return isOnline;
}

//============================================================================
EPluginAccess GuiUserBase::getMyAccessPermissionFromHim( EPluginType pluginType, bool inGroup )
{ 
    updateIsOnline();
    return m_NetIdent.getMyAccessPermissionFromHim( pluginType, inGroup ); 
}

//============================================================================
QString GuiUserBase::describeMyFriendshipToHim( bool inGroup )
{
    updateIsOnline();
    return GuiParams::describeFriendship( m_NetIdent.getMyFriendshipToHim(),  inGroup );
}

//============================================================================
QString GuiUserBase::describeHisFriendshipToMe( bool inGroup )
{
    updateIsOnline();
    return GuiParams::describeFriendship( m_NetIdent.getHisFriendshipToMe(),  inGroup );
}

//============================================================================
void GuiUserBase::setNetIdent( VxNetIdent* netIdent )
{ 
    if( netIdent )
    {
        m_NetIdent = *netIdent; 
        m_OnlineId = m_NetIdent.getMyOnlineId();
        updateIsOnline();
    }
    else
    {
        LogMsg( LOG_ERROR, "GuiUserBase::setNetIdent null ident" );
    }
}

//============================================================================
bool GuiUserBase::setRelayStatus( bool isRelayed )
{
    bool relayStateChanged = isRelayed != m_IsRelayed;
    if( relayStateChanged )
    {
        m_IsRelayed = isRelayed;
    }

    return relayStateChanged;
}

//============================================================================
bool GuiUserBase::setOnlineStatus( bool isOnline )
{
    bool onlineStateChanged = isOnline != m_IsOnline;
    if( onlineStateChanged )
    {
        m_IsOnline = isOnline;
        if( !m_IsOnline )
        {
            m_SessionId.clearVxGUID();
        }
    }

    return onlineStateChanged;
}

//============================================================================
bool GuiUserBase::isInSession( void )
{
    return m_IsOnline && m_SessionId.isVxGUIDValid();
}

//============================================================================
void GuiUserBase::addHostType( EHostType hostType )
{
    if( hostType > eHostTypeUnknown && hostType < eMaxHostType )
    {
        m_HostSet.insert( hostType );
    }
}

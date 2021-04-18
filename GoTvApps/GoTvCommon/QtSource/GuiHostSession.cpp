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

#include <app_precompiled_hdr.h>
#include "GuiHostSession.h"
#include "GuiHelpers.h"
#include "GuiParams.h"

#include <PktLib/VxSearchDefs.h>
#include <CoreLib/VxParse.h>
#include <CoreLib/VxFileInfo.h>

#include <QObject>

//============================================================================
GuiHostSession::GuiHostSession( QWidget* parent )
    : QWidget( parent )
{
}

//============================================================================
GuiHostSession::GuiHostSession( EHostType hostType, VxGUID& sessionId, VxNetIdent& hostIdent, PluginSetting& pluginSetting, QWidget* parent )
    : QWidget( parent )
    , m_HostType( hostType )
    , m_SessionId( sessionId )
    , m_HostIdent( hostIdent )
    , m_PluginSetting( pluginSetting )
    , m_OnlineId( hostIdent.getMyOnlineId() )
{
}

//============================================================================
GuiHostSession::GuiHostSession( const GuiHostSession &rhs )
    : QWidget()
    , m_HostType( rhs.m_HostType )
    , m_SessionId( rhs.m_SessionId )
    , m_HostIdent( rhs.m_HostIdent )
    , m_PluginSetting( rhs.m_PluginSetting )
    , m_OnlineId( rhs.m_OnlineId )
{
}

//============================================================================
GuiHostSession& GuiHostSession::operator =( const GuiHostSession &rhs )
{
	if( this != &rhs )   
	{
        m_HostType		        = rhs.m_HostType;
        m_SessionId				= rhs.m_SessionId;
        m_HostIdent		        = rhs.m_HostIdent;
        m_PluginSetting			= rhs.m_PluginSetting;
        m_OnlineId			    = rhs.m_OnlineId;
	}

	return *this;
}

//============================================================================
std::string GuiHostSession::getHostUrl( void )
{
    return m_HostIdent.getMyOnlineUrl();
}

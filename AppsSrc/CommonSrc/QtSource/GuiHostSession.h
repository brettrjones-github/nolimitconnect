#pragma once
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
// http://www.nolimitconnect.org
//============================================================================

#include <QWidget> // must be declared first or linux Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include <GuiInterface/IDefs.h>
#include <ptop_src/ptop_engine_src/PluginSettings/PluginSetting.h>

#include <CoreLib/VxGUID.h>

#include <QWidget>

class P2PEngine;
class VxNetIdent;
class GuiUser;

class GuiHostSession : public QWidget
{
public:
	GuiHostSession( QWidget* parent = nullptr );
	GuiHostSession(	EHostType hostType, VxGUID& sessionId, GuiUser* hostIdent, PluginSetting& pluginSetting, QWidget* parent = nullptr );
	GuiHostSession( const GuiHostSession &rhs );

	GuiHostSession&			    operator =( const GuiHostSession &rhs );

    GuiUser*                    getUserIdent( void )                        { return m_HostIdent; }
    EHostType                   getHostType( void )                         { return m_HostType; }
    std::string                 getHostUrl( void );

    VxGUID&					    getOnlineId( void )                         { return m_OnlineId; }

    PluginSetting&				getPluginSetting( void )                    { return m_PluginSetting; }
    void						setSessionId( VxGUID& sessionId )           { m_SessionId = sessionId; }
    VxGUID&					    getSessionId( void )                        { return m_SessionId; }

    void						setWidget( QWidget* widget )				{ m_Widget = widget; }
    QWidget*					getWidget( void )							{ return m_Widget; }

    std::string                 getHostDescription( void );
    VxGUID                      getHostThumbId( void );

protected:
	//=== vars ===//
    EHostType                   m_HostType{ eHostTypeUnknown };
	VxGUID					    m_SessionId;
    VxGUID					    m_OnlineId;
    GuiUser*			    	m_HostIdent{ nullptr };
    PluginSetting               m_PluginSetting;
    QWidget*					m_Widget{ nullptr };
};


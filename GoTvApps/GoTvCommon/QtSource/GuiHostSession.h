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
// http://www.nolimitconnect.com
//============================================================================

#include <GoTvInterface/IDefs.h>

#include <QWidget>

class P2PEngine;
class VxGUID;
class VxNetIdent;
class PluginSetting;

class GuiHostSession : public QWidget
{
public:
	GuiHostSession( QWidget* parent = nullptr );
	GuiHostSession(	EHostType hostType, VxGUID& sessionId, VxNetIdent& hostIdent, PluginSetting& pluginSetting, QWidget* parent = nullptr );
	GuiHostSession( const GuiHostSession &rhs );

	GuiHostSession&			    operator =( const GuiHostSession &rhs );

    VxNetIdent&					getHostIdent( void )                        { return m_HostIdent; }
    EHostType                   getHostType( void )                         { return m_HostType; }
    std::string                 getHostUrl( void );

    PluginSetting&				getPluginSetting( void )                    { return m_PluginSetting; }
    void						setSessionId( VxGUID& sessionId )           { m_SessionId = sessionId; }
    VxGUID&					    getSessionId( void )                        { return m_SessionId; }

    void						setWidget( QWidget * widget )				{ m_Widget = widget; }
    QWidget *					getWidget( void )							{ return m_Widget; }

protected:
	//=== vars ===//
    EHostType                   m_HostType{ eHostTypeUnknown };
	VxGUID					    m_SessionId;
    VxNetIdent			    	m_HostIdent;
    PluginSetting               m_PluginSetting;
    QWidget*					m_Widget{ nullptr };
};


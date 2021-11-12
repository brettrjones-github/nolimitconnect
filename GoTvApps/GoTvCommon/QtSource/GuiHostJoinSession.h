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

#include <GuiInterface/IDefs.h>

#include "GuiUser.h"

#include <QWidget>

class GuiHostJoin;
class P2PEngine;
class VxGUID;
class VxNetIdent;
class PluginSetting;

class GuiHostJoinSession : public QWidget
{
public:
	GuiHostJoinSession( QWidget* parent = nullptr );
	GuiHostJoinSession(	EHostType hostType, GuiHostJoin* hostJoinIdent, QWidget* parent = nullptr );
	GuiHostJoinSession( const GuiHostJoinSession &rhs );

	GuiHostJoinSession&			operator =( const GuiHostJoinSession &rhs );

    GuiHostJoin*                getUserIdent( void )                        { return m_HostIdent; }
    EHostType                   getHostType( void )                         { return m_HostType; }
    std::string                 getHostUrl( void );

    VxGUID&					    getOnlineId( void )                         { return m_OnlineId; }

    void						setWidget( QWidget * widget )				{ m_Widget = widget; }
    QWidget *					getWidget( void )							{ return m_Widget; }

    std::string                 getHostDescription( void );
    VxGUID                      getHostThumbId( void );

protected:
	//=== vars ===//
    EHostType                   m_HostType{ eHostTypeUnknown };
    VxGUID					    m_OnlineId;
    GuiHostJoin*			    m_HostIdent{ nullptr };
    QWidget*					m_Widget{ nullptr };
};


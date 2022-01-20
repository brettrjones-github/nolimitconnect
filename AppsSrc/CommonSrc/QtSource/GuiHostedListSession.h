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

#include "GuiHosted.h"

#include <QWidget>

class GuiUser;

class GuiHostedListSession : public QWidget
{
public:
	GuiHostedListSession( QWidget* parent = nullptr );
	GuiHostedListSession( HostedId& hostedId, GuiHosted* guiHosted, QWidget* parent = nullptr );
	GuiHostedListSession( const GuiHostedListSession &rhs );

	GuiHostedListSession&		operator =( const GuiHostedListSession &rhs );

    HostedId&                   getHostedId( void )                         { return m_HostedId; }
    GuiHosted*                  getHosted( void )                           { return m_GuiHosted; }
    GuiUser*                    getUserIdent( void )                        { return m_GuiHosted ? m_GuiHosted->getUser() : nullptr; }
    EHostType                   getHostType( void )                         { return m_HostedId.getHostType(); }
    VxGUID&					    getOnlineId( void )                         { return m_HostedId.getOnlineId(); }

    std::string                 getHostUrl( void );
    std::string                 getHostTitle( void );
    std::string                 getHostDescription( void );

    void						setSessionId( VxGUID& sessionId )           { m_SessionId = sessionId; }
    VxGUID&                     getSessionId( void );

    void						setWidget( QWidget* widget )                { m_Widget = widget; }
    QWidget*                    getWidget( void )                           { return m_Widget; }

    VxGUID                      getHostThumbId( void );

protected:
	//=== vars ===//
    HostedId                    m_HostedId;
    GuiHosted*                  m_GuiHosted{ nullptr };
	VxGUID					    m_SessionId;

    QWidget*					m_Widget{ nullptr };
};


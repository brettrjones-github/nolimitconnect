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

#include "GuiGroupie.h"

#include <QWidget>

class GuiUser;

class GuiGroupieListSession : public QWidget
{
public:
	GuiGroupieListSession( QWidget* parent = nullptr );
	GuiGroupieListSession( GroupieId& hostedId, GuiGroupie* guiGroupie, QWidget* parent = nullptr );
	GuiGroupieListSession( const GuiGroupieListSession &rhs );

	GuiGroupieListSession&		operator =( const GuiGroupieListSession &rhs );

    GroupieId&                  getGroupieId( void )                        { return m_GroupieId; }
    GuiGroupie*                 getGroupie( void )                          { return m_GuiGroupie; }
    GuiUser*                    getUserIdent( void )                        { return m_GuiGroupie ? m_GuiGroupie->getUser() : nullptr; }
    EHostType                   getHostType( void )                         { return m_GroupieId.getHostType(); }
    VxGUID&					    getGroupieOnlineId( void )                  { return m_GroupieId.getGroupieOnlineId(); }
    VxGUID&                     getHostOnlineId( void )                     { return m_GroupieId.getHostOnlineId(); }

    std::string                 getGroupieUrl( void );
    std::string                 getGroupieTitle( void );
    std::string                 getGroupieDescription( void );

    void						setSessionId( VxGUID& sessionId )           { m_SessionId = sessionId; }
    VxGUID&					    getSessionId( void )                        { return m_SessionId; }

    void						setWidget( QWidget* widget )                { m_Widget = widget; }
    QWidget*                    getWidget( void )                           { return m_Widget; }

    VxGUID                      getHostThumbId( void );

protected:
	//=== vars ===//
    GroupieId                   m_GroupieId;
    GuiGroupie*                 m_GuiGroupie{ nullptr };
	VxGUID					    m_SessionId;

    QWidget*					m_Widget{ nullptr };
};


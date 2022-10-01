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

#include <GuiInterface/IDefs.h>

#include "GuiUser.h"

#include <PktLib/GroupieId.h>

#include <QWidget>

class GuiHostJoin;
class P2PEngine;
class VxNetIdent;

class GuiHostJoinSession : public QWidget
{
public:
	GuiHostJoinSession( QWidget* parent = nullptr );
	GuiHostJoinSession(	GuiHostJoin* hostJoinIdent, QWidget* parent = nullptr );
	GuiHostJoinSession( const GuiHostJoinSession &rhs );

	GuiHostJoinSession&			operator =( const GuiHostJoinSession &rhs );

    GuiUser*                    getGuiUser( void );

    void                        setHostJoin( GuiHostJoin* guiHostJoin );
    GuiHostJoin*                getHostJoin( void )                         { return m_GuiHostJoin; }

    void						setGroupieId( GroupieId& groupieId )        { m_GroupieId = groupieId; }
    GroupieId&                  getGroupieId( void )                        { return m_GroupieId; }
    EHostType                   getHostType( void );

    void						setSessionId( VxGUID& sessionId )           { m_SessionId = sessionId; }
    VxGUID&                     getSessionId( void )                        { return m_SessionId; }

    std::string                 getGroupieUrl( void );
    std::string                 getHostUrl( void );

    void						setWidget( QWidget* widget )				{ m_Widget = widget; }
    QWidget*					getWidget( void )							{ return m_Widget; }

    std::string                 getOnlineName( void )                       { return getGuiUser() ? getGuiUser()->getOnlineName() : ""; }
    std::string                 getHostDescription( void );
    VxGUID                      getHostThumbId( void );

protected:
	//=== vars ===//
    GroupieId                   m_GroupieId;
    GuiHostJoin*			    m_GuiHostJoin{ nullptr };
    VxGUID					    m_SessionId;
    QWidget*					m_Widget{ nullptr };
};


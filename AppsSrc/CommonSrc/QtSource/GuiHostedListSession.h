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

#include <QWidget> // must be declared first or linux Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include "GuiHosted.h"

class GuiUser;

class GuiHostedListSession : public QWidget
{
public:
	GuiHostedListSession( QWidget* parent = nullptr );
	GuiHostedListSession( HostedId& hostedId, GuiHosted* guiHosted, QWidget* parent = nullptr );
	GuiHostedListSession( const GuiHostedListSession &rhs );

	GuiHostedListSession&		operator =( const GuiHostedListSession &rhs );

   
    void                        setGuiHosted( GuiHosted* guiHosted )        { m_GuiHosted = guiHosted; if( guiHosted->getUser() ) setGuiUser( guiHosted->getUser() ); }
    GuiHosted*                  getGuiHosted( void )                        { return m_GuiHosted; }

    HostedId&                   getHostedId( void )                         { return m_HostedId; }
    VxGUID&                     getOnlineId( void )                         { return m_HostedId.getOnlineId(); }
    EHostType                   getHostType( void )                         { return m_HostedId.getHostType(); }

    void                        setGuiUser( GuiUser* guiUser )              { if( m_GuiHosted && guiUser ) m_GuiHosted->setUser( guiUser ); }
    GuiUser*                    getGuiUser( void )                          { return m_GuiHosted ? m_GuiHosted->getUser() : nullptr; }

    std::string                 getHostUrl( void );
    std::string                 getHostTitle( void );
    std::string                 getHostDescription( void );

    void						setSessionId( VxGUID& sessionId )           { m_SessionId = sessionId; }
    VxGUID&                     getSessionId( void );

    void						setWidget( QWidget* widget )                { m_Widget = widget; }
    QWidget*                    getWidget( void )                           { return m_Widget; }

    VxGUID                      getHostThumbId( void );

    void                        updateUser( GuiUser* guiUser );

protected:
	//=== vars ===//
    GuiHosted*                  m_GuiHosted{ nullptr };
    HostedId                    m_HostedId;
    VxGUID					    m_SessionId;

    QWidget*					m_Widget{ nullptr };
};


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

#include "GuiGroupieListSession.h"
#include "GuiUser.h"

//============================================================================
GuiGroupieListSession::GuiGroupieListSession( QWidget* parent )
    : QWidget( parent )
{
}

//============================================================================
GuiGroupieListSession::GuiGroupieListSession( GroupieId& hostedId, GuiGroupie* guiGroupie, QWidget* parent )
    : QWidget( parent )
    , m_GroupieId( hostedId )
    , m_GuiGroupie( guiGroupie )
{
}

//============================================================================
GuiGroupieListSession::GuiGroupieListSession( const GuiGroupieListSession &rhs )
    : QWidget()
    , m_GroupieId( rhs.m_GroupieId )
    , m_GuiGroupie( rhs.m_GuiGroupie )
    , m_SessionId( rhs.m_SessionId )
{
}

//============================================================================
GuiGroupieListSession& GuiGroupieListSession::operator =( const GuiGroupieListSession &rhs )
{
	if( this != &rhs )   
	{
        m_GroupieId              = rhs.m_GroupieId;
        m_SessionId				= rhs.m_SessionId;
        m_GuiGroupie             = rhs.m_GuiGroupie;
	}

	return *this;
}

//============================================================================
std::string GuiGroupieListSession::getGroupieUrl( void )
{
    if( m_GuiGroupie )
    {
        return m_GuiGroupie->getGroupieUrl();
    }

    return "";
}

//============================================================================
std::string GuiGroupieListSession::getGroupieTitle( void )
{
    if( m_GuiGroupie )
    {
        return m_GuiGroupie->getGroupieTitle();
    }

    return "";
}

//============================================================================
std::string GuiGroupieListSession::getGroupieDescription( void )
{
    if( m_GuiGroupie )
    {
        return m_GuiGroupie->getGroupieDescription();
    }

    return "";
}

//============================================================================
VxGUID GuiGroupieListSession::getHostThumbId( void )
{
    if( m_GuiGroupie && m_GuiGroupie->getUser() )
    {
        return m_GuiGroupie->getUser()->getHostThumbId( m_GroupieId.getHostType(), true );
    }

    VxGUID emptyThumbId;
    return emptyThumbId;
}

//============================================================================
void GuiGroupieListSession::updateUser( GuiUser* guiUser )
{
    if( m_GuiGroupie && m_GuiGroupie->getUser() != guiUser )
    {
        return m_GuiGroupie->setUser( guiUser );
    }
}

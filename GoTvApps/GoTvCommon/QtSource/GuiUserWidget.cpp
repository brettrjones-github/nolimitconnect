//============================================================================
// Copyright (C) 2021 Brett R. Jones
// Issued to MIT style license by Brett R. Jones in 2017
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

#include "GuiUserWidget.h"

//============================================================================
GuiUserWidget::GuiUserWidget( VxNetIdent * netIdent, VxGUID& sessionId, bool online )
    : m_NetIdent( *netIdent )
    , m_SessionId( sessionId )
    , m_IsOnline( online )
{
}

//============================================================================
GuiUserWidget::GuiUserWidget( const GuiUserWidget& rhs )
    : m_NetIdent( rhs.m_NetIdent )
    , m_SessionId( rhs.m_SessionId )
    , m_IsOnline( rhs.m_IsOnline )
{
}

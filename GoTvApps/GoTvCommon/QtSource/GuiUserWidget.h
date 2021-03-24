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

#include <QWidget>

#include <PktLib/VxCommon.h>
#include <CoreLib/VxGUID.h>

class VxNetIdent;
class VxSktBase;

class GuiUserWidget : public QWidget
{
    Q_OBJECT
public:
    GuiUserWidget() = default;
    GuiUserWidget( VxNetIdent * netIdent, VxGUID& sessionId, bool online = false );
    GuiUserWidget( const GuiUserWidget& rhs );
	virtual ~GuiUserWidget() = default;

    void                        setNetIdent( VxNetIdent* netIdent )     { m_NetIdent = *netIdent; }
    VxNetIdent&                 getNetIdent( void )                     { return m_NetIdent; }
    void                        setSessionId( VxGUID& sessionId )       { m_SessionId = sessionId; }
    VxGUID&                     getSessionId( void )                    { return m_SessionId; }

    void                        setIsOnline( bool isOnline )            { m_IsOnline = isOnline; }
    bool                        getIsOnline( void )                     { return m_IsOnline; }

protected:
    VxNetIdent                  m_NetIdent;
    VxGUID                      m_SessionId;
    bool                        m_IsOnline{ false };
};

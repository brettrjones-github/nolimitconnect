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

#include <CoreLib/VxGUID.h>

class VxNetIdent;
class VxSktBase;

class User
{
public:
    User();
    User(VxNetIdent * netIdent, VxSktBase * sktBase);
	virtual ~User();

    void                        setNetIdent( VxNetIdent * netIdent )    { m_NetIdent = netIdent; }
    VxNetIdent *                getNetIdent( void )                     { return m_NetIdent; }
    void                        setSktBase( VxSktBase * sktBase )       { m_SktBase = sktBase; }
    VxSktBase *                 getSktBase ( void )                     { return m_SktBase; }

protected:
    VxNetIdent *                m_NetIdent{ nullptr };
    VxSktBase *                 m_SktBase{ nullptr };
};

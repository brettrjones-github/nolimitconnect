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

class GuiUserMgrGuiUserUpdateInterface;

class GuiUserMgrGuiUserUpdateClient
{
public:
    GuiUserMgrGuiUserUpdateClient( GuiUserMgrGuiUserUpdateInterface* callback )
        : m_Callback( callback )
    {
    }

    GuiUserMgrGuiUserUpdateClient( const GuiUserMgrGuiUserUpdateClient& rhs )
        : m_Callback( rhs.m_Callback )
    {
    }

    GuiUserMgrGuiUserUpdateClient& operator =( const GuiUserMgrGuiUserUpdateClient& rhs )
    {
        if( this != &rhs )
        {
            m_Callback = rhs.m_Callback;
        }

        return *this;
    }

    //=== vars ===//
    GuiUserMgrGuiUserUpdateInterface* m_Callback{ nullptr };
};


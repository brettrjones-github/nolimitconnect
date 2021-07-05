//============================================================================
// Copyright (C) 2020 Brett R. Jones 
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

#include "HostConnectInterface.h"

//============================================================================
bool IHostConnectCallback::hasHostConnectType( EHostConnectType hostConnectType )
{
    bool found = false;
    for( auto iter = m_ConnectTypeList.begin(); iter != m_ConnectTypeList.end(); ++iter )
    {
        if( *iter == hostConnectType )
        {
            found = true;
            break;
        }
    }

    return found;
}

//============================================================================
void IHostConnectCallback::addHostConnectType( EHostConnectType hostConnectType )
{
    if( !hasHostConnectType( hostConnectType ) )
    {
        m_ConnectTypeList.push_back( hostConnectType );
    }
}

//============================================================================
void IHostConnectCallback::removeConnectType( EHostConnectType hostConnectType )
{
    for( auto iter = m_ConnectTypeList.begin(); iter != m_ConnectTypeList.end(); ++iter )
    {
        if( *iter == hostConnectType )
        {
            m_ConnectTypeList.erase( iter );
            break;
        }
    }
}

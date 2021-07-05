//============================================================================
// Copyright (C) 2020 Brett R. Jones
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

#include "HostDefs.h"

namespace
{
	const char * g_HostActions[] = {"Announce","RelaysOnly","List","RandomConnect","RandomConnectRelay" };
    const char * g_OtherHostType[] = { "Host Unknown", "Network Host", "Connect Test", "Group Host", "Chat Host", "Peer User" };
}

//============================================================================
EHostAction HostActionStringToEnum( const char * action )
{
	if( 0 == strcmp( action, "Announce" ) )
	{
		return eHostActionAnnounce;
	}
	else if( 0 == strcmp( action, "List" ) )
	{
		return eHostActionList;
	}
	else if( 0 == strcmp( action, "RelaysOnly" ) )
	{
		return eHostActionRelaysOnly;
	}
    else if( 0 == strcmp( action, "RandomConnect" ) )
    {
        return eHostActionRandomConnect;
    }
    else if( 0 == strcmp( action, "RandomConnectRelay" ) )
    {
        return eHostActionRandomConnectRelay;
    }
	else
	{
		return eHostActionUnknown;
	}
}

//============================================================================
const char *	HostActionEnumToString( EHostAction eHostAction )
{
    if( eHostAction < eMaxHostAction && eHostAction >= 0 )
    {
        return g_HostActions[ eHostAction ];
    }

    return "UNKNOWN HOST ACTION";
}


//============================================================================
EOtherHostType OtherHostTypeStringToEnum( const char * hostType )
{
    if( 0 == strcmp( hostType, "Host Unknown" ) )
    {
        return eOtherHostUnknown;
    }
    else if( 0 == strcmp( hostType, "Network Host" ) )
    {
        return eOtherHostNetworkHost;
    }
    else if( 0 == strcmp( hostType, "Connect Test" ) )
    {
        return eOtherHostConnectTest;
    }
    else if( 0 == strcmp( hostType, "Group Host" ) )
    {
        return eOtherHostGroupHost;
    }
    else if( 0 == strcmp( hostType, "Chat Host" ) )
    {
        return eOtherHostChatHost;
    }
    else if( 0 == strcmp( hostType, "Peer User" ) )
    {
        return eOtherPeerUserHost;
    }
    else
    {
        return eOtherHostUnknown;
    }
}

//============================================================================
const char *	OtherHostTypeEnumToString( EOtherHostType eHostType )
{
    if( eHostType < eMaxOtherHostType && eHostType >= 0 )
    {
        return g_OtherHostType[ eHostType ];
    }

    return "UNKNOWN HOST TYPE";
}

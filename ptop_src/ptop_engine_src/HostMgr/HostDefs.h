//============================================================================
// Copyright (C) 2019 Brett R. Jones
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
#pragma once

#include "config_appcorelibs.h"

enum EOtherHostType
{
    eOtherHostUnknown = 0,
    eOtherHostNetworkHost = 1,
    eOtherHostConnectTest = 2,
    eOtherHostGroupHost = 3,
    eOtherHostChatHost = 4,
    eOtherPeerUserHost = 5,

    eMaxOtherHostType
};

enum EHostAction
{
	eHostActionUnknown					= 0,
	eHostActionAnnounce					= 1,
	eHostActionRelaysOnly				= 2,
	eHostActionList						= 3,
	eHostActionRandomConnect			= 4,
    eHostActionRandomConnectRelay       = 5,

	eMaxHostAction
};

EOtherHostType			        OtherHostTypeStringToEnum( const char * hostType );
const char *			        OtherHostTypeEnumToString( EOtherHostType eHostType );

EHostAction			            HostActionStringToEnum( const char * action );
const char *			        HostActionEnumToString( EHostAction eHostAction );



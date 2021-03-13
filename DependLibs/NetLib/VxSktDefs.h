#pragma once
//============================================================================
// Copyright (C) 2008 Brett R. Jones 
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

#include <GoTvInterface/IDefs.h>

class VxSktBase;

typedef void (*VX_SKT_CALLBACK)(VxSktBase *, void *);

#define MAX_DISCONNECTED_SKT_TO_KEEP_AROUND				50

enum ESktMgrType
{
	eSktMgrTypeNone				= 0,
	eSktMgrTypeTcpConnect		= 1,
	eSktMgrTypeTcpAccept		= 2,

	eMaxSktMgrType			// always last
};

enum ESktCallbackReason
{
	eSktCallbackReasonUnknown		= 0,
	eSktCallbackReasonConnecting	= 1,
	eSktCallbackReasonConnectError	= 2,
	eSktCallbackReasonConnected		= 3,
	eSktCallbackReasonData			= 4,
	eSktCallbackReasonClosing		= 5,
	eSktCallbackReasonClosed		= 6,
	eSktCallbackReasonError			= 7,
	eSktCallbackReasonNewMgr		= 8,	// socket is being transfered to new manager

	eMaxSktCallbackReason			// always last
};

enum EIpProtocolType
{
	eIpProtocolTypeInvalid	= 0,
	eIpProtocolTypeUdp		= 1,
	eIpProtocolTypeTcp		= 2,
	eMaxIpProtocolType		= 3
};

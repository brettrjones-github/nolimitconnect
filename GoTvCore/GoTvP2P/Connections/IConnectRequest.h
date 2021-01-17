#pragma once
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

#include <vector>

#include <GoTvInterface/IDefs.h>

class VxSktBase;
class VxGUID;

class IConnectRequestCallback
{
public:
    virtual void                onUrlActionQueryIdSuccess( std::string& url, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) = 0;
    virtual void                onUrlActionQueryIdFail( std::string& url, ERunTestStatus testStatus, EConnectReason connectReason = eConnectReasonUnknown ) = 0;

    /// returns false if one time use and packet has been sent. Connect Manager will disconnect if nobody else needs the connection
    virtual bool                onContactConnected( VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) = 0;
    virtual void                onConnectRequestFail( VxGUID& onlineId, EConnectStatus connectStatus, EConnectReason connectReason = eConnectReasonUnknown ) = 0;
    virtual void                onContactDisconnected( VxSktBase* sktBased, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) = 0;
};

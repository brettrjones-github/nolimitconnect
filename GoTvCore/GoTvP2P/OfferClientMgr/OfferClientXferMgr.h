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

#include <GoTvCore/GoTvP2P/OfferBase/OfferBaseXferMgr.h>

#include "OfferClientXferDb.h"
#include "OfferClientInfo.h"

#include <GoTvInterface/IDefs.h>

#include <PktLib/VxCommon.h>
#include <CoreLib/VxThread.h>

#include <map>

class PluginMessenger;
class PluginSessionMgr;
class PluginMgr;
class VxPktHdr;
class OfferClientRxSession;
class OfferClientTxSession;
class OfferClientMgr;
class IToGui;
class P2PEngine;
class VxSha1Hash;

class PktOfferSendReq;
class PktOfferSendReply;
class PktOfferChunkReq;
class PktOfferSendCompleteReq;
class PktOfferClientListReply;

class OfferClientXferMgr : public OfferBaseXferMgr
{
public:
	typedef std::map<VxGUID, OfferClientRxSession *>::iterator OfferClientRxIter;
	typedef std::vector<OfferClientTxSession *>::iterator OfferClientTxIter;

	OfferClientXferMgr( P2PEngine& engine, OfferClientMgr& offerMgr, PluginMessenger& plugin, PluginSessionMgr&	pluginSessionMgr );
    virtual ~OfferClientXferMgr() = default;
};



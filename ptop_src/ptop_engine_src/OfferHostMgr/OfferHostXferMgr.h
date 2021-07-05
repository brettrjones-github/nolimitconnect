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

#include <ptop_src/ptop_engine_src/OfferBase/OfferBaseXferMgr.h>

#include "OfferHostXferDb.h"
#include "OfferHostInfo.h"

#include <PktLib/VxCommon.h>
#include <CoreLib/VxThread.h>

#include <map>

class PluginMessenger;
class PluginSessionMgr;
class PluginMgr;
class VxPktHdr;
class OfferHostRxSession;
class OfferHostTxSession;
class OfferHostMgr;
class OfferBaseMgr;
class IToGui;
class P2PEngine;
class VxSha1Hash;

class PktOfferSendReq;
class PktOfferSendReply;
class PktOfferChunkReq;
class PktOfferSendCompleteReq;
class PktOfferHostListReply;

class OfferHostXferMgr : public OfferBaseXferMgr
{
public:
	typedef std::map<VxGUID, OfferHostRxSession *>::iterator OfferHostRxIter;
	typedef std::vector<OfferHostTxSession *>::iterator OfferHostTxIter;

	OfferHostXferMgr( P2PEngine& engine, OfferHostMgr offerMgr, PluginMessenger& plugin, PluginSessionMgr&	pluginSessionMgr, const char * stateDbName );
	virtual ~OfferHostXferMgr() = default;
};




#pragma once
//============================================================================
// Copyright (C) 2013 Brett R. Jones
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
// http://www.nolimitconnect.org
//============================================================================

#include <ptop_src/ptop_engine_src/AssetBase/AssetBaseXferMgr.h>

class PktBlobSendReq;
class PktBlobSendReply;
class PktBlobChunkReq;
class PktBlobSendCompleteReq;
class PktBlobListReply;

class BlobXferMgr : public AssetBaseXferMgr
{
public:
	BlobXferMgr( P2PEngine& engine, AssetBaseMgr& assetMgr, BaseXferInterface& xferInterface, const char * stateDbName, const char * workThreadName );
	virtual ~BlobXferMgr() = default;

	virtual void				onPktBlobSendReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktBlobSendReply			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktBlobChunkReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktBlobChunkReply		    ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktBlobSendCompleteReq	( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktBlobSendCompleteReply	( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktBlobXferErr			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );

};




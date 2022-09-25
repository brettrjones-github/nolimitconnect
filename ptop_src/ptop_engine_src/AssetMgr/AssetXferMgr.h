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

class PktAssetSendReq;
class PktAssetSendReply;
class PktAssetChunkReq;
class PktAssetSendCompleteReq;
class PktAssetListReply;

class AssetBaseMgr;

class AssetXferMgr : public AssetBaseXferMgr
{
public:
	AssetXferMgr( P2PEngine& engine, AssetBaseMgr& assetMgr, BaseXferInterface& xferInterface, const char * stateDbName, const char * workThreadName );
	virtual ~AssetXferMgr() = default;

    virtual void				onPktAssetGetReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktAssetGetReply			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktAssetSendReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktAssetSendReply			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktAssetChunkReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktAssetChunkReply		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktAssetGetCompleteReq	( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktAssetGetCompleteReply	( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktAssetSendCompleteReq	( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktAssetSendCompleteReply	( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktAssetXferErr			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );

    virtual PktBaseGetReq*			    createPktBaseGetReq( void ) override;
    virtual PktBaseGetReply*			createPktBaseGetReply( void ) override;
    virtual PktBaseSendReq*			    createPktBaseSendReq( void ) override;
    virtual PktBaseSendReply*			createPktBaseSendReply( void ) override;
    virtual PktBaseChunkReq*			createPktBaseChunkReq( void ) override;
    virtual PktBaseChunkReply*          createPktBaseChunkReply( void ) override;
    virtual PktBaseGetCompleteReq*		createPktBaseGetCompleteReq( void ) override;
    virtual PktBaseGetCompleteReply*	createPktBaseGetCompleteReply( void ) override;
    virtual PktBaseSendCompleteReq*		createPktBaseSendCompleteReq( void ) override;
    virtual PktBaseSendCompleteReply*	createPktBaseSendCompleteReply( void ) override;
    virtual PktBaseXferErr*			    createPktBaseXferErr( void ) override;


};




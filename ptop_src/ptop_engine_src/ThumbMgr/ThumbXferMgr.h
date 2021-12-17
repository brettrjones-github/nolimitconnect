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

#include <ptop_src/ptop_engine_src/AssetBase/AssetBaseXferMgr.h>

class PktThumbSendReq;
class PktThumbSendReply;
class PktThumbChunkReq;
class PktThumbSendCompleteReq;
class PktThumbListReply;

class ThumbXferMgr : public AssetBaseXferMgr
{
public:
	ThumbXferMgr( P2PEngine& engine, AssetBaseMgr& assetMgr, BaseXferInterface& xferInterface, const char * stateDbName, const char * workThreadName );
	virtual ~ThumbXferMgr() = default;

    virtual void				sendToGuiAssetAction( EAssetAction assetAction, VxGUID& assetId, int pos0to100000 ) override {}; // dont send so is not in uploads/downloads

    virtual void				onPktThumbGetReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
    virtual void				onPktThumbGetReply			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
    virtual void				onPktThumbSendReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktThumbSendReply			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktThumbChunkReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktThumbChunkReply		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
    virtual void				onPktThumbGetCompleteReq	( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
    virtual void				onPktThumbGetCompleteReply	( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktThumbSendCompleteReq	( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktThumbSendCompleteReply	( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktThumbXferErr			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );

    virtual PktBaseGetReq*			    createPktBaseGetReq( void ) override;
    virtual PktBaseGetReply*			createPktBaseGetReply( void ) override;
    virtual PktBaseSendReq*			    createPktBaseSendReq( void ) override;
    virtual PktBaseSendReply*			createPktBaseSendReply( void ) override;
    virtual PktBaseChunkReq*			createPktBaseChunkReq( void ) override;
    virtual PktBaseChunkReply*			createPktBaseChunkReply( void ) override;
    virtual PktBaseGetCompleteReq*		createPktBaseGetCompleteReq( void ) override;
    virtual PktBaseGetCompleteReply*	createPktBaseGetCompleteReply( void ) override;
    virtual PktBaseSendCompleteReq*		createPktBaseSendCompleteReq( void ) override;
    virtual PktBaseSendCompleteReply*	createPktBaseSendCompleteReply( void ) override;
    virtual PktBaseXferErr*			    createPktBaseXferErr( void ) override;
    virtual PktBaseListReq*			    createPktBaseListReq( void ) override;
    virtual PktBaseListReply*			createPktBaseListReply( void ) override;

    virtual bool				        requestPluginThumb( VxSktBase* sktBase, VxNetIdent* netIdent, VxGUID& thumbId );
};




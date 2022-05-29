#pragma once
//============================================================================
// Copyright (C) 2010 Brett R. Jones
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

#include <PktLib/VxCommon.h>

#include "PluginBase.h"
#include "PluginSessionMgr.h"
#include "FileXferMgr.h"

class PluginBaseWebClient : public PluginBase
{
public:
	PluginBaseWebClient( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, EPluginType pluginType );
	virtual ~PluginBaseWebClient() = default;


	virtual bool				fromGuiMakePluginOffer( VxNetIdent *	netIdent,				
														int				pvUserData,
														const char *	pOfferMsg,				
														const char *	pFileName = NULL,
                                                        uint8_t *		fileHashId = 0,
                                                        VxGUID			lclSessionId = VxGUID::nullVxGUID() ) override;

	virtual bool				fromGuiOfferReply(	VxNetIdent *	netIdent,
													int				pvUserdata,				
													EOfferResponse	eOfferResponse,
                                                    VxGUID			lclSessionId ) override;

    virtual	void				fromGuiStartPluginSession( PluginSessionBase * poOffer ) override;
    virtual int					fromGuiDeleteFile( const char * fileName, bool shredFile ) override;
    virtual void				fromGuiCancelDownload( VxGUID& lclSessionId ) override;
    virtual void				fromGuiCancelUpload( VxGUID& lclSessionId ) override;

protected:
    virtual void				onPktPluginOfferReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) override;
    virtual void				onPktPluginOfferReply		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) override;

    virtual void				onPktFileGetReq				( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) override;
    virtual void				onPktFileGetReply			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) override;
    virtual void				onPktFileSendReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) override;
    virtual void				onPktFileSendReply			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) override;
    virtual void				onPktFileChunkReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) override;
    virtual void				onPktFileChunkReply			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) override;
    virtual void				onPktFileGetCompleteReq		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) override;
    virtual void				onPktFileGetCompleteReply	( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) override;
    virtual void				onPktFileSendCompleteReq	( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) override;
    virtual void				onPktFileSendCompleteReply	( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) override;

    void						onPktFindFileReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) override;
    void						onPktFindFileReply			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) override;
    void						onPktFileListReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) override;
    void						onPktFileListReply			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) override;

	//override so can create FileXferPluginSession
    virtual P2PSession *		createP2PSession( VxGUID& lclSessionId, VxSktBase * sktBase, VxNetIdent * netIdent ) override;
    virtual	void				onConnectionLost( VxSktBase * sktBase ) override;
    virtual void				replaceConnection( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt ) override;
    virtual void				onContactWentOffline( VxNetIdent * netIdent, VxSktBase * sktBase ) override;
    virtual void				onSessionStart( PluginSessionBase * session, bool pluginIsLocked ) override;

	void						cancelXferSession( VxGUID& lclSessionId );

	//=== vars ===//
	PluginSessionMgr			m_PluginSessionMgr;
	FileXferMgr					m_FileXferMgr;
};

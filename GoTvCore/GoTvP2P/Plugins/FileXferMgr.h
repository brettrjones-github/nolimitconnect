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
// http://www.nolimitconnect.com
//============================================================================

#include "FileXferBaseMgr.h"
#include "FileXferPluginSession.h"
#include "PluginSessionMgr.h"

#include <CoreLib/VxFileUtil.h>

class PktFileSendReq;
class PktFileChunkReply;
class PktFileChunkReq;
class PktFileSendCompleteReq;
class FileXferPluginSession;

class FileXferMgr : public FileXferBaseMgr
{
public:
	FileXferMgr( P2PEngine& engine, PluginBase& plugin, PluginSessionMgr& sessionMgr );
	virtual ~FileXferMgr() = default;

	virtual	void				fromGuiStartPluginSession( bool pluginIsLocked, PluginSessionBase * poOffer );

	virtual EXferError			beginFileSend( bool pluginIsLocked, VxSktBase * sktBase, FileXferPluginSession * poSession );

	virtual void				onPktFileGetReq				( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktFileGetReply			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktFileSendReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktFileSendReply			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktFileChunkReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktFileChunkReply			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktFileGetCompleteReq		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktFileGetCompleteReply	( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktFileSendCompleteReq	( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktFileSendCompleteReply	( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktFindFileReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktFindFileReply			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktFileListReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktFileListReply			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktFileInfoReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktFileInfoReply			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktFileInfoErr			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );

protected:
	virtual void				onFileSent( FileXferPluginSession * poSession, const char * pFileName, EXferError xferError, bool pluginIsLocked );
	virtual void				onFileReceived( FileXferPluginSession * poSession, const char * pFileName, EXferError xferError, bool pluginIsLocked );
	virtual bool				endFileXferRxSession( FileXferPluginSession * xferSession, bool isPluginLocked );
	virtual bool				endFileXferTxSession( FileXferPluginSession * xferSession, bool isPluginLocked );

	virtual	EXferError			beginFileReceive( bool pluginIsLocked, VxSktBase * sktBase, FileXferPluginSession * poSession, PktFileSendReq * poPkt );
	virtual EXferError			txNextFileChunk( FileXferPluginSession * poSession, uint32_t remoteErr, bool pluginIsLocked );
	virtual EXferError			rxFileChunk( FileXferPluginSession * poSession, PktFileChunkReq * poPkt );
	//! send final sent file packet and close file
	virtual void				finishFileReceive( FileXferPluginSession * poSession, PktFileSendCompleteReq * poPkt, bool pluginIsLocked  );

	virtual long				sendFileXferError(	VxSktBase *		sktBase,	// socket
													int				iPktType,	// type of packet
													unsigned short	u16Cmd,		// packet command
													long			rc,			// error code
													const char *	pMsg, ...);	// error message


	//=== vars ====//
	PluginSessionMgr&			m_PluginSessionMgr;
};

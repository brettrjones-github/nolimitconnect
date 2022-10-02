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

#include "FileXferBaseMgr.h"
#include <PktLib/VxCommon.h>
#include <ptop_src/ptop_engine_src/P2PEngine/FileShareSettings.h>
#include <GuiInterface/IDefs.h>

#include <CoreLib/VxFileUtil.h>

#include <map>

class VxPktHdr;
class FileRxSession;
class FileTxSession;
class FileXferMgr;
class FileInfo;
class SharedFilesMgr;
class FileLibraryMgr;
class VxSha1Hash;

class PktFileGetCompleteReq;
class PktFileSendReq;
class PktFileChunkReq;
class PktFileSendCompleteReq;
class PktFileListReply;


class FileShareXferMgr : public FileXferBaseMgr
{
public:
	enum EFileXOptions
	{
		eFileXOptionReplaceIfExists		= 0,
		eFileXOptionResumeIfExists		= 1,
		eFileXOptionFailIfExists		= 2
	};

	typedef std::map<VxGUID, FileRxSession *>::iterator FileRxIter;
	typedef std::vector<FileTxSession *>::iterator FileTxIter;

	FileShareXferMgr( P2PEngine& engine, PluginBase& plugin, SharedFilesMgr& sharedFilesMgr, FileLibraryMgr& fileLibraryMgr );
	virtual ~FileShareXferMgr();

	EPluginType					gePluginType( void );

	void						fileAboutToBeDeleted( std::string& fileName );

	virtual void				fromGuiUserLoggedOn( void );
	virtual void				fromGuiGetFileShareSettings( FileShareSettings& fileShareSettings );
	virtual void				fromGuiSetFileShareSettings( FileShareSettings& fileShareSettings );

	virtual void				fromGuiStartPluginSession( VxNetIdent* netIdent = NULL,	int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() );
	virtual void				fromGuiStopPluginSession( VxNetIdent* netIdent = NULL,		int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() );
	virtual bool				fromGuiIsPluginInSession( VxNetIdent* netIdent = NULL,		int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() );

	void						fromGuiCancelDownload( VxGUID& fileInstance );
	void						fromGuiCancelUpload( VxGUID& fileInstance );
	bool						fromGuiMakePluginOffer( VxNetIdent *	netIdent,				// identity of friend
														int				pvUserData,
														const char *	pOfferMsg,				// offer message
														const char *	pFileName		= NULL,
														uint8_t *			fileHashId		= 0,
														VxGUID			lclSessionId	= VxGUID::nullVxGUID() );		

	int							fromGuiPluginControl(	VxNetIdent *	netIdent,
														const char *	pControl, 
														const char *	pAction,
														uint32_t				u32ActionData,
														VxGUID&			fileId,
														uint8_t *			fileHashId );
	// returns -1 if unknown else percent downloaded
	virtual int					fromGuiGetFileDownloadState( uint8_t * fileHashId );

	virtual void				onConnectionLost( VxSktBase* sktBase );

	virtual void				onPktPluginOfferReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktPluginOfferReply		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );

	virtual void				onPktFileGetReq				( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktFileGetReply			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktFileSendReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktFileSendReply			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktFileChunkReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktFileChunkReply			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktFileGetCompleteReq		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktFileGetCompleteReply	( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktFileSendCompleteReq	( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktFileSendCompleteReply	( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktFindFileReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktFindFileReply			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktFileListReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktFileListReply			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktFileInfoReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktFileInfoReply			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktFileInfoErr			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );

	virtual void				replaceConnection( VxNetIdent* netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt );

protected:
	virtual void				onFileReceived( FileRxSession * xferSession, std::string pFileName, EXferError error );
	virtual void				onFileSent( FileTxSession * xferSession, std::string pFileName, EXferError error );

	bool						isFileDownloading( VxSha1Hash& fileHashId );
	bool						isFileInDownloadFolder( const char * pPartialFileName );

	virtual FileRxSession *		findRxSession( VxNetIdent* netIdent );
	virtual FileRxSession *		findRxSession( VxGUID& lclSessionId );
	virtual FileRxSession *		findOrCreateRxSession( VxNetIdent* netIdent, VxSktBase* sktBase );
	virtual FileRxSession *		findOrCreateRxSession( VxGUID& lclSessionId, VxNetIdent* netIdent, VxSktBase* sktBase );
	virtual FileTxSession *		findTxSession( VxNetIdent* netIdent );
	virtual FileTxSession *		findTxSession( VxGUID& lclSessionId );
	virtual FileTxSession *		createTxSession( VxNetIdent* netIdent, VxSktBase* sktBase );
	virtual FileTxSession *		findOrCreateTxSession( VxNetIdent* netIdent, VxSktBase* sktBase );
	virtual FileTxSession *		findOrCreateTxSession( VxGUID& lclSessionId, VxNetIdent* netIdent, VxSktBase* sktBase );

	virtual EXferError			beginFileReceive( FileRxSession * xferSession, PktFileSendReq * poPkt );
	virtual EXferError			beginFileSend( FileTxSession * xferSession );

	virtual void				endFileXferSession( FileRxSession * xferSession );
	virtual void				endFileXferSession( FileTxSession * xferSession );

	virtual EXferError			rxFileChunk( FileRxSession * xferSession, PktFileChunkReq * poPkt );
	virtual EXferError			txNextFileChunk( FileTxSession * xferSession );

	virtual void				finishFileReceive( FileRxSession * xferSession, PktFileSendCompleteReq * poPkt );
	virtual void				finishFileReceive( FileRxSession * xferSession, PktFileGetCompleteReq * poPkt );

	virtual RCODE				sendFileShareError(		VxSktBase *		sktBase,		// socket
														int				iPktType,	// type of packet
														unsigned short	u16Cmd,		// packet command
														long			rc,			// error code
														const char *	pMsg, ...);	// error message

	EXferError					beginFileGet( FileRxSession * xferSession );
	EXferError					canTxFile( VxNetIdent* netIdent, VxSha1Hash& fileHashId );
	bool						isViewFileListMatch( FileTxSession * xferSession, FileInfo& fileInfo );
	void						clearRxSessionsList( void );
	void						clearTxSessionsList( void );
	void						checkQueForMoreFilesToSend( void );

protected:

	//=== vars ===//
	std::map<VxGUID, FileRxSession *>	m_RxSessions;
	std::vector<FileTxSession *>		m_TxSessions;

	SharedFilesMgr&				m_SharedFilesMgr;
	FileLibraryMgr&				m_FileLibraryMgr;

	bool						m_bIsInSession;
	bool						m_bIsInitialized;

	EFileXOptions				m_eFileRxOption;
	FileShareSettings			m_FileShareSettings;
};




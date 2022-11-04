#pragma once
//============================================================================
// Copyright (C) 2022 Brett R. Jones
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


#include "PluginBase.h"
#include "PluginSessionMgr.h"
#include "FileInfoBaseMgr.h"

#include <ptop_src/ptop_engine_src/P2PEngine/FileShareSettings.h>

#include <PktLib/VxCommon.h>

class FileInfoBaseMgr;
class FileRxSession;
class FileTxSession;
class FileShareSettings;
class FileInfoBaseMgr;
class FileInfoXferMgr;
class IToGui;
class P2PEngine;
class PktFileListReply;
class FileInfo;
class VxNetIdent;
class VxFileShredder;

class PluginBaseFiles : public PluginBase
{
public:
	PluginBaseFiles( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent* myIdent, EPluginType pluginType, FileInfoBaseMgr& fileInfoBaseMgr );
	virtual ~PluginBaseFiles() = default;

	FileInfoBaseMgr&			getFileInfoMgr( void )			{ return m_FileInfoMgr; }

	virtual void				onAfterUserLogOnThreaded( void ) override;

    virtual void				fromGuiStartPluginSession( VxNetIdent* netIdent = nullptr, 	int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() ) override;
    virtual void				fromGuiStopPluginSession( VxNetIdent* netIdent = nullptr, 	int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() ) override;
    virtual bool				fromGuiIsPluginInSession( VxNetIdent* netIdent = nullptr, 	int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() ) override;

    void						fromGuiGetFileShareSettings( FileShareSettings& fileShareSettings ) override;
    void						fromGuiSetFileShareSettings( FileShareSettings& fileShareSettings ) override;

    void						fromGuiCancelDownload( VxGUID& fileInstance ) override;
    void						fromGuiCancelUpload( VxGUID& fileInstance ) override;

	bool						fromGuiMakePluginOffer( VxNetIdent* netIdent, OfferBaseInfo& offerInfo ) override;

	EXferError					fromGuiFileXferControl( VxNetIdent* netIdent, EXferAction xferAction, FileInfo& fileInfo ) override;

	virtual bool				fromGuiBrowseFiles( std::string& dir, uint8_t fileFilterMask );
	virtual bool				fromGuiGetSharedFiles( uint8_t fileTypeFilter );
	virtual bool				fromGuiSetFileIsShared( FileInfo& fileInfo, bool isShared );
	virtual bool				fromGuiSetFileIsShared( std::string& fileName, bool isShared );
	virtual bool				fromGuiGetIsFileShared( std::string& fileName );
	virtual bool				fromGuiRemoveSharedFile( std::string& fileName );
	// returns -1 if unknown else percent downloaded
	virtual int					fromGuiGetFileDownloadState( uint8_t * fileHashId );
	virtual bool				fromGuiQueryFileHash( FileInfo& fileInfo );
	virtual void				fromGuiFileHashGenerated( std::string& fileName, int64_t fileLen, VxSha1Hash& fileHash );

    virtual void				onSharedFilesUpdated( uint16_t u16FileTypes ) override;

	virtual bool				isServingFiles( void );

	virtual void				deleteFile( std::string fileName, bool shredFile );

protected:
	virtual void				sendFileSearchResultToGui( VxGUID& searchSessionId, VxNetIdent* netIdent, FileInfo& fileInfo );

	void						onPktPluginOfferReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;

	virtual void				onPktFileGetReq				( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
	virtual void				onPktFileGetReply			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
	virtual void				onPktFileSendReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
	virtual void				onPktFileSendReply			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
	virtual void				onPktFileChunkReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
	virtual void				onPktFileChunkReply			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
	virtual void				onPktFileGetCompleteReq		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
	virtual void				onPktFileGetCompleteReply	( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
	virtual void				onPktFileSendCompleteReq	( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
	virtual void				onPktFileSendCompleteReply	( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;

	virtual void				onPktFindFileReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
	virtual void				onPktFindFileReply			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
	virtual void				onPktFileListReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
	virtual void				onPktFileListReply			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
	virtual void				onPktFileShareErr			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;

	virtual void				onPktFileInfoInfoReq		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
	virtual void				onPktFileInfoInfoReply		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
	virtual void				onPktFileInfoAnnReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
	virtual void				onPktFileInfoAnnReply		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
	virtual void				onPktFileInfoSearchReq		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
	virtual void				onPktFileInfoSearchReply	( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
	virtual void				onPktFileInfoMoreReq		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
	virtual void				onPktFileInfoMoreReply		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;

	virtual bool				updateFromFileInfoSearchBlob( VxGUID& searchSessionId, VxGUID& hostOnlineId, VxSktBase* sktBase, VxNetIdent* netIdent, PktBlobEntry& blobEntry, int fileInfoCount );
	virtual bool				requestMoreFileInfoFromServer( VxGUID& searchSessionId, VxSktBase* sktBase, VxNetIdent* netIdent, VxGUID& nextSearchAssetId, std::string& searchText );

	virtual ECommErr			searchRequest( PktFileInfoSearchReply& pktReply, VxGUID& specificAssetId, std::string& searchStr, uint8_t searchFileTypes, VxSktBase* sktBase, VxNetIdent* netIdent );
	virtual ECommErr			searchMoreRequest( PktFileInfoMoreReply& pktReply, VxGUID& nextFileAssetId, std::string& searchStr, uint8_t searchFileTypes, VxSktBase* sktBase, VxNetIdent* netIdent );

	// should be overwitten by Plugin specific class
	virtual bool                fileInfoSearchResult( VxGUID& searchSessionId, VxSktBase* sktBase, VxNetIdent* netIdent, FileInfo& fileInfo ) { return false; };
	virtual void                fileInfoSearchCompleted( VxGUID& searchSessionId, VxSktBase* sktBase, VxNetIdent* netIdent, ECommErr commErr ) {};

    bool						isFileShared( std::string& fileName );
    virtual void				replaceConnection( VxNetIdent* netIdent, VxSktBase* poOldSkt, VxSktBase* poNewSkt ) override;
    virtual void				onContactWentOffline( VxNetIdent* netIdent, VxSktBase* sktBase ) override;
    virtual void				onConnectionLost( VxSktBase* sktBase ) override;	

	//=== vars ===//
	VxFileShredder&				m_FileShredder;
	PluginSessionMgr			m_PluginSessionMgr;
	FileInfoBaseMgr&			m_FileInfoMgr;
	FileInfoXferMgr&			m_FileInfoXferMgr;
};



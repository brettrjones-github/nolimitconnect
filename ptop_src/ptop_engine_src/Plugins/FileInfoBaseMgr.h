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

#include "FileInfoDb.h"
#include "FileInfoXferMgr.h"

#include <PktLib/VxCommon.h>

#include <CoreLib/VxThread.h>
#include <CoreLib/VxMutex.h>
#include <CoreLib/Sha1GeneratorCallback.h>

class FileInfo;
class IToGui;
class PluginBase;
class P2PEngine;
class PktFileListReply;
class PktFileInfoSearchReply;
class PktFileInfoMoreReply;
class SearchParams;
class SharedFilesMgr;
class VxSha1Hash;
class VxFileShredder;

class FileInfoBaseMgr : public Sha1GeneratorCallback
{
public:
	FileInfoBaseMgr() = delete;
	FileInfoBaseMgr( const FileInfoBaseMgr& rhs ) = delete;
	FileInfoBaseMgr( P2PEngine& engine, PluginBase& plugin, FileInfoDb& fileInfoDb );
	virtual ~FileInfoBaseMgr();

	FileInfoBaseMgr& operator=( const FileInfoBaseMgr& rhs ) = delete;

	bool						getIsInitialized( void )			{ return m_FilesInitialized; }

	P2PEngine&					getEngine( void )					{ return m_PrivateEngine; }

	FileInfoXferMgr&			getFileInfoXferMgr( void )			{ return m_FileInfoXferMgr; }

	virtual void				fileInfoMgrShutdown( void );

	void						lockFileList( void )				{ m_FilesListMutex.lock(); }
	void						unlockFileList( void )				{ m_FilesListMutex.unlock(); }
	std::map<VxGUID, FileInfo>& getFileLibraryList( void )			{ return m_FileInfoList; }

	void						setRootFolder( std::string& rootFileFolder ) { m_RootFileFolder = rootFileFolder; }
	std::string					getRootFolder( void )				{ return m_RootFileFolder; }

	virtual std::string			getIncompleteFileXferDirectory( VxGUID& onlineId );

	bool						cancelAndDelete( VxGUID& assetId );

	// returns -1 if unknown else percent downloaded
	virtual int					fromGuiGetFileDownloadState(	uint8_t *		fileHashId );
	
	virtual bool				fromGuiBrowseFiles( const char* dir, uint8_t fileFilterMask ) { return false; }

	virtual bool				fromGuiGetSharedFiles( uint8_t fileTypeFilter );
	virtual bool				fromGuiSetFileIsShared( FileInfo& fileInfo, bool isShared );
	virtual bool				fromGuiSetFileIsShared( std::string& fileName, bool isShared );
	virtual bool				fromGuiGetIsFileShared( std::string& fileName );
	virtual bool				fromGuiRemoveSharedFile( std::string& fileName );

	virtual bool				fromGuiAddSharedFile( FileInfo& fileInfo, bool isShared );

	virtual void				updateFileTypes( void );

	virtual bool				isFileShared( std::string& fileName, bool listIsLocked = false );
	virtual bool				isFileShared( VxGUID& assetId, bool listIsLocked = false );
	virtual bool				isFileShared( VxSha1Hash& fileHashId, bool listIsLocked = false );
	virtual bool				isFileShared( VxGUID& assetId, VxSha1Hash& fileHashId, bool listIsLocked = false );

	virtual bool				addFileToDbAndList( FileInfo& fileInfoIn );
	virtual bool				addFileToDbAndList( VxGUID& onlineId, std::string& fileName, VxGUID& assetId );
	virtual bool				addFileToDbAndList( VxGUID&			onlineId,
													VxGUID&			assetId,
													std::string		fileName,
													uint64_t		fileLen,
													uint8_t			fileType,
													VxSha1Hash&		fileHashId );

	virtual bool				removeFromDbAndList( std::string& fileName, bool listIsLocked = false );

	bool						isAllowedFileOrDir( std::string strFileName );

	void						clearLibraryFileList( void );

	bool						getFileFullName( VxGUID& assetId, VxSha1Hash& fileHashId, std::string& retFileFullName );
	bool						getFileHashId( std::string& fileFullName, VxSha1Hash& retFileHashId );

	virtual bool				onFileDownloadComplete( VxNetIdent* netIdent, VxSktBase* sktBase, VxGUID& lclSessionId, std::string& fileName, VxGUID& assetId, VxSha1Hash& sha11Hash);

	bool						loadAboutMePageStaticAssets( void );
	bool						loadStoryboardPageFileAssets( void );

	// virtual void				fromGuiUserLoggedOn( void ) override;
	virtual void				onAfterUserLogOnThreaded( void );

	ECommErr					searchRequest( PktFileInfoSearchReply& pktReply, VxGUID& specificAssetId, std::string& searchStr, uint8_t searchFileTypes, VxSktBase* sktBase, VxNetIdent* netIdent );
	ECommErr					searchMoreRequest( PktFileInfoMoreReply& pktReply, VxGUID& nextFileAssetId, std::string& searchStr, uint8_t searchFileTypes, VxSktBase* sktBase, VxNetIdent* netIdent );

	bool						startDownload( FileInfo& fileInfo, VxGUID& searchSessionId, VxSktBase* sktBase, VxNetIdent* netIdent );

	virtual void				toGuiRxedPluginOffer( VxNetIdent*		netIdent,				// identity of friend
														EPluginType		ePluginType,			// plugin type
														const char*		pOfferMsg,				// offer message
														int				pvUserData,				// plugin defined data
														const char*		pFileName = NULL,		// filename if any
														uint8_t*		fileHashData = 0,
														VxGUID&			lclSessionId = VxGUID::nullVxGUID(),
														VxGUID&			rmtSessionId = VxGUID::nullVxGUID() );
	virtual void				toGuiRxedOfferReply( VxNetIdent*	netIdent,
													EPluginType		ePluginType,
													int				pvUserData,
													EOfferResponse	eOfferResponse,
													const char*		pFileName = 0,
													uint8_t*		fileHashData = 0,
													VxGUID&			lclSessionId = VxGUID::nullVxGUID(),
													VxGUID&			rmtSessionId = VxGUID::nullVxGUID() );

	virtual void				toGuiFileUploadStart( VxGUID& onlineId, VxGUID& lclSessionId, FileInfo& fileInfo );
	virtual void				toGuiFileDownloadStart( VxGUID& onlineId, VxGUID& lclSessionId, FileInfo& fileInfo );

	void						updateToGuiFileXferState( VxGUID& lclSessionId, EXferDirection xferDir, EXferState xferState, EXferError xferErr, int param = 0 );
	virtual void				toGuiFileDownloadComplete( VxGUID& lclSessionId, std::string fileName, EXferError xferError );
	virtual void				toGuiFileUploadComplete( VxGUID& lclSessionId, std::string fileName, EXferError xferError );

	virtual void				sendFileSearchResultToGui( VxGUID& searchSessionId, VxNetIdent* netIdent, FileInfo& fileInfo );

protected:
	void						checkForInitializeCompleted( void );
	bool						findFileAsset( VxGUID& fileAssetId, FileInfo& fileInfo ); // assumes file list is already locked

	void						addFileToGenHashQue( VxGUID& assetId, std::string fileName );
	void						removeFileFromGenHashQue( VxGUID& assetId, std::string fileName );
	virtual void				callbackSha1GenerateResult( ESha1GenResult sha1GenResult, VxGUID& assetId, Sha1Info& sha1Info ) override;

	bool						isFileShareServer( void );
	bool						isLibraryServer( void );

	//=== vars ===//
	PluginBase&					m_Plugin;
	VxFileShredder&				m_FileShredder;

	FileInfoDb&					m_FileInfoDb;
	FileInfoXferMgr				m_FileInfoXferMgr;

	int64_t						m_LastUpdateTime{ 0 };
	int64_t						m_s64TotalByteCnt{ 0 };
	uint16_t					m_u16FileTypes{ 0 };

	VxMutex						m_FilesListMutex;
	std::map<VxGUID,FileInfo>	m_FileInfoList; // map of assetId, FileInfo
	std::vector<FileInfo>		m_FileInfoNeedHashAndSaveList;


	std::string					m_RootFileFolder{ "" };
	bool						m_FilesInitialized{ false };

private:
	P2PEngine&					m_PrivateEngine; // to avoid abiguous access
};


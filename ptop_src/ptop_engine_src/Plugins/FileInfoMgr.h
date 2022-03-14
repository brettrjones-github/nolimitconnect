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
// http://www.nolimitconnect.com
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
class SearchParams;
class SharedFilesMgr;
class VxSha1Hash;
class VxFileShredder;

class FileInfoMgr : public Sha1GeneratorCallback
{
public:
	FileInfoMgr( P2PEngine& engine, PluginBase& plugin, std::string fileInfoDbName );
	virtual ~FileInfoMgr();

	bool						getIsInitialized( void )			{ return m_FilesInitialized; }

	void						fileLibraryShutdown( void );

	void						lockFileList( void )				{ m_FilesListMutex.lock(); }
	void						unlockFileList( void )				{ m_FilesListMutex.unlock(); }
	std::map<VxGUID, FileInfo*>& getFileLibraryList( void )			{ return m_FileInfoList; }

	void						lockFileListPackets( void )			{ m_PacketsMutex.lock(); }
	void						unlockFileListPackets( void )		{ m_PacketsMutex.unlock(); }
	std::vector<PktFileListReply*>& getFileListPackets( void )		{ return m_FileListPackets; }

	void						setRootFolder( std::string& rootFileFolder ) { m_RootFileFolder = rootFileFolder; }
	std::string					getRootFolder( void )				{ return m_RootFileFolder; }


	bool						isFileInLibrary( std::string& fileName );
	bool						isFileInLibrary( VxSha1Hash& fileHashId );
	bool						isFileInLibrary( VxGUID& onlineId, VxGUID& assetId );
	bool						isFileInLibrary( VxGUID& assetId );

	bool						addFileToLibrary( VxGUID& onlineId, std::string& fileName, VxGUID& assetId );
	bool						addFileToLibrary(	VxGUID&			onlineId,
													VxGUID&			assetId,
													std::string		fileName,
													uint64_t		fileLen,
													uint8_t			fileType,
													VxSha1Hash&		fileHashId );
	bool						addFileToLibrary( FileInfo* fileInfo );

	// returns -1 if unknown else percent downloaded
	virtual int					fromGuiGetFileDownloadState(	uint8_t *		fileHashId );
	
	virtual bool				fromGuiBrowseFiles( const char* dir, bool lookupShareStatus, uint8_t fileFilterMask ) { return false; }
	virtual bool				fromGuiGetSharedFiles( uint8_t fileTypeFilter )				{ return false; }
	virtual bool				fromGuiSetFileIsShared( std::string& fileName, bool isShared, uint8_t* fileHashId = 0 ) { return false; }
	virtual bool				fromGuiGetIsFileShared( const char* fileName )				{ return false; }

	virtual void				fromGuiGetFileLibraryList( uint8_t fileTypeFilter )			{};
	virtual bool				fromGuiGetIsFileInLibrary( const char* fileName )			{ return false; }
	virtual bool				fromGuiAddFileToLibrary( const char* fileName, bool addFile, uint8_t* fileHashId );

	void						updateFileTypes( void );

	void						onFileLibraryUpdated( void );

	void						removeFromLibrary( std::string& fileName );


	bool						isAllowedFileOrDir( std::string strFileName );

	void						addFileToGenHashQue( VxGUID& assetId, std::string fileName );
	virtual void				callbackSha1GenerateResult( ESha1GenResult sha1GenResult, VxGUID& assetId, Sha1Info& sha1Info ) override;

	void						clearLibraryFileList( void );

	bool						getFileFullName( VxSha1Hash& fileHashId, std::string& retFileFullName );
	bool						getFileHashId( std::string& fileFullName, VxSha1Hash& retFileHashId );

	virtual bool				onFileDownloadComplete( std::string& fileName, bool addFile, uint8_t* fileHashId );

	virtual bool				isFileShared( VxSha1Hash& fileHashId );
	virtual bool				isFileShared( std::string& fileName );

	void						getAboutMePageStaticAssets( std::vector<std::pair<VxGUID, std::string>>& assetList );

	void						getStoryboardStaticAssets( std::vector<std::pair<VxGUID, std::string>>& assetList );

	// virtual void				fromGuiUserLoggedOn( void ) override;
	virtual void				onAfterUserLogOnThreaded( void );

	ECommErr					searchRequest( SearchParams& searchParams, PktFileInfoSearchReply& searchReply, std::string& searchStr, VxSktBase* sktBase, VxNetIdent* netIdent );

protected:
	void						checkForInitializeCompleted( void );

	//=== vars ===//
    P2PEngine&					m_Engine;
	PluginBase&					m_Plugin;
	VxFileShredder&				m_FileShredder;

	int64_t						m_LastUpdateTime{ 0 };
	int64_t						m_s64TotalByteCnt{ 0 };
	uint16_t					m_u16FileTypes{ 0 };

	VxMutex						m_FilesListMutex;
	std::map<VxGUID,FileInfo*>	m_FileInfoList;
	std::vector<FileInfo*>		m_FileInfoNeedHashAndSaveList;

	std::vector<PktFileListReply*>m_FileListPackets;
	VxMutex						m_PacketsMutex;

	FileInfoDb					m_FileInfoDb;
	FileInfoXferMgr				m_FileInfoXferMgr;

	std::string					m_RootFileFolder{ "" };
	bool						m_FilesInitialized{ false };
};


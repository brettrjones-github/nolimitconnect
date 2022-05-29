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

#include "PluginBaseFilesClient.h"

class PluginStoryboardClient : public PluginBaseFilesClient
{
public:
	PluginStoryboardClient( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, EPluginType pluginType );
	virtual ~PluginStoryboardClient() = default;

	bool						getIsInitialized( void ) { return m_WebPageClientReady; }

	virtual bool				fromGuiDownloadWebPage( EWebPageType webPageType, VxGUID& onlineId ) override;
	virtual bool				fromGuiCancelWebPage( EWebPageType webPageType, VxGUID& onlineId ) override;

	virtual std::string			getIncompleteFileXferDirectory( VxGUID& onlineId ) override;

	void						lockSearchFileList( void ) { m_SearchFilesListMutex.lock(); }
	void						unlockSearchFileList( void ) { m_SearchFilesListMutex.unlock(); }

	void						lockInProgressFileList( void ) { m_InProgressFilesListMutex.lock(); }
	void						unlockInProgressFileList( void ) { m_InProgressFilesListMutex.unlock(); }

	void						lockCompletedFileList( void ) { m_CompletedFilesListMutex.lock(); }
	void						unlockCompletedFileList( void ) { m_CompletedFilesListMutex.unlock(); }

protected:
	virtual bool                fileInfoSearchResult( VxGUID& searchSessionId, VxSktBase* sktBase, VxNetIdent* netIdent, FileInfo& fileInfo ) override;
	virtual void                fileInfoSearchCompleted( VxGUID& searchSessionId, VxSktBase* sktBase, VxNetIdent* netIdent, ECommErr commErr ) override;

	virtual void				onAfterUserLogOnThreaded( void ) override;
	virtual void				onLoadedFilesReady( int64_t lastFileUpdateTime, int64_t totalBytes, uint16_t fileTypes ) override;
	virtual void				onFilesChanged( int64_t lastFileUpdateTime, int64_t totalBytes, uint16_t fileTypes ) override;

	virtual bool				onFileDownloadComplete( VxNetIdent* netIdent, VxSktBase* sktBase, VxGUID& lclSessionId, std::string& fileName, VxGUID& assetId, VxSha1Hash& sha11Hash ) override;

	void						setIsWebPageClientReady( bool isReady );
	bool						getIsWebPageClientReady( void ) { return m_WebPageClientReady; }

	void						checkIsWebPageClientReady( void );
	void						onWebPageClientReady( bool isReady );
	void						cancelDownload( void );
	bool						startDownload( VxGUID& searchSessionId, VxSktBase* sktBase, VxNetIdent* netIdent );

	std::string					getWebIndexFileName( void ) { return "story_board.htm"; }

	std::string					m_RootFileFolder{ "" };
	std::string					m_DownloadFileFolder{ "" };
	std::string					m_WebPageIndexFile{ "" };

	bool						m_WebPageClientReady{ false };
	VxGUID						m_HisOnlineId;

	VxMutex						m_SearchFilesListMutex;
	std::vector<FileInfo>		m_SearchFileInfoList;

	VxMutex						m_InProgressFilesListMutex;
	std::vector<FileInfo>		m_InProgressFileInfoList; // map of assetId, FileInfo

	VxMutex						m_CompletedFilesListMutex;
	std::vector<FileInfo>		m_CompletedFileInfoList;
};




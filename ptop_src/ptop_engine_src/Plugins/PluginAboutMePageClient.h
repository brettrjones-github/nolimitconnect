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
// http://www.nolimitconnect.com
//============================================================================

#include "PluginBaseFilesClient.h"

class PluginAboutMePageClient : public PluginBaseFilesClient
{
public:
    PluginAboutMePageClient( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, EPluginType pluginType );
	virtual ~PluginAboutMePageClient() = default;

	bool						getIsInitialized( void )			{ return m_AboutMePageReady; }

	virtual bool				fromGuiDownloadWebPage( EWebPageType webPageType, VxGUID& onlineId ) override;
	virtual bool				fromGuiCancelWebPage( EWebPageType webPageType, VxGUID& onlineId ) override;

	void						lockSearchFileList( void )		{ m_SearchFilesListMutex.lock(); }
	void						unlockSearchFileList( void )	{ m_SearchFilesListMutex.unlock(); }
	std::vector<FileInfo>&		getSearchFileList( void )		{ return m_SearchFileInfoList; }

protected:
	virtual void                fileInfoSearchResult( VxGUID& searchSessionId, VxSktBase* sktBase, VxNetIdent* netIdent, FileInfo& fileInfo ) override;
	virtual void                fileInfoSearchCompleted( VxGUID& searchSessionId, VxSktBase* sktBase, VxNetIdent* netIdent, ECommErr commErr ) override;

	virtual void				onAfterUserLogOnThreaded( void ) override;
	virtual void				onLoadedFilesReady( int64_t lastFileUpdateTime, int64_t totalBytes, uint16_t fileTypes ) override;
	virtual void				onFilesChanged( int64_t lastFileUpdateTime, int64_t totalBytes, uint16_t fileTypes ) override;

	void						setIsAboutMePageReady( bool isReady );
	bool						getIsAboutMePageReady( void ) { return m_AboutMePageReady; }

	void						checkIsAboutMePageReady( void );
	void						onAboutMePageReady( bool isReady );

	std::string					m_RootFileFolder{ "" };
	std::string					m_DownloadFileFolder{ "" };
	std::vector<std::pair<VxGUID, std::string>> m_AssetList;
	bool						m_AboutMePageReady{ false };
	VxGUID						m_HisOnlineId;

	VxMutex						m_SearchFilesListMutex;
	std::vector<FileInfo>		m_SearchFileInfoList; // map of assetId, FileInfo
};




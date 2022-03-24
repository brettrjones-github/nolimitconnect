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

#include "PluginBaseFiles.h"

class PluginBaseFilesClient : public PluginBaseFiles
{
public:
	enum EFileClientState
	{
		eFileClientStateInitializing,
		eFileClientStateIdle,
		eFileClientStateRetrieveInfo,
		eFileClientStateDownloading,
		eFileClientStateDownloadComplete,
		eFileClientStateCommError,

		eMaxFileClientState
	};

	PluginBaseFilesClient( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent* myIdent, EPluginType pluginType, std::string fileInfoDbName );
	virtual ~PluginBaseFilesClient() = default;

	bool						connectForWebPageDownload( VxGUID& onlineId );
	bool						onConnectForWebPageDownload( VxSktBase* sktBase, VxNetIdent* netIdent );


	virtual void				toGuiStartDownload( VxNetIdent*		netIdent,
													EPluginType		ePluginType,
													VxGUID&			lclSessionId,
													uint8_t			u8FileType,
													uint64_t		u64FileLen,
													const char*		pFileName,
													VxGUID			assetId,
													uint8_t*		fileHashData ) override;

	virtual void				toGuiFileXferState( VxGUID& localSessionId, EXferState xferState, EXferError xferErr, int param = 0 ) override;
	virtual void				toGuiFileDownloadComplete( VxGUID& lclSessionId, const char* newFileName, EXferError xferError ) override;

protected:

	VxNetIdent*					m_HisIdent{ nullptr };
	VxGUID						m_HisOnlineId;
	VxGUID						m_SearchSessionId;
	EFileClientState			m_FileClientState{ eFileClientStateInitializing };
};



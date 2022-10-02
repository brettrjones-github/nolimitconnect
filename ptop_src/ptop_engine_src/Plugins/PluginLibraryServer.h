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

#include "PluginBaseFilesServer.h"

class PluginLibraryServer : public PluginBaseFilesServer
{
public:
	PluginLibraryServer( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent* netIdent, EPluginType pluginType );
	virtual ~PluginLibraryServer() = default;

	virtual void				onNetworkConnectionReady( bool requiresRelay ) override;

	virtual void				updateSharedFilesInfo( void );

	virtual bool				fromGuiAddFileToLibrary( const char* fileName, bool addFile, uint8_t* fileHashId = 0 );
	virtual void				fromGuiGetFileLibraryList( uint8_t fileTypeFilter );
	virtual bool				fromGuiGetIsFileInLibrary( const char* fileName );

	bool						isFileInLibrary( std::string& fileName );
	bool						isFileInLibrary( VxSha1Hash& fileHashId );
	bool						isFileInLibrary( VxGUID& assetId );

	bool						addFileToLibrary( VxGUID& onlineId, std::string& fileName, VxGUID& assetId );
	bool						addFileToLibrary( VxGUID& onlineId,
													VxGUID& assetId,
													std::string		fileName,
													uint64_t		fileLen,
													uint8_t			fileType,
													VxSha1Hash& fileHashId );
	bool						addFileToLibrary( FileInfo& fileInfo );

	void						deleteFile( const char* fileName, bool shredFile ) override;

protected:
	virtual void				onFilesChanged( int64_t lastFileUpdateTime, int64_t totalBytes, uint16_t fileTypes ) override;
};



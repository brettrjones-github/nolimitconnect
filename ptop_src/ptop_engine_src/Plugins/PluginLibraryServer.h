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

	virtual bool				fromGuiSetFileIsInLibrary( FileInfo& fileInfo, bool inLibrary );
	virtual bool				fromGuiSetFileIsInLibrary( std::string& fileName, bool inLibrary );
	virtual void				fromGuiGetFileLibraryList( uint8_t fileTypeFilter );
	virtual bool				fromGuiGetIsFileInLibrary( std::string& fileName );
	virtual bool				fromGuiRemoveFromLibrary( std::string& fileName );

	bool						isFileInLibrary( std::string& fileName );
	bool						isFileInLibrary( VxSha1Hash& fileHashId );
	bool						isFileInLibrary( VxGUID& assetId );

	bool						addFileToLibrary( FileInfo& fileInfo );

	void						deleteFile( std::string fileName, bool shredFile ) override;

protected:
	virtual void				onFilesChanged( int64_t lastFileUpdateTime, int64_t totalBytes, uint16_t fileTypes ) override;
};



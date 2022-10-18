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

#include "PluginBaseFilesServer.h"

#include <ptop_src/ptop_engine_src/Plugins/FileInfoLibraryMgr.h>

class PluginFileShareServer : public PluginBaseFilesServer
{
public:
	PluginFileShareServer( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent* netIdent, EPluginType pluginType );
	virtual ~PluginFileShareServer() = default;

	virtual void				onNetworkConnectionReady( bool requiresRelay ) override;

	virtual void				updateSharedFilesInfo( void );

	void						deleteFile( std::string fileName, bool shredFile ) override;

protected:
	virtual void				onFilesChanged( int64_t lastFileUpdateTime, int64_t totalBytes, uint16_t fileTypes ) override;
};



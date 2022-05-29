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

class PluginStoryboardServer : public PluginBaseFilesServer
{
public:
	PluginStoryboardServer( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, EPluginType pluginType );
	virtual ~PluginStoryboardServer() = default;

protected:
	virtual void				onAfterUserLogOnThreaded( void ) override;
	virtual void				onLoadedFilesReady( int64_t lastFileUpdateTime, int64_t totalBytes, uint16_t fileTypes ) override;
	virtual void				onFilesChanged( int64_t lastFileUpdateTime, int64_t totalBytes, uint16_t fileTypes ) override;

	void						setIsWebPageServerReady( bool isReady );
	bool						getIsWebPageServerReady( void ) { return m_WebPageServerReady; }

	void						checkIsWebPageServerReady( void );
	void						onWebPageServerReady( bool isReady );

	std::string					m_RootFileFolder{ "" };
	bool						m_WebPageServerReady{ false };
};




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

#include "PluginBaseFilesServer.h"

class PluginAboutMePageServer : public PluginBaseFilesServer
{
public:
	PluginAboutMePageServer( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, EPluginType pluginType );
	virtual ~PluginAboutMePageServer() = default;

protected:

	virtual void				onAfterUserLogOnThreaded( void ) override;
	virtual void				onLoadedFilesReady( int64_t lastFileUpdateTime, int64_t totalBytes, uint16_t fileTypes ) override;
	virtual void				onFilesChanged( int64_t lastFileUpdateTime, int64_t totalBytes, uint16_t fileTypes ) override;

	void						setIsAboutMePageReady( bool isReady );
	bool						getIsAboutMePageReady( void ) { return m_AboutMePageReady; }

	void						checkIsAboutMePageReady( void );
	void						onAboutMePageReady( bool isReady );

	std::string					m_RootFileFolder{""};
	bool						m_AboutMePageReady{ false };
};




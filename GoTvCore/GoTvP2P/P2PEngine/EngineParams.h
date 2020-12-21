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

#include <NetLib/NetSettings.h>
#include <PktLib/VxCommon.h>
#include <CoreLib/VxSettings.h>

class EngineParams : public VxSettings
{
public:

	EngineParams();
	virtual ~EngineParams();

	RCODE						engineParamsStartup( std::string& strDbFileName );
	void						engineParamsShutdown( void );

	void						setLastHostWebsiteUrl( std::string& strWebsiteUrl );
	void						getLastHostWebsiteUrl( std::string& strWebsiteUrl );
	void						setLastHostWebsiteResolvedIp( std::string& strWebsiteIp );
	void						getLastHostWebsiteResolvedIp( std::string& strWebsiteIp );

	void						setLastConnectTestUrl( std::string& strWebsiteUrl );
	void						getLastConnectTestUrl( std::string& strWebsiteUrl );
	void						setLastConnectTestResolvedIp( std::string& strWebsiteIp );
	void						getLastConnectTestResolvedIp( std::string& strWebsiteIp );

private:
	bool						m_Initialized;
};

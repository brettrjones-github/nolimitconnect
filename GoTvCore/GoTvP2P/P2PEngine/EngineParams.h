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
// brett.jones@engineer.com
// http://www.gotvptop.net
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

	void						setLastAnchorWebsiteUrl( std::string& strWebsiteUrl );
	void						getLastAnchorWebsiteUrl( std::string& strWebsiteUrl );
	void						setLastAnchorWebsiteResolvedIp( std::string& strWebsiteIp );
	void						getLastAnchorWebsiteResolvedIp( std::string& strWebsiteIp );

	void						setLastNetServiceWebsiteUrl( std::string& strWebsiteUrl );
	void						getLastNetServiceWebsiteUrl( std::string& strWebsiteUrl );
	void						setLastNetServiceWebsiteResolvedIp( std::string& strWebsiteIp );
	void						getLastNetServiceWebsiteResolvedIp( std::string& strWebsiteIp );

private:
	bool						m_Initialized;
};

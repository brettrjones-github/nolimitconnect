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

#include "Sha1GeneratorCallback.h"
#include "Sha1ClientInfo.h"
#include "VxThread.h"
#include "VxMutex.h"

class Sha1GeneratorMgr
{
public:
	Sha1GeneratorMgr();
	virtual ~Sha1GeneratorMgr() = default;

	void						generateSha1( VxGUID& fileId, std::string& fileName, Sha1GeneratorCallback* client );

	void						threadGenerateSha1( VxThread* vxThread );

	void						announceResult( ESha1GenResult sha1GenResult, Sha1ClientInfo& sha1Info );

protected:
	void						startThreadIfNotStarted( void );

	//=== vars ===//
	VxMutex						m_Sha1ListMutex;
	VxThread					m_Sha1Thread;
	std::vector<Sha1ClientInfo>	m_Sha1List;
};

Sha1GeneratorMgr& GetSha1GeneratorMgr( void );

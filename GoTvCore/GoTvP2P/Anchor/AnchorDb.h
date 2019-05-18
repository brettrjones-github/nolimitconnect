#pragma once
//============================================================================
// Copyright (C) 2014 Brett R. Jones
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
// http://www.gotvptop.net
//============================================================================

#include "AnchorDefs.h"
#include "PhoneShakeEntry.h"

#include <GoTvCore/GoTvP2P/NetServices/NetServiceDefs.h>

#include <CoreLib/DbBase.h>

class AnchorList;

class AnchorDb : public DbBase
{
public:
	AnchorDb();
	virtual ~AnchorDb();

	virtual RCODE				handleAnnounce(		AnchorList&			anchorListIn, 
													AnchorList&			anchorListOut,
													VxGUID				ignoreMe = VxGUID::nullVxGUID() );
	void						addIgnoreId( VxGUID& onlineIdToIgnore );

protected:
	virtual RCODE				onCreateTables( int iDbVersion );
	virtual RCODE				onDeleteTables( int iOldVersion );

	bool						validateId( std::string& onlineId );
	bool						validatePort( std::string& port );
	bool						validateIPv4( std::string& ipv4 );
	bool						validateIPv6( std::string& ipv6 );
	RCODE						handlePhoneShake(	AnchorListEntry&	callerEntry, 
													AnchorList&			anchorListOut,
													int64_t					s64PostTimeMs );

	int							m_iInseredEntryCount;
	std::vector<PhoneShakeEntry>	m_PhoneShakeList;
	VxMutex						m_PhoneShakeMutex;
	std::vector<VxGUID>			m_IgnoreIdList;
};



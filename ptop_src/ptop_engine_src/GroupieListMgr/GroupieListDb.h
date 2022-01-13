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
// http://www.nolimitconnect.com
//============================================================================

#include "GroupieInfo.h"

#include <CoreLib/DbBase.h>

class GroupieListDb : public DbBase
{
public:
	GroupieListDb();
	virtual ~GroupieListDb();

	RCODE						groupieListDbStartup( int dbVersion, const char* dbFileName );
	RCODE						groupieListDbShutdown( void );

	void						getAllGroupies( std::vector<GroupieInfo>& groupieList );
	bool						saveGroupie( GroupieInfo& hostedInfo );
	void						removeGroupieInfo( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType );

	bool                        updateIsFavorite( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType, bool isFavorite );
	bool                        updateLastConnected( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType, int64_t lastConnectedTime );
	bool                        updateLastJoined( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType, int64_t lastJoinedTime );
	bool						updateGroupieUrl( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType, std::string& groupieUrl );
	bool                        updateGroupieTitleAndDescription( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType, std::string& title, std::string& description, int64_t lastDescUpdateTime );


protected:
	virtual RCODE				onCreateTables( int iDbVersion );
	virtual RCODE				onDeleteTables( int iOldVersion );

	bool						doesGroupieInfoExist( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType, std::string& retGroupieOnlineHexStr, std::string& retHostOnlineHexStr );
};



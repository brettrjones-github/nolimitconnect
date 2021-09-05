#pragma once
//============================================================================
// Copyright (C) 2021 Brett R. Jones 
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

#include <GuiInterface/IDefs.h>

#include <CoreLib/VxGUID.h>

class VxNetIdent;

class BaseInfo
{
public:
    BaseInfo();
    BaseInfo( VxGUID& creatorId, int64_t modifiedTime = 0 );
    BaseInfo( VxGUID& creatorId, VxGUID& assetId, int64_t modifiedTime = 0 );
	BaseInfo( const BaseInfo& rhs );

    virtual ~BaseInfo() = default;

	BaseInfo&				    operator=( const BaseInfo& rhs ); 

    virtual void				setOnlineId( VxGUID onlineId )                  { m_OnlineId = onlineId; }
    virtual void				setOnlineId( const char * onlineId )            { m_OnlineId.fromVxGUIDHexString( onlineId ); }
    virtual VxGUID&				getOnlineId( void )                             { return m_OnlineId; }

    virtual void				setThumbId( VxGUID& thumbId )                   { m_ThumbId = thumbId; }
    virtual void				setThumbId( const char * thumbId )              { m_ThumbId.fromVxGUIDHexString( thumbId ); }
    virtual VxGUID&				getThumbId( void )                              { return m_ThumbId; }

    virtual void				setInfoModifiedTime( int64_t timestamp )        { m_InfoModifiedTime = timestamp; }
    virtual int64_t			    getInfoModifiedTime( void )                     { return m_InfoModifiedTime; }

    virtual void                fillBaseInfo( VxNetIdent* netIdent, EHostType hostType );

    virtual void                assureHasCreatorId( void );
public:
	//=== vars ===//
    VxGUID						m_OnlineId; 
    VxGUID						m_ThumbId; 
    int64_t						m_InfoModifiedTime{ 0 };

};

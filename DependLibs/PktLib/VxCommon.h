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

#include "VxNetIdentBase.h"
#include <GuiInterface/IDefs.h>

class VxSktBase;

//! \public enumerated application State
enum EAppState
{
	eAppStateInvalid		= 0,	// unknown or disabled
	eAppStateStartup		= 1,	// app has started
	eAppStateShutdown		= 2,	// app shutdown
	eAppStateSleep			= 3,	// app sleep
	eAppStateWake			= 4,	// app wake
	eAppStatePause			= 5,	// pause app
	eAppStateResume			= 6,	// resume
	eAppStatePermissionErr	= 7,	// disabled by permission
	eAppStateInactive		= 8,	// inactive due to no files or error etc

	eMaxAppState // must be last.. maximum application states
};

#define USE_PLUGIN_WEB_SERVER		1
#define USE_PLUGIN_RELAY			1
#define USE_PLUGIN_CAM_SERVER		1
#define USE_PLUGIN_INST_MSG			1
#define USE_PLUGIN_VOICE_PHONE		1
#define USE_PLUGIN_VIDEO_PHONE		1
#define USE_PLUGIN_TRUTH_OR_DARE	1
#define USE_PLUGIN_FILE_OFFER		1
#define USE_PLUGIN_FILE_SERVER		1
#define USE_PLUGIN_STORY_BOARD		1

//! describe plugin local name
const char * DescribePluginLclName( EPluginType ePluginType );

// permission bits
// 0000		disabled or ignore
// 0001		anonymous or anyone
// 0010		guest 
// 0011		friends
// 0100		admin

#pragma pack(1)
//! 24 bytes in size
//! 48 max possible plugins
#define PERMISSION_ARRAY_SIZE 24
/// class to handle plugin permissions array
class PluginPermission
{
public:
	PluginPermission();
    PluginPermission( const PluginPermission &rhs ) = default;
    bool                        addToBlob( PktBlobEntry& blob );
    bool                        extractFromBlob( PktBlobEntry& blob );
    PluginPermission&           operator =( const PluginPermission &rhs );

	bool						isPluginEnabled( EPluginType ePlugin );
	//! get type of permission user has set for given plugin
	EFriendState				getPluginPermission( EPluginType ePluginType ); 
	//! set type of permission user has set for given plugin
	void						setPluginPermission( EPluginType ePluginType, EFriendState eFriendState ); 
	//! reset permissions to default values
	void						setPluginPermissionsToDefaultValues( void );
	uint8_t *					getPluginPermissions( void )				{ return m_au8Permissions; }

private:
	//=== vars ===//
	uint8_t						m_au8Permissions[ PERMISSION_ARRAY_SIZE ];
};

//  size
// +   24 bytes PluginPermission
// +    2 bytes m_u16AppVersion;	
// +    2 bytes m_u16PingTimeMs;	
// +    2 bytes m_NetIdentRes1;	
// +    1 bytes m_NetIdentRes2;
// +    1 bytes m_NetIdentRes3;
// +    8 bytes m_NetIdentRes4;	
// +    8 bytes m_LastSessionTime;
// =   48 bytes
// +  384 bytes VxNetIdentBase   
// =  432 bytes total

/// network indentiy of contact
class VxNetIdent : public VxNetIdentBase, public PluginPermission
{
public:
	VxNetIdent();
	VxNetIdent(const VxNetIdent &rhs );
    bool                        addToBlob( PktBlobEntry& blob );
    bool                        extractFromBlob( PktBlobEntry& blob );
    VxNetIdent&                 operator =( const VxNetIdent &rhs );

	EPluginAccess			    getHisAccessPermissionFromMe( EPluginType ePluginType );
	bool						isHisAccessAllowedFromMe( EPluginType ePluginType );

	EPluginAccess			    getMyAccessPermissionFromHim( EPluginType ePluginType );
	bool						isMyAccessAllowedFromHim( EPluginType ePluginType );

	void						setPingTimeMs( uint16_t pingTime );
	uint16_t					getPingTimeMs( void );

	void						setLastSessionTimeMs( int64_t lastSessionTimeGmtMs )		{ m_LastSessionTimeGmtMs = lastSessionTimeGmtMs; }
	int64_t					    getLastSessionTimeMs( void )							    { return m_LastSessionTimeGmtMs; }

	void						debugDumpIdent( void );

	bool                        isValidNetIdent( void );
	bool						isOnlineNameValid( void );

	bool						canRequestJoin( EHostType hostType );
	bool						canJoinImmediate( EHostType hostType ); // request to join will be granted immediate because have sufficient permission

private:
	EPluginAccess			    getPluginAccessState( EPluginType ePluginType, EFriendState eFriendState );

	//=== vars ===//
    uint16_t					m_u16AppVersion{ 0 };
	uint16_t					m_u16PingTimeMs{ 0 };	
    uint16_t					m_NetIdentRes1{ 0 };  
    uint8_t					    m_NetIdentRes2{ 0 };     
    uint8_t						m_NetIdentRes3{ 0 };
    int64_t                     m_NetIdentRes4{ 0 };
    int64_t					    m_LastSessionTimeGmtMs{ 0 };
};

#pragma pack()

//! report a attempt to hack or add spyware etc
///RCODE VxReportHack(	VxNetIdentBase *	poContactIdent,			// users identity info ( may be null if not known then use ipAddress )
///						uint32_t					u32HackLevel,			// 1=severe 2=medium 3=suspicious
///						uint32_t					u32IpAddr,				// ip address if identity not known
///						const char *		pDescription, ... );	// description of hack attempt
RCODE VxReportHack(	EHackerLevel hackerLevel, EHackerReason hackerReason, VxSktBase * sktBase, const char * pDescription, ... );
RCODE VxReportHack(	EHackerLevel hackerLevel, EHackerReason hackerReason, SOCKET skt, const char * ipAddr, const char * pDescription, ... );

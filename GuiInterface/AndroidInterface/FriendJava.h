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
// http://www.nolimitconnect.com
//============================================================================

// android specific code
#ifdef TARGET_OS_ANDROID

class FriendJavaClass : public JavaBaseClass
{
public:
	FriendJavaClass( JavaEnvCache& envCache, jclass javaRefClass );
	virtual ~FriendJavaClass();
	//=== vars ===//
	bool			m_bIsThreadAttached; // if true thread attached and required detachment
	jclass 			m_poJavaRefClass;
	jclass 			m_poJavaObject;

	jmethodID 		m_methConstructor;

	jfieldID		m_u64RelayIdLoPartID;
	jfieldID		m_u64RelayIdHiPartID;
	jfieldID		m_RelayPortID;
	jfieldID		m_as8RelayIPv4ID;
	jfieldID		m_as8RelayIPv6ID;

	jfieldID		m_u8RelayFlagsID;

	jfieldID		m_u64IdLoPartID;
	jfieldID		m_u64IdHiPartID;
	jfieldID		m_OnlinePortID;
	jfieldID		m_as8LocalIPv4ID;
	jfieldID		m_as8OnlineIPv4ID;
	jfieldID		m_as8OnlineIPv6ID;

	jfieldID		m_u8OnlineStatusFlagsID;

	jfieldID		m_as8OnlineNameID;
	jfieldID		m_as8OnlineDescID;
	jfieldID		m_u8FriendMatchID;
	jfieldID		m_u8SearchFlagsID;
	jfieldID		m_u8FileTypeFlagsID;
	jfieldID		m_au8PermissionsID;
	jfieldID		m_u32TruthCnt;		
	jfieldID		m_u32DareCnt;			
	jfieldID		m_RejectedTruthCnt;	
	jfieldID		m_RejectedDareCnt;		
	jfieldID		m_s64LastSessionTimeID;
};

FriendJavaClass * GetFriendJavaClass( void );
void FriendJavaClassThreadExit( unsigned int uiThreadId );
void FriendJavaClassShutdown( void );

bool java2VxNetIdentBase( jobject& o,  VxNetIdent& oIdent );
bool vxNetIdentBase2Java(VxNetIdent * poIdent, jobject& o );
bool vxNetIdentBaseFillFriendJavaClass( VxNetIdent * poIdent, jobject oFriendClass );


#endif //TARGET_OS_ANDROID

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
#include "NativeCommon.h"
#include "FriendJava.h"
#include <CoreLib/VxDebug.h>

//#define DEBUG_JAVAFRIEND 1 

namespace
{
	std::map<unsigned int, FriendJavaClass *>		g_oJavaFriendMap;
	VxMutex											g_JavaFriendListMutex;
}

//============================================================================
FriendJavaClass::FriendJavaClass( JavaEnvCache& envCache, jclass javaRefClass )
: JavaBaseClass( envCache, javaRefClass )
{
}

//============================================================================
FriendJavaClass::~FriendJavaClass()
{
}

//============================================================================
void FriendJavaClassThreadExit( unsigned int uiThreadId )
{
	g_JavaFriendListMutex.lock();
	std::map<unsigned int, FriendJavaClass *>::iterator oIter;

	oIter = g_oJavaFriendMap.find(uiThreadId);
	if( oIter != g_oJavaFriendMap.end() )
	{
		delete oIter->second;
		g_oJavaFriendMap.erase( oIter );
	}

	g_JavaFriendListMutex.unlock();
}

//============================================================================
void FriendJavaClassShutdown( void )
{
	g_JavaFriendListMutex.lock();
	std::map<unsigned int, FriendJavaClass *>::iterator oIter;
	for( oIter = g_oJavaFriendMap.begin(); oIter != g_oJavaFriendMap.end(); ++oIter )
	{
		delete oIter->second;
	}

	g_oJavaFriendMap.clear();	
	g_JavaFriendListMutex.unlock();
}

//============================================================================
FriendJavaClass * GetFriendJavaClass( void )
{
	g_JavaFriendListMutex.lock();
	unsigned int uiThreadId = VxGetCurrentThreadTid();
	std::map<unsigned int, FriendJavaClass *>::iterator oIter;

	oIter = g_oJavaFriendMap.find(uiThreadId);
	if( oIter != g_oJavaFriendMap.end() )
	{
		FriendJavaClass * javaFriendClass =  oIter->second;
		g_JavaFriendListMutex.unlock();
		return javaFriendClass;
	}

	FriendJavaClass * poClass = new FriendJavaClass( GetJavaEnvCache(), GetJavaEnvCache().getClassFriendBase() );
	if( false == poClass->attachToThread() )
	{
		LogMsg( LOG_ERROR, "GetNativeToJavaClass: FAIL Thread 0x%x\n", uiThreadId );
		delete poClass;
		g_JavaFriendListMutex.unlock();
		return NULL;
	}

	// get java member id s
	//LogMsg( LOG_INFO, "GetFriendJavaClass: Getting m_u64RelayIdLoPartID\n" );
	poClass->m_u64RelayIdLoPartID 	= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_u64RelayIdLoPart", "J");
	poClass->m_u64RelayIdHiPartID 	= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_u64RelayIdHiPart", "J");
	poClass->m_RelayPortID 			= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_RelayPort", "I");
	poClass->m_as8RelayIPv4ID 		= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_as8RelayIPv4", "Ljava/lang/String;");
	poClass->m_as8RelayIPv6ID 		= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_as8RelayIPv6", "Ljava/lang/String;");

	poClass->m_u64IdLoPartID 		= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_u64IdLoPart", "J");
	poClass->m_u64IdHiPartID 		= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_u64IdHiPart", "J");
	poClass->m_OnlinePortID 		= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_OnlinePort", "I");
	poClass->m_as8LocalIPv4ID 		= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_as8LocalIPv4", "Ljava/lang/String;");
	poClass->m_as8OnlineIPv4ID 		= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_as8OnlineIPv4", "Ljava/lang/String;");
	poClass->m_as8OnlineIPv6ID 		= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_as8OnlineIPv6", "Ljava/lang/String;");

	poClass->m_u8OnlineStatusFlagsID = poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_u8OnlineStatusFlags", "B");

	poClass->m_as8OnlineNameID 		= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_as8OnlineName", "Ljava/lang/String;");
	poClass->m_as8OnlineDescID 		= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_as8OnlineDesc", "Ljava/lang/String;");
	poClass->m_u8SearchFlagsID 		= poClass->m_poEnv->GetFieldID( poClass->m_JavaClass, "m_u8SearchFlags", "B" );
	poClass->m_u8FileTypeFlagsID 	= poClass->m_poEnv->GetFieldID( poClass->m_JavaClass, "m_u8FileTypeFlags", "B" );
	poClass->m_u8FriendMatchID 		= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_u8FriendMatch", "B");
	poClass->m_u8RelayFlagsID 		= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_u8RelayFlags", "B");
	poClass->m_au8PermissionsID 	= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_au8Permissions", "[B");

	poClass->m_u32TruthCnt 			= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_u32TruthCnt", "I");
	poClass->m_u32DareCnt 			= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_u32DareCnt", "I");
	poClass->m_RejectedTruthCnt 	= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_RejectedTruthCnt", "I");
	poClass->m_RejectedDareCnt 		= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_RejectedDareCnt", "I");
	poClass->m_s64LastSessionTimeID = poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_s64LastSessionTime", "J");

	
	//#if(DEBUG_JAVAFRIEND)
//	LogMsg( LOG_INFO, "FriendIdent ids %x %x %x %x - %x %x %x %x - %x %x %x %x %x\n",
//			poClass->m_u64RelayIdLoPartID, poClass->m_u64RelayIdHiPartID, poClass->m_u16RelayPortID, poClass->m_u16RelayPortID,
//			poClass->m_u64IdLoPartID, poClass->m_u64IdHiPartID, poClass->m_as8OnlineNameID, poClass->m_as8OnlineDescID,
//			poClass->m_u16SearchFlagsID, poClass->m_u8FriendMatchID, poClass->m_u8RelayFlagsID, poClass->m_u16OnlinePortID, poClass->m_as8OnlineIPv4ID, poClass->m_u64PermissionsID );
//#endif
	//LogMsg( LOG_INFO, "GetFriendJavaClass: inserting pair\n" );
	g_oJavaFriendMap.insert( std::make_pair(uiThreadId, poClass) );

	//LogMsg( LOG_INFO, "GetFriendJavaClass: Added thread 0x%x env 0x%x FriendJavaClass 0x%x\n", uiThreadId, env, poClass );
	g_JavaFriendListMutex.unlock();
	return poClass;
}

//============================================================================
bool java2VxNetIdentBase( jobject& o, VxNetIdent& oIdent )
{
	FriendJavaClass * poClass = GetFriendJavaClass();
	if( poClass )
	{
		//LogMsg( LOG_INFO, "java2VxNetIdentBase start\n" );
		uint64_t u64IdLoPart =	(uint64_t)poClass->m_poEnv->GetLongField( o, poClass->m_u64RelayIdLoPartID );
		uint64_t u64IdHiPart =  	(uint64_t)poClass->m_poEnv->GetLongField( o, poClass->m_u64RelayIdHiPartID );

		oIdent.m_RelayConnectId.setVxGUID( u64IdHiPart, u64IdLoPart );
		oIdent.m_RelayConnectId.setPort( ( uint16_t )poClass->m_poEnv->GetIntField( o, poClass->m_RelayPortID ) );

		jstring jstrRelayIPv4 = (jstring) poClass->m_poEnv->GetObjectField( o, poClass->m_as8RelayIPv4ID );
		if ( jstrRelayIPv4 )
		{
			const char * pRelayIPv4 = poClass->m_poEnv->GetStringUTFChars( jstrRelayIPv4, 0 );
			oIdent.m_RelayConnectId.m_IPv4OnlineIp.setIp( pRelayIPv4 );
			poClass->m_poEnv->ReleaseStringUTFChars( jstrRelayIPv4, pRelayIPv4 );
			poClass->m_poEnv->DeleteLocalRef( jstrRelayIPv4 );
		}

		jstring jstrRelayIPv6 = (jstring) poClass->m_poEnv->GetObjectField( o, poClass->m_as8RelayIPv6ID );
		if( jstrRelayIPv6 )
		{
			const char * pRelayIPv6 = poClass->m_poEnv->GetStringUTFChars( jstrRelayIPv6, 0 );
			oIdent.m_RelayConnectId.m_IPv6OnlineIp.setIp( pRelayIPv6 );
			poClass->m_poEnv->ReleaseStringUTFChars( jstrRelayIPv6, pRelayIPv6 );
			poClass->m_poEnv->DeleteLocalRef( jstrRelayIPv6 );
		}

		u64IdLoPart =	(uint64_t)poClass->m_poEnv->GetLongField(o, poClass->m_u64IdLoPartID );
		u64IdHiPart =  	(uint64_t)poClass->m_poEnv->GetLongField(o, poClass->m_u64IdHiPartID );
		oIdent.m_DirectConnectId.setVxGUID( u64IdHiPart, u64IdLoPart );
		oIdent.m_DirectConnectId.setPort( ( uint16_t )poClass->m_poEnv->GetIntField( o, poClass->m_OnlinePortID ) );

		jstring jstrLocalIPv4 = (jstring) poClass->m_poEnv->GetObjectField( o, poClass->m_as8LocalIPv4ID );
		if ( jstrLocalIPv4 )
		{
			const char * pLocalIPv4 = poClass->m_poEnv->GetStringUTFChars( jstrLocalIPv4, 0 );
			oIdent.m_LanIPv4.setIp( pLocalIPv4 );
			poClass->m_poEnv->ReleaseStringUTFChars( jstrLocalIPv4, pLocalIPv4 );
			poClass->m_poEnv->DeleteLocalRef( jstrLocalIPv4 );
		}

		jstring jstrOnlineIPv4 = (jstring) poClass->m_poEnv->GetObjectField( o, poClass->m_as8OnlineIPv4ID );
		if ( jstrOnlineIPv4 )
		{
			const char * pOnlineIPv4 = poClass->m_poEnv->GetStringUTFChars( jstrOnlineIPv4, 0 );
			oIdent.m_DirectConnectId.m_IPv4OnlineIp.setIp( pOnlineIPv4 );
			poClass->m_poEnv->ReleaseStringUTFChars( jstrOnlineIPv4, pOnlineIPv4 );		
			poClass->m_poEnv->DeleteLocalRef( jstrOnlineIPv4 );
		}

		jstring jstrOnlineIPv6 = (jstring) poClass->m_poEnv->GetObjectField( o, poClass->m_as8OnlineIPv6ID );
		if ( jstrOnlineIPv6 )
		{
			const char * pOnlineIPv6 = poClass->m_poEnv->GetStringUTFChars( jstrOnlineIPv6, 0 );
			oIdent.m_DirectConnectId.m_IPv6OnlineIp.setIp( pOnlineIPv6 );
			poClass->m_poEnv->ReleaseStringUTFChars( jstrOnlineIPv6, pOnlineIPv6 );
			poClass->m_poEnv->DeleteLocalRef( jstrOnlineIPv6 );
		}

		oIdent.m_u8OnlineStatusFlags = poClass->m_poEnv->GetByteField(o, poClass->m_u8OnlineStatusFlagsID );

		//LogMsg( LOG_INFO, "java2VxNetIdentBase id1 %llx id2 %llx\n", oIdent.getMyOnlineIdHiPart(), oIdent.getMyOnlineIdLoPart() );
		jstring jstrName = (jstring) poClass->m_poEnv->GetObjectField( o, poClass->m_as8OnlineNameID );
		const char * pName = (jstrName == 0) ? 0 : poClass->m_poEnv->GetStringUTFChars(jstrName, 0);
		if( pName )
		{
			strcpy( oIdent.getOnlineName(), pName );
			//LogMsg( LOG_INFO, "java2VxNetIdentBase onlineName %s\n", oIdent.getOnlineName() );
			// Release memory (required or else memory leaks)
			poClass->m_poEnv->ReleaseStringUTFChars(jstrName, pName);
			poClass->m_poEnv->DeleteLocalRef( jstrName );
		}
		else
		{
			LogMsg( LOG_INFO, "java2VxNetIdentBase ERROR null online name\n" );
			return false; // in this case it is invalid.. name is required
		}

		oIdent.getOnlineDescription()[0] = 0; // description is not required
		jstring jstrDesc = (jstring) poClass->m_poEnv->GetObjectField(o, poClass->m_as8OnlineDescID );
		if ( jstrDesc )
		{
			const char * pDesc = (jstrDesc == 0) ? 0 : poClass->m_poEnv->GetStringUTFChars(jstrDesc, 0);
			if( pDesc )
			{
				strcpy( oIdent.getOnlineDescription(), pDesc );
				poClass->m_poEnv->ReleaseStringUTFChars( jstrDesc, pDesc );
				poClass->m_poEnv->DeleteLocalRef( jstrDesc );
			}
		}

		oIdent.m_u8FriendMatch 		= poClass->m_poEnv->GetByteField( o, poClass->m_u8FriendMatchID );
		oIdent.m_u8RelayFlags 		= poClass->m_poEnv->GetByteField( o, poClass->m_u8RelayFlagsID );
		
		oIdent.setSearchFlags( poClass->m_poEnv->GetByteField( o, poClass->m_u8SearchFlagsID ) );
		oIdent.setSharedFileTypes( poClass->m_poEnv->GetByteField( o, poClass->m_u8FileTypeFlagsID ) );

		jbyteArray jArrayPermissions = (jbyteArray) poClass->m_poEnv->GetObjectField(o, poClass->m_au8PermissionsID );
	    poClass->m_poEnv->GetByteArrayRegion(jArrayPermissions, 0, PERMISSION_ARRAY_SIZE, (jbyte *)oIdent.getPluginPermissions() );
		//poClass->m_poEnv->ReleaseByteArrayElements( jArrayPermissions, (jbyte *)oIdent.getPluginPermissions(), JNI_ABORT );
		poClass->m_poEnv->DeleteLocalRef( jArrayPermissions );

		oIdent.setTruthCount( poClass->m_poEnv->GetIntField(o, poClass->m_u32TruthCnt ) );
		oIdent.setDareCount( poClass->m_poEnv->GetIntField(o, poClass->m_u32DareCnt ) );
		oIdent.setRejectedTruthCount( (uint16_t)poClass->m_poEnv->GetIntField(o, poClass->m_RejectedTruthCnt ) );
		oIdent.setRejectedDareCount( (uint16_t)poClass->m_poEnv->GetIntField(o, poClass->m_RejectedDareCnt ) );
		oIdent.setLastSessionTime( poClass->m_poEnv->GetLongField(o, poClass->m_s64LastSessionTimeID ) );

		//LogMsg( LOG_DEBUG, "java2VxNetIdentBase %s 0x%llx 0x%llx\n",
		//			oIdent.getOnlineName(),
		//			oIdent.getMyOnlineIdHiPart(),
		//			oIdent.getMyOnlineIdLoPart() );

		return true;
	}
	else
	{
		LogMsg( LOG_INFO, "java2VxNetIdentBase: FAIL GetFriendJavaClass\n" );
	}
    return false;
}

//============================================================================
void javaFillFriendClass( FriendJavaClass * poClass, VxNetIdent * poIdent, jobject poFriendClass )
{
	//LogMsg( LOG_DEBUG, "Fill Friend start\n");
	uint64_t u64RelayHiPart = poIdent->m_RelayConnectId.getOnlineId().getVxGUIDHiPart();
	uint64_t u64RelayLoPart = poIdent->m_RelayConnectId.getOnlineId().getVxGUIDLoPart();
	//LogMsg( LOG_DEBUG, "set relay lo\n");
	poClass->m_poEnv->SetLongField(poFriendClass, poClass->m_u64RelayIdLoPartID, u64RelayLoPart);
	//LogMsg( LOG_DEBUG, "set relay hi\n");
	poClass->m_poEnv->SetLongField(poFriendClass, poClass->m_u64RelayIdHiPartID, u64RelayHiPart);
	poClass->m_poEnv->SetIntField(poFriendClass, poClass->m_RelayPortID, poIdent->m_RelayConnectId.getPort() );

	std::string strRelayIPv4;
	poIdent->m_RelayConnectId.getIPv4( strRelayIPv4 );
	//LogMsg( LOG_DEBUG, "set relay ipv4\n");
	poClass->m_poEnv->SetObjectField(poFriendClass, poClass->m_as8RelayIPv4ID, poClass->m_poEnv->NewStringUTF( strRelayIPv4.c_str() ) );
	std::string strRelayIPv6;
	poIdent->m_RelayConnectId.getIPv6( strRelayIPv6 );
	poClass->m_poEnv->SetObjectField(poFriendClass, poClass->m_as8RelayIPv6ID, poClass->m_poEnv->NewStringUTF( strRelayIPv6.c_str() ) );

	uint64_t u64OnlineIdHiPart = poIdent->getMyOnlineId().getVxGUIDHiPart();
	uint64_t u64OnlineIdLoPart = poIdent->getMyOnlineId().getVxGUIDLoPart();
	//LogMsg( LOG_DEBUG, "vxNetIdentBase2Java %s 0x%llx 0x%llx\n", poIdent->getOnlineName(), u64OnlineIdHiPart, u64OnlineIdLoPart );
	poClass->m_poEnv->SetLongField(poFriendClass, poClass->m_u64IdLoPartID, u64OnlineIdLoPart );
	poClass->m_poEnv->SetLongField(poFriendClass, poClass->m_u64IdHiPartID, u64OnlineIdHiPart);
	poClass->m_poEnv->SetIntField( poFriendClass, poClass->m_OnlinePortID, poIdent->m_DirectConnectId.getPort() );

	std::string strLocalIPv4 = poIdent->m_LanIPv4.toStdString();
	poClass->m_poEnv->SetObjectField(poFriendClass, poClass->m_as8LocalIPv4ID, poClass->m_poEnv->NewStringUTF( strLocalIPv4.c_str() ) );

	std::string strOnlineIPv4;
	poIdent->m_DirectConnectId.getIPv4( strOnlineIPv4 );
	poClass->m_poEnv->SetObjectField(poFriendClass, poClass->m_as8OnlineIPv4ID, poClass->m_poEnv->NewStringUTF( strOnlineIPv4.c_str() ) );

	std::string strOnlineIPv6;
	poIdent->m_DirectConnectId.getIPv6( strOnlineIPv6 );
	poClass->m_poEnv->SetObjectField(poFriendClass, poClass->m_as8OnlineIPv6ID, poClass->m_poEnv->NewStringUTF( strOnlineIPv6.c_str() ) );

	poClass->m_poEnv->SetByteField(poFriendClass, poClass->m_u8OnlineStatusFlagsID, poIdent->m_u8OnlineStatusFlags);
	poClass->m_poEnv->SetByteField(poFriendClass, poClass->m_u8RelayFlagsID, poIdent->m_u8RelayFlags);

	poClass->m_poEnv->SetObjectField( poFriendClass, poClass->m_as8OnlineNameID, poClass->m_poEnv->NewStringUTF( poIdent->getOnlineName() ));
	poClass->m_poEnv->SetObjectField( poFriendClass, poClass->m_as8OnlineDescID, poClass->m_poEnv->NewStringUTF( poIdent->getOnlineDescription() ));
	poClass->m_poEnv->SetByteField( poFriendClass, poClass->m_u8FriendMatchID, poIdent->m_u8FriendMatch);
	poClass->m_poEnv->SetByteField( poFriendClass, poClass->m_u8SearchFlagsID, poIdent->getSearchFlags() );
	poClass->m_poEnv->SetByteField( poFriendClass, poClass->m_u8FileTypeFlagsID, poIdent->getSharedFileTypes() );
	//LogMsg( LOG_DEBUG, "set plugin permissions\n");
	jbyteArray jArrayPermissions = (jbyteArray) poClass->m_poEnv->NewByteArray( PERMISSION_ARRAY_SIZE );
	poClass->m_poEnv->SetByteArrayRegion( jArrayPermissions, 0, PERMISSION_ARRAY_SIZE, (jbyte *)poIdent->getPluginPermissions() );
	poClass->m_poEnv->SetObjectField( poFriendClass, poClass->m_au8PermissionsID, jArrayPermissions );
	//poClass->m_poEnv->ReleaseByteArrayElements( jArrayPermissions, (jbyte *)poIdent->getPluginPermissions(), JNI_ABORT );
	poClass->m_poEnv->DeleteLocalRef( jArrayPermissions );

	//LogMsg( LOG_DEBUG, "set truth count\n");
	poClass->m_poEnv->SetIntField(poFriendClass, poClass->m_u32TruthCnt, poIdent->getTruthCount() );
	poClass->m_poEnv->SetIntField(poFriendClass, poClass->m_u32DareCnt, poIdent->getDareCount() );
	poClass->m_poEnv->SetIntField(poFriendClass, poClass->m_RejectedTruthCnt, poIdent->getRejectedTruthCount() );
	poClass->m_poEnv->SetIntField(poFriendClass, poClass->m_RejectedDareCnt, poIdent->getRejectedDareCount() );

	// LogMsg( LOG_DEBUG, "set session time\n");
	poClass->m_poEnv->SetLongField(poFriendClass, poClass->m_s64LastSessionTimeID, poIdent->getLastSessionTime() );
	//LogMsg( LOG_DEBUG, "Fill Friend done\n");
}

//============================================================================
bool vxNetIdentBase2Java( VxNetIdent * poIdent, jobject& oFriendClass )
{
	FriendJavaClass * poClass = GetFriendJavaClass();
	if( NULL == poClass )
	{
		LogMsg( LOG_INFO, "java2VxNetIdentBase: FAIL GetFriendJavaClass\n" );
		return false;
	}

	oFriendClass = poClass->creatNewObj();
	if( NULL == oFriendClass )
	{
		LogMsg( LOG_ERROR, "JavaClassCreate: FAIL create a FriendJava object");
		return false;
	}

	//LogMsg( LOG_DEBUG, "Calling javaFillFriendClass\n");
	javaFillFriendClass( poClass, poIdent, oFriendClass );
	return true;
}

//============================================================================
bool vxNetIdentBaseFillFriendJavaClass( VxNetIdent * poIdent, jobject poFriendClass )
{
	FriendJavaClass * poClass = GetFriendJavaClass();
	if( NULL == poClass )
	{
		LogMsg( LOG_INFO, "vxNetIdentBaseFillFriendJavaClass: FAIL GetFriendJavaClass\n" );
		return false;
	}

	if( NULL == poFriendClass )
	{
		LogMsg( LOG_ERROR, "vxNetIdentBaseFillFriendJavaClass: FAIL because NULL FriendJava object" );
		return false;
	}

	javaFillFriendClass( poClass, poIdent, poFriendClass );
	return true;
}

#endif //TARGET_OS_ANDROID

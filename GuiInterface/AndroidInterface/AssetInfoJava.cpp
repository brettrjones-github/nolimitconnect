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
#include "AssetInfoJava.h"
#include "GuidIdJava.h"
#include "../CoreLib/VxDebug.h"

#include <GoTvCore/GoTvP2P/AssetMgr/AssetInfo.h>

#define DEBUG_JAVAAssetInfo 1 

namespace
{
	std::map<unsigned int, JavaAssetInfoClass *>	g_oJavaAssetInfoMap;
	VxMutex											g_AssetInfoJavaListMutex;
}


//============================================================================
JavaAssetInfoClass::JavaAssetInfoClass( JavaEnvCache& envCache, jclass javaRefClass )
: JavaBaseClass( envCache, javaRefClass )
{
}

//============================================================================
JavaAssetInfoClass::~JavaAssetInfoClass()
{
}

//============================================================================
void JavaAssetInfoClassThreadExit( unsigned int uiThreadId )
{
	std::map<unsigned int, JavaAssetInfoClass *>::iterator oIter;
	g_AssetInfoJavaListMutex.lock();
	oIter = g_oJavaAssetInfoMap.find(uiThreadId);
	if( oIter != g_oJavaAssetInfoMap.end() )
	{
		delete oIter->second;
		g_oJavaAssetInfoMap.erase( oIter );
	}

	g_AssetInfoJavaListMutex.unlock();
}

//============================================================================
void JavaAssetInfoClassShutdown( void )
{
	g_AssetInfoJavaListMutex.lock();
	std::map<unsigned int, JavaAssetInfoClass *>::iterator oIter;
	for( oIter = g_oJavaAssetInfoMap.begin(); oIter != g_oJavaAssetInfoMap.end(); ++oIter )
	{
		delete oIter->second;
	}

	g_oJavaAssetInfoMap.clear();	
	g_AssetInfoJavaListMutex.unlock();
}

//============================================================================
JavaAssetInfoClass * GetJavaAssetInfoClass( void )
{
	unsigned int uiThreadId = VxGetCurrentThreadTid();
	std::map<unsigned int, JavaAssetInfoClass *>::iterator oIter;
	g_AssetInfoJavaListMutex.lock();
	oIter = g_oJavaAssetInfoMap.find(uiThreadId);
	if( oIter != g_oJavaAssetInfoMap.end() )
	{
		JavaAssetInfoClass * javaFriendClass =  oIter->second;
		g_AssetInfoJavaListMutex.unlock();
		return javaFriendClass;
	}

	// does not exist for this thread
	JavaAssetInfoClass * poClass = new JavaAssetInfoClass( GetJavaEnvCache(), GetJavaEnvCache().getClassAssetGuiInfo() );
	if( false == poClass->attachToThread() )
	{
		g_AssetInfoJavaListMutex.unlock();
		LogMsg( LOG_ERROR, "GetJavaAssetInfoClass: FAIL Thread 0x%x\n", uiThreadId );
		delete poClass;
		return NULL;
	}

	// get java member id s
	//LogMsg( LOG_INFO, "GetJavaAssetInfoClass: Getting member ids\n" );

	poClass->m_AssetNameID 			= poClass->m_poEnv->GetFieldID( poClass->m_JavaClass, "m_AssetName", "Ljava/lang/String;" );
	poClass->m_AssetTagID 			= poClass->m_poEnv->GetFieldID( poClass->m_JavaClass, "m_AssetTag", "Ljava/lang/String;" );

	poClass->m_UniqueIdID 			= poClass->m_poEnv->GetFieldID( poClass->m_JavaClass, "m_UniqueId", "Lcom/waycoolapps/myp2pweb/VxGUID;" );
	poClass->m_CreatorIdID 			= poClass->m_poEnv->GetFieldID( poClass->m_JavaClass, "m_CreatorId", "Lcom/waycoolapps/myp2pweb/VxGUID;" );
	poClass->m_HistoryIdID 			= poClass->m_poEnv->GetFieldID( poClass->m_JavaClass, "m_HistoryId", "Lcom/waycoolapps/myp2pweb/VxGUID;" );

	poClass->m_AssetHashID 			= poClass->m_poEnv->GetFieldID( poClass->m_JavaClass, "m_AssetHash", "[B" );

	poClass->m_s64AssetLenID 		= poClass->m_poEnv->GetFieldID( poClass->m_JavaClass, "m_AssetLen", "J" );
	poClass->m_u16AssetTypeID 		= poClass->m_poEnv->GetFieldID( poClass->m_JavaClass, "m_AssetType", "S" );
	poClass->m_LocationFlagsID 		= poClass->m_poEnv->GetFieldID( poClass->m_JavaClass, "m_LocationFlags", "I" );
	poClass->m_CreationTimeID 		= poClass->m_poEnv->GetFieldID( poClass->m_JavaClass, "m_CreationTime", "J" );
	poClass->m_AssetSendStateID 		= poClass->m_poEnv->GetFieldID( poClass->m_JavaClass, "m_AssetSendState", "I" );
	//LogMsg( LOG_INFO, "GetJavaAssetInfoClass: Getting m_u64RelayIdLoPartID\n" );

	//#if(DEBUG_JAVAFRIEND)
	//	LogMsg( LOG_INFO, "FriendIdent ids %x %x %x %x - %x %x %x %x - %x %x %x %x %x\n",
	//			poClass->m_u64RelayIdLoPartID, poClass->m_u64RelayIdHiPartID, poClass->m_u16RelayPortID, poClass->m_u16RelayPortID,
	//			poClass->m_u64IdLoPartID, poClass->m_u64IdHiPartID, poClass->m_as8OnlineNameID, poClass->m_as8OnlineDescID,
	//			poClass->m_u16SearchFlagsID, poClass->m_u8FriendMatchID, poClass->m_u8RelayFlagsID, poClass->m_u16OnlinePortID, poClass->m_as8OnlineIPv4ID, poClass->m_u64PermissionsID );
	//#endif
	//LogMsg( LOG_INFO, "GetJavaAssetInfoClass: inserting pair\n" );
	g_oJavaAssetInfoMap.insert( std::make_pair(uiThreadId, poClass) );

	//LogMsg( LOG_INFO, "GetJavaAssetInfoClass: Added thread 0x%x JavaAssetInfoClass 0x%x\n", uiThreadId, poClass );
	g_AssetInfoJavaListMutex.unlock();
	return poClass;
}

//============================================================================
bool Java2AssetInfo( jobject& o, AssetInfo * assetInfo )
{
	JavaAssetInfoClass * poClass = GetJavaAssetInfoClass();
	if( poClass )
	{
		//LogMsg( LOG_INFO, "Java2AssetInfo start\n" );
		jstring jstrAssetName = ( jstring ) poClass->m_poEnv->GetObjectField( o, poClass->m_AssetNameID );
		const char * pAssetName = poClass->m_poEnv->GetStringUTFChars( jstrAssetName, 0 );
		assetInfo->setAssetName( pAssetName );
		poClass->m_poEnv->ReleaseStringUTFChars( jstrAssetName, pAssetName );

		jstring jstrAssetTag = ( jstring ) poClass->m_poEnv->GetObjectField( o, poClass->m_AssetTagID );
		const char * pAssetTag = poClass->m_poEnv->GetStringUTFChars( jstrAssetTag, 0 );
		assetInfo->setAssetTag( pAssetTag );
		poClass->m_poEnv->ReleaseStringUTFChars( jstrAssetTag, pAssetTag );

		jobject jobjUniqueId = ( jobject ) poClass->m_poEnv->GetObjectField( o, poClass->m_UniqueIdID );
		JavaGuidId2VxGUID( jobjUniqueId, assetInfo->getAssetUniqueId() );
		jobject jobjCreatorId = ( jobject ) poClass->m_poEnv->GetObjectField( o, poClass->m_CreatorIdID );
		JavaGuidId2OnlineId( jobjCreatorId, assetInfo->getCreatorId() );
		jobject jobjHistoryId = ( jobject ) poClass->m_poEnv->GetObjectField( o, poClass->m_HistoryIdID );
		JavaGuidId2OnlineId( jobjHistoryId, assetInfo->getHistoryId() );
		
		jbyteArray jArrayHashId = ( jbyteArray ) poClass->m_poEnv->GetObjectField( o, poClass->m_AssetHashID );
		poClass->m_poEnv->GetByteArrayRegion( jArrayHashId, 0, 20, ( jbyte * )assetInfo->getAssetHashId().getHashData() );

		int64_t s64AssetLen			=	( int64_t )poClass->m_poEnv->GetLongField( o, poClass->m_s64AssetLenID );
		assetInfo->setAssetLength( s64AssetLen );
		uint16_t u16AssetType		=	( uint16_t )poClass->m_poEnv->GetShortField( o, poClass->m_u16AssetTypeID );
		assetInfo->setAssetType( ( EAssetType )u16AssetType );
		uint32_t u32LocationFlags	=	( uint32_t )poClass->m_poEnv->GetIntField( o, poClass->m_LocationFlagsID );
		assetInfo->setLocationFlags( u32LocationFlags );
		uint64_t u64CreationTime		=  	( uint64_t )poClass->m_poEnv->GetLongField( o, poClass->m_CreationTimeID );
		assetInfo->setCreationTime( ( time_t ) (u64CreationTime / 1000) );
		EAssetSendState sendState =	( EAssetSendState )poClass->m_poEnv->GetIntField( o, poClass->m_AssetSendStateID );
		assetInfo->setAssetSendState( sendState );
	
		//LogMsg( LOG_INFO, "Java2AssetInfo done\n" );
		return true;
	}
	else
	{
		LogMsg( LOG_INFO, "Java2AssetInfo: FAIL GetJavaAssetInfoClass\n" );
	}

    return false;
}

//============================================================================
void JavaFillAssetInfoClass( JavaAssetInfoClass * poClass, AssetInfo * assetInfo, jobject jdestObj )
{
	//LogMsg( LOG_DEBUG, "Fill AssetInfo start\n");
	
	poClass->m_poEnv->SetObjectField( jdestObj, poClass->m_AssetNameID, poClass->m_poEnv->NewStringUTF( assetInfo->getAssetName().c_str() ) );
	poClass->m_poEnv->SetObjectField( jdestObj, poClass->m_AssetTagID, poClass->m_poEnv->NewStringUTF( assetInfo->getAssetTag().c_str() ) );

	jbyteArray jArrayHashId = ( jbyteArray ) poClass->m_poEnv->GetObjectField( jdestObj, poClass->m_AssetHashID );
	poClass->m_poEnv->SetByteArrayRegion( jArrayHashId, 0, 20, ( jbyte * )assetInfo->getAssetHashId().getHashData() );

	jobject jobjUniqueId = ( jobject ) poClass->m_poEnv->GetObjectField( jdestObj, poClass->m_UniqueIdID );
	VxGUID2JavaGuidId( assetInfo->getAssetUniqueId(), jobjUniqueId, false );

	jobject jobjCreatorId = ( jobject ) poClass->m_poEnv->GetObjectField( jdestObj, poClass->m_CreatorIdID );
	VxGUID2JavaGuidId( assetInfo->getCreatorId(), jobjCreatorId, false );

	jobject jobjHistoryId = ( jobject ) poClass->m_poEnv->GetObjectField( jdestObj, poClass->m_HistoryIdID );
	VxGUID2JavaGuidId( assetInfo->getHistoryId(), jobjHistoryId, false );

	poClass->m_poEnv->SetLongField( jdestObj, poClass->m_s64AssetLenID, assetInfo->getAssetLength() );
	poClass->m_poEnv->SetShortField( jdestObj, poClass->m_u16AssetTypeID, ( jshort )assetInfo->getAssetType() );
	poClass->m_poEnv->SetIntField( jdestObj, poClass->m_LocationFlagsID, assetInfo->getLocationFlags() );
	int64_t creationTime = assetInfo->getCreationTime();
	creationTime *= 1000; // java needs milliseconds
	poClass->m_poEnv->SetLongField( jdestObj, poClass->m_CreationTimeID, (jlong)( creationTime ) );
	poClass->m_poEnv->SetIntField( jdestObj, poClass->m_AssetSendStateID, (jint)assetInfo->getAssetSendState() );

	//LogMsg( LOG_DEBUG, "Fill AssetInfo done\n");
}

//============================================================================
bool AssetInfo2Java( AssetInfo * passetInfo, jobject& oAssetInfoClass )
{
	JavaAssetInfoClass * poClass = GetJavaAssetInfoClass();
	if( NULL == poClass )
	{
		LogMsg( LOG_INFO, "Java2AssetInfo: FAIL GetJavaAssetInfoClass\n" );
		return false;
	}

	oAssetInfoClass = poClass->creatNewObj();

	if( NULL == oAssetInfoClass )
	{
		LogMsg( LOG_ERROR, "JavaClassCreate: FAIL create a AssetInfoJava object" );
		return false;
	}

	//LogMsg( LOG_DEBUG, "Calling javaFillAssetInfoClass\n");
	JavaFillAssetInfoClass( poClass, passetInfo, oAssetInfoClass );
	return true;
}

//============================================================================
bool AssetInfoFillJavaAssetInfoClass( AssetInfo * passetInfo, jobject poAssetInfoClass )
{
	JavaAssetInfoClass * poClass = GetJavaAssetInfoClass();
	if( NULL == poClass )
	{
		LogMsg( LOG_INFO, "AssetInfoBaseFillJavaAssetInfoClass: FAIL GetJavaAssetInfoClass\n" );
		return false;
	}

	if( NULL == poAssetInfoClass )
	{
		LogMsg( LOG_ERROR, "AssetInfoBaseFillJavaAssetInfoClass: FAIL because NULL poAssetInfoClass object" );
		return false;
	}

	JavaFillAssetInfoClass( poClass, passetInfo, poAssetInfoClass );
	return true;
}

#endif //TARGET_OS_ANDROID

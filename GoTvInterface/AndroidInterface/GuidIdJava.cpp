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
#include "GuidIdJava.h"

namespace
{
	std::map<unsigned int, JavaGuidIdClass *>		g_oJavaGuidIdMap;
	VxMutex											g_GuidIdavaListMutex;
}

//============================================================================
JavaGuidIdClass::JavaGuidIdClass( JavaEnvCache& envCache, jclass javaRefClass )
: JavaBaseClass( envCache, javaRefClass )
{
}

//============================================================================
JavaGuidIdClass::~JavaGuidIdClass()
{
}

//============================================================================
void JavaGuidIdClassThreadExit( unsigned int uiThreadId )
{
	std::map<unsigned int, JavaGuidIdClass *>::iterator oIter;
	g_GuidIdavaListMutex.lock();
	oIter = g_oJavaGuidIdMap.find(uiThreadId);
	if( oIter != g_oJavaGuidIdMap.end() )
	{
		delete oIter->second;
		g_oJavaGuidIdMap.erase( oIter );
	}

	g_GuidIdavaListMutex.unlock();
}

//============================================================================
void JavaGuidIdClassShutdown( void )
{
	std::map<unsigned int, JavaGuidIdClass *>::iterator oIter;
	g_GuidIdavaListMutex.lock();
	for( oIter = g_oJavaGuidIdMap.begin(); oIter != g_oJavaGuidIdMap.end(); ++oIter )
	{
		delete oIter->second;
	}

	g_oJavaGuidIdMap.clear();	
	g_GuidIdavaListMutex.unlock();
}

//============================================================================
JavaGuidIdClass * GetJavaGuidIdClass( void )
{
	unsigned int uiThreadId = VxGetCurrentThreadTid();
	std::map<unsigned int, JavaGuidIdClass *>::iterator oIter;

	g_GuidIdavaListMutex.lock();
	oIter = g_oJavaGuidIdMap.find( uiThreadId );
	if( oIter != g_oJavaGuidIdMap.end() )
	{
		JavaGuidIdClass * javaGuidIdClass =  oIter->second;
		g_GuidIdavaListMutex.unlock();
		return javaGuidIdClass;
	}

	// does not exist for this thread
	JavaGuidIdClass * poClass = new JavaGuidIdClass( GetJavaEnvCache(), GetJavaEnvCache().getClassGuidId() );
	if( false == poClass->attachToThread() )
	{
		LogMsg( LOG_ERROR, "GetNativeToJavaClass: FAIL Thread 0x%x\n", uiThreadId );
		delete poClass;
		g_GuidIdavaListMutex.unlock();
		return NULL;
	}
	// get java member id s
	//LogMsg( LOG_INFO, "GetJavaGuidIdClass: Getting m_u64GuidIdLoPartID\n" );
	poClass->m_u64GuidIdLoPartID 	= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_u64IdLoPart", "J");
	poClass->m_u64GuidIdHiPartID 	= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_u64IdHiPart", "J");

	//LogMsg( LOG_INFO, "GetJavaGuidIdClass: inserting pair\n" );
	g_oJavaGuidIdMap.insert( std::make_pair(uiThreadId, poClass) );

	//LogMsg( LOG_INFO, "GetJavaGuidIdClass: Added thread 0x%x env 0x%x JavaGuidIdClass 0x%x\n", uiThreadId, env, poClass );
	g_GuidIdavaListMutex.unlock();
	return poClass;

}

//============================================================================
bool JavaGuidId2VxGUID( jobject& o, VxGUID& guidId )
{
	JavaGuidIdClass * poClass = GetJavaGuidIdClass();
	if( poClass )
	{
		//LogMsg( LOG_INFO, "java2VxNetIdentBase start\n" );
		uint64_t u64IdLoPart =	( uint64_t )poClass->m_poEnv->GetLongField( o, poClass->m_u64GuidIdLoPartID );
		uint64_t u64IdHiPart =  	( uint64_t )poClass->m_poEnv->GetLongField( o, poClass->m_u64GuidIdHiPartID );

		guidId.setVxGUID( u64IdHiPart, u64IdLoPart );
		return true;
	}
	else
	{
		LogMsg( LOG_INFO, "java2VxNetIdentBase: FAIL GetJavaGuidIdClass\n" );
	}

    return false;
}

//============================================================================
bool JavaGuidId2OnlineId( jobject& o, VxGUID& onlineId )
{
	JavaGuidIdClass * poClass = GetJavaGuidIdClass();
	if ( poClass )
	{
		//LogMsg( LOG_INFO, "java2VxNetIdentBase start\n" );
		uint64_t u64IdLoPart =	( uint64_t )poClass->m_poEnv->GetLongField( o, poClass->m_u64GuidIdLoPartID );
		uint64_t u64IdHiPart =  	( uint64_t )poClass->m_poEnv->GetLongField( o, poClass->m_u64GuidIdHiPartID );

		onlineId.setVxGUID( u64IdHiPart, u64IdLoPart );
		return true;
	}
	else
	{
		LogMsg( LOG_INFO, "java2VxNetIdentBase: FAIL GetJavaGuidIdClass\n" );
	}
	return false;
}

//============================================================================
void JavaFillGuidIdClass( JavaGuidIdClass * poClass, VxGUID& guidId, jobject poGuidIdClass )
{
	//LogMsg( LOG_DEBUG, "Fill VxGUID start\n" );
	uint64_t u64GuidHiPart = guidId.getVxGUIDHiPart();
	uint64_t u64GuidLoPart = guidId.getVxGUIDLoPart();
	//LogMsg( LOG_DEBUG, "set id lo\n" );
	poClass->m_poEnv->SetLongField( poGuidIdClass, poClass->m_u64GuidIdLoPartID, u64GuidLoPart );
	//LogMsg( LOG_DEBUG, "set id hi\n" );
	poClass->m_poEnv->SetLongField( poGuidIdClass, poClass->m_u64GuidIdHiPartID, u64GuidHiPart );
	//LogMsg( LOG_DEBUG, "Fill VxGUID done\n" );
}

//============================================================================
bool VxGUID2JavaGuidId( VxGUID& guidId, jobject& oGuidIdClass, bool createNewJavaObj )
{
	JavaGuidIdClass * poClass = GetJavaGuidIdClass();
	if( NULL == poClass )
	{
		LogMsg( LOG_INFO, "java2VxNetIdentBase: FAIL GetJavaGuidIdClass\n" );
		return false;
	}

	if ( createNewJavaObj )
	{
		oGuidIdClass = poClass->creatNewObj();
	}

	if( NULL == oGuidIdClass )
	{
		LogMsg( LOG_ERROR, "JavaClassCreate: FAIL create a GuidIdJava object"  );
		return false;
	}

	//LogMsg( LOG_DEBUG, "Calling javaFillGuidIdClass\n");
	JavaFillGuidIdClass( poClass, guidId, oGuidIdClass );
	return true;
}

//============================================================================
bool OnlineId2JavaGuidId( VxGUID& onlineId, jobject& oGuidIdClass, bool createNewJavaObj )
{
	JavaGuidIdClass * poClass = GetJavaGuidIdClass();
	if ( NULL == poClass )
	{
		LogMsg( LOG_INFO, "OnlineId2JavaGuidId: FAIL GetJavaGuidIdClass\n" );
		return false;
	}

	if ( createNewJavaObj )
	{
		oGuidIdClass = poClass->creatNewObj();
	}

	if ( NULL == oGuidIdClass )
	{
		LogMsg( LOG_ERROR, "OnlineId2JavaGuidId: FAIL create a GuidIdJava object" );
		return false;
	}

	LogMsg( LOG_DEBUG, "Calling JavaFillGuidIdClass\n" );
	JavaFillGuidIdClass( poClass, onlineId, oGuidIdClass );
	return true;
}

#endif //TARGET_OS_ANDROID

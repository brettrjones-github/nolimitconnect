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

//Java Type JNI Type machine dependent
//C/C++ typedef Signature Call...Method
//Get...Field 
//boolean jboolean unsigned char Z Boolean 
//byte jbyte signed char B Byte 
//char jchar unsigned short C Char 
//short jshort short S Short 
//int jint int I Int 
//long jlong long J Long 
//float jfloat float F Float 
//double jdouble double D Double 
//void void V Void 
//nonprimitive jobject *... L...; Object  

// android specific code
#ifdef TARGET_OS_ANDROID
#include "NativeCommon.h"
#include <NetLib/NetSettings.h>
#include "NetSettingsJava.h"

#define DEBUG_JAVAFRIEND 1 

namespace
{
	std::map<unsigned int, NetSettingsJavaClass *>	g_oNetSettingsJavaMap;
	VxMutex											g_NetSettingsJavaListMutex;
}

//============================================================================
NetSettingsJavaClass::NetSettingsJavaClass( JavaEnvCache& envCache, jclass javaRefClass )
: JavaBaseClass( envCache, javaRefClass )
{
}

//============================================================================
NetSettingsJavaClass::~NetSettingsJavaClass()
{
}

//============================================================================
void NetSettingsJavaClassThreadExit( unsigned int uiThreadId )
{
	std::map<unsigned int, NetSettingsJavaClass *>::iterator oIter;
	g_NetSettingsJavaListMutex.lock();
	oIter = g_oNetSettingsJavaMap.find(uiThreadId);
	if( oIter != g_oNetSettingsJavaMap.end() )
	{
		delete oIter->second;
		g_oNetSettingsJavaMap.erase( oIter );
	}

	g_NetSettingsJavaListMutex.unlock();
}

//============================================================================
void NetSettingsJavaClassShutdown( void )
{
	std::map<unsigned int, NetSettingsJavaClass *>::iterator oIter;
	g_NetSettingsJavaListMutex.lock();
	for( oIter = g_oNetSettingsJavaMap.begin(); oIter != g_oNetSettingsJavaMap.end(); ++oIter )
	{
		delete oIter->second;
	}

	g_oNetSettingsJavaMap.clear();	
	g_NetSettingsJavaListMutex.unlock();
}

//============================================================================
NetSettingsJavaClass * GetNetSettingsJavaClass( void )
{
	unsigned int uiThreadId = VxGetCurrentThreadTid();
	std::map<unsigned int, NetSettingsJavaClass *>::iterator oIter;

	g_NetSettingsJavaListMutex.lock();
	oIter = g_oNetSettingsJavaMap.find(uiThreadId);
	if( oIter != g_oNetSettingsJavaMap.end() )
	{
		NetSettingsJavaClass * netSettingsJava = oIter->second;
		g_NetSettingsJavaListMutex.unlock();
		return netSettingsJava;
	}

	NetSettingsJavaClass * poClass = new NetSettingsJavaClass( GetJavaEnvCache(), GetJavaEnvCache().getClassNetSettings() );
	if( false == poClass->attachToThread() )
	{
		LogMsg( LOG_ERROR, "GetNativeToJavaClass: FAIL Thread 0x%x\n", uiThreadId );
		delete poClass;
		g_NetSettingsJavaListMutex.unlock();
		return NULL;
	}

	// get java member id s
	//LogMsg( LOG_INFO, "GetNetSettingsJavaClass: Getting ID\n" );
	poClass->m_NetworkNameID 				= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_NetworkName", "Ljava/lang/String;");
	poClass->m_NetworkHostUrlID 		= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_NetworkHostUrl", "Ljava/lang/String;");
	poClass->m_HostWebsiteUrlID 			= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_HostWebsiteUrl", "Ljava/lang/String;");

	poClass->m_ThisDeviceIsAnHostID 		= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_ThisDeviceIsAnHost", "Z");
	poClass->m_MyTcpInPortID 				= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_MyTcpInPort", "I");
	poClass->m_MyMulticastPortID 			= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_MyMulticastPort", "I");

	poClass->m_MulticastEnableID 			= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_MulticastEnable", "Z");
	poClass->m_UseUpnpPortForwardID 		= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_UseUpnpPortForward", "Z");

	//LogMsg( LOG_INFO, "GetNetSettingsJavaClass: inserting pair\n" );
	g_oNetSettingsJavaMap.insert( std::make_pair(uiThreadId, poClass) );
#ifdef DEBUG_THREADS
	LogMsg( LOG_INFO, "GetNetSettingsJavaClass: Added thread 0x%x env 0x%x NetSettingsJavaClass 0x%x\n", uiThreadId, env, poClass );
#endif // DEBUG_THREADS
	g_NetSettingsJavaListMutex.unlock();
	return poClass;
}

//============================================================================
bool Java2NetSettings( jobject& o, NetSettings& netSettings )
{
	NetSettingsJavaClass * poClass = GetNetSettingsJavaClass();
	if( poClass )
	{
		poClass->m_NetworkNameID 			= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_NetworkName", "Ljava/lang/String;");
		poClass->m_NetworkHostUrlID 	= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_NetworkHostUrl", "Ljava/lang/String;");
		poClass->m_HostWebsiteUrlID 		= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_HostWebsiteUrl", "Ljava/lang/String;");

		poClass->m_ThisDeviceIsAnHostID 	= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_ThisDeviceIsAnHost", "Z");
		poClass->m_MyTcpInPortID 			= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_MyTcpInPort", "I");
		poClass->m_MyMulticastPortID 		= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_MyMulticastPort", "I");

		poClass->m_MulticastEnableID 		= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_MulticastEnable", "Z");
		poClass->m_UseUpnpPortForwardID 	= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_UseUpnpPortForward", "Z");

		//LogMsg( LOG_INFO, "Java2NetSettings start\n" );
		jstring jstrNetworkName = (jstring) poClass->m_poEnv->GetObjectField( o, poClass->m_NetworkNameID );
		const char * pNetworkName = poClass->m_poEnv->GetStringUTFChars(jstrNetworkName, 0);
		netSettings.setNetworkName( pNetworkName );
		poClass->m_poEnv->ReleaseStringUTFChars( jstrNetworkName, pNetworkName );

		jstring jstrNetServiceUrl = (jstring) poClass->m_poEnv->GetObjectField( o, poClass->m_NetworkHostUrlID );
		const char * pNetServiceUrl = poClass->m_poEnv->GetStringUTFChars(jstrNetServiceUrl, 0);
		netSettings.setConnectTestUrl( pNetServiceUrl );
		poClass->m_poEnv->ReleaseStringUTFChars( jstrNetServiceUrl, pNetServiceUrl );

		jstring jstrHostWebsiteUrl = (jstring) poClass->m_poEnv->GetObjectField( o, poClass->m_HostWebsiteUrlID );
		const char * pHostWebsiteUrl = poClass->m_poEnv->GetStringUTFChars(jstrHostWebsiteUrl, 0);
		netSettings.setHostWebsiteUrl( pHostWebsiteUrl );
		poClass->m_poEnv->ReleaseStringUTFChars( jstrHostWebsiteUrl, pHostWebsiteUrl );

		uint16_t port = (uint16_t)poClass->m_poEnv->GetIntField( o, poClass->m_MyTcpInPortID );
		netSettings.setMyTcpInPort( port );

		bool bValue = poClass->m_poEnv->GetBooleanField( o, poClass->m_ThisDeviceIsAnHostID );
		netSettings.setIsThisNodeAnHost( bValue );

		uint16_t multicastPort = poClass->m_poEnv->GetIntField( o, poClass->m_MyMulticastPortID );
		netSettings.setMyMulticastPort( multicastPort );

		bValue = poClass->m_poEnv->GetBooleanField( o, poClass->m_MulticastEnableID );
		netSettings.setMulticastEnable( bValue );

		bValue = poClass->m_poEnv->GetBooleanField( o, poClass->m_UseUpnpPortForwardID );
		netSettings.setUseUpnpPortForward( bValue );
		return true;
	}
	else
	{
		LogMsg( LOG_INFO, "java2VxNetIdentBase: FAIL GetNetSettingsJavaClass\n" );
	}

    return false;
}

//============================================================================
void javaFillNetSettingsClass( NetSettingsJavaClass * poClass, NetSettings& netSettings, jobject& oNetSettingsClass )
{
	poClass->m_poEnv->SetObjectField(oNetSettingsClass, poClass->m_NetworkNameID, poClass->m_poEnv->NewStringUTF( netSettings.getNetworkName().c_str() ) );
	poClass->m_poEnv->SetObjectField(oNetSettingsClass, poClass->m_NetworkHostUrlID, poClass->m_poEnv->NewStringUTF( netSettings.getConnectTestUrl().c_str() ) );
	poClass->m_poEnv->SetObjectField(oNetSettingsClass, poClass->m_HostWebsiteUrlID, poClass->m_poEnv->NewStringUTF( netSettings.getHostWebsiteUrl().c_str() ) );

	poClass->m_poEnv->SetBooleanField(oNetSettingsClass, poClass->m_ThisDeviceIsAnHostID, netSettings.getIsThisNodeAnHost() );
	poClass->m_poEnv->SetShortField(oNetSettingsClass, poClass->m_MyTcpInPortID, netSettings.getMyTcpInPort() );
	poClass->m_poEnv->SetShortField(oNetSettingsClass, poClass->m_MyMulticastPortID, netSettings.getMyMulticastPort() );

	poClass->m_poEnv->SetBooleanField(oNetSettingsClass, poClass->m_MulticastEnableID, netSettings.getMulticastEnable() );
	poClass->m_poEnv->SetBooleanField(oNetSettingsClass, poClass->m_UseUpnpPortForwardID, netSettings.getUseUpnpPortForward() );
}

//============================================================================
bool NetSettings2Java( NetSettings& netSettings, jobject& oNetSettingsClass )
{
	NetSettingsJavaClass * poClass = GetNetSettingsJavaClass();
	if( NULL == poClass )
	{
		LogMsg( LOG_INFO, "java2VxNetIdentBase: FAIL GetNetSettingsJavaClass\n" );
		return false;
	}

	oNetSettingsClass = poClass->creatNewObj();

	if( NULL == oNetSettingsClass )
	{
		LogMsg( LOG_ERROR, "NetSettings2Java: FAIL create a NetSettingsJava object" );
		return false;
	}

	javaFillNetSettingsClass( poClass, netSettings, oNetSettingsClass );
	return true;
}


//============================================================================
bool FillNetSettingsJavaClass( NetSettings& netSettings, jobject& oNetSettingsClass )
{
	NetSettingsJavaClass * poClass = GetNetSettingsJavaClass();
	if( NULL == poClass )
	{
		LogMsg( LOG_INFO, "FillNetSettingsJavaClass: FAIL GetNetSettingsJavaClass\n" );
		return false;
	}

	if( NULL == oNetSettingsClass )
	{
		LogMsg( LOG_ERROR, "FillNetSettingsJavaClass: FAIL because NULL NetSettingsJava object" );
		return false;
	}

	javaFillNetSettingsClass( poClass, netSettings, oNetSettingsClass );
	return true;
}

#endif //TARGET_OS_ANDROID

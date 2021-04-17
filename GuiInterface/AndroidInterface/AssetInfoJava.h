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

#include "JavaBaseClass.h"

class JavaAssetInfoClass : public JavaBaseClass
{
public:
	JavaAssetInfoClass( JavaEnvCache& envCache, jclass javaRefClass );
	virtual ~JavaAssetInfoClass();
	//=== vars ===//	
	jfieldID		m_AssetNameID;
	jfieldID		m_AssetTagID;
	
	jfieldID		m_UniqueIdID;
	jfieldID		m_CreatorIdID;
	jfieldID		m_HistoryIdID;

	jfieldID		m_AssetHashID;

	jfieldID		m_s64AssetLenID;
	jfieldID		m_u16AssetTypeID;
	jfieldID		m_LocationFlagsID;
	jfieldID		m_CreationTimeID;
	jfieldID		m_AssetSendStateID;

	jobject			CreatNewObj( void );
};

JavaAssetInfoClass * GetJavaAssetInfoClass( void );
void JavaAssetInfoClassThreadExit( unsigned int uiThreadId );
void JavaAssetInfoClassShutdown( void );

bool Java2AssetInfo( jobject& o,  AssetInfo * assetInfo );
bool AssetInfo2Java( AssetInfo * assetInfo, jobject& o );
bool AssetInfoFillJavaAssetInfoClass( AssetInfo * assetInfo, jobject oAssetInfoClass );

#endif //TARGET_OS_ANDROID

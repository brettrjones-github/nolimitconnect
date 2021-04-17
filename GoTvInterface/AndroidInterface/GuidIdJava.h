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
class VxGUID;

class JavaGuidIdClass : public JavaBaseClass
{
public:
	JavaGuidIdClass( JavaEnvCache& envCache, jclass javaRefClass );
	virtual ~JavaGuidIdClass();
	
	//=== vars ===//
	jfieldID		m_u64GuidIdLoPartID;
	jfieldID		m_u64GuidIdHiPartID;
};

JavaGuidIdClass * GetJavaGuidIdClass( void );
void JavaGuidIdClassThreadExit( unsigned int uiThreadId );
void JavaGuidIdClassShutdown( void );

bool JavaGuidId2VxGUID( jobject& o, VxGUID& guidId );
bool JavaGuidId2OnlineId( jobject& o, VxGUID& onlineId );
bool VxGUID2JavaGuidId( VxGUID& guidId, jobject& o, bool createNewJavaObj = true );
bool OnlineId2JavaGuidId( VxGUID& onlineId, jobject& o, bool createNewJavaObj = true );

#endif //TARGET_OS_ANDROID


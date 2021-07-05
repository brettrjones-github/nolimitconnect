//============================================================================
// Copyright (C) 2012 Brett R. Jones 
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

#include "SharedFileInfo.h"

#include <PktLib/VxSearchDefs.h>

#include <CoreLib/VxFileLists.h>
#include <CoreLib/VxFileIsTypeFunctions.h>
#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxDebug.h>

#include <sys/types.h>
#include <sys/stat.h>

//============================================================================
SharedFileInfo::SharedFileInfo()
: m_ContainedInDir("")
, m_s64FileLen(0)
, m_u32Attributes(0) 
, m_u8FileType(VXFILE_TYPE_UNKNOWN)
, m_IsDirty( true )
{ 
}

//============================================================================
SharedFileInfo::SharedFileInfo( const std::string& str )
: m_ContainedInDir("")
, m_s64FileLen(0)
, m_u32Attributes(0) 
, m_u8FileType(VXFILE_TYPE_UNKNOWN)
, m_IsDirty( true )
{ 
	*this = str;
	determineSharedDir();
}

//============================================================================
SharedFileInfo::SharedFileInfo( const std::string& str, uint64_t fileLen, uint8_t fileType )
: m_ContainedInDir("")
, m_s64FileLen(fileLen)
, m_u32Attributes(0) 
, m_u8FileType(fileType)
, m_IsDirty( true )
{ 
	*this = str;
	determineSharedDir();
}


//============================================================================
SharedFileInfo& SharedFileInfo::operator=( const SharedFileInfo& rhs ) 
{	
	m_FullFileName			= rhs.m_FullFileName; 
	m_ContainedInDir		= rhs.m_ContainedInDir;
	m_s64FileLen			= rhs.m_s64FileLen;
	m_u32Attributes			= rhs.m_u32Attributes;
	m_u8FileType			= rhs.m_u8FileType;
	m_FileHash				= rhs.m_FileHash;
	m_IsDirty				= rhs.m_IsDirty;

	return *this;
}

//============================================================================
SharedFileInfo& SharedFileInfo::operator=(const std::string& str) 
{ 
	m_FullFileName = ( str );
	determineSharedDir();
	return *this;
}

//============================================================================
SharedFileInfo& SharedFileInfo::operator=(const char * str) 
{ 
	m_FullFileName = ( str ); 
	determineSharedDir();
	return *this;
}

/*
//============================================================================
SharedFileInfo& SharedFileInfo::operator+=(const std::string& str) 
{ 
	std::string::operator +=(str); 
	return *this;
}

//============================================================================
SharedFileInfo& SharedFileInfo::operator+=(const char * str) 
{ 
	std::string::operator +=(str); 
	return *this;
}
*/

//============================================================================
void SharedFileInfo::determineSharedDir( void )
{
	if( 0 == m_ContainedInDir.length() )
	{
		std::string 	strJustFileName;
		RCODE rc = VxFileUtil::seperatePathAndFile(	getLocalFileName(),					
													m_ContainedInDir,		
													strJustFileName );	
		if( 0 != rc )
		{
			LogMsg( LOG_ERROR, "SharedFileInfo::determineSharedDir failed error %d file %s\n", rc, getLocalFileName().c_str() );
		}
	}
}

//============================================================================
bool SharedFileInfo::isDirectory( void )
{
	return ( VXFILE_TYPE_DIRECTORY & m_u8FileType ) ? true : false;
}

//============================================================================
std::string SharedFileInfo::getRemoteFileName( void )
{
	std::string		rmtFileName("");
	std::string 	filePath;
	RCODE rc = VxFileUtil::seperatePathAndFile(	getLocalFileName(),					
												filePath,		
												rmtFileName );	
	if( 0 != rc )
	{
		LogMsg( LOG_ERROR, "SharedFileInfo::getRemoteFileName failed error %d file %s\n", rc, getLocalFileName().c_str() );
	}

	return rmtFileName;
}

//============================================================================
void SharedFileInfo::updateFileInfo( VxThread * callingThread )
{
	return;
}

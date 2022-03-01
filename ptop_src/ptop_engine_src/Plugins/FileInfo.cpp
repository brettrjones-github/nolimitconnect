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

#include "FileInfo.h"

#include <PktLib/VxSearchDefs.h>

#include <CoreLib/VxFileLists.h>
#include <CoreLib/VxFileIsTypeFunctions.h>
#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxDebug.h>

#include <sys/types.h>
#include <sys/stat.h>

//============================================================================
FileInfo::FileInfo()
{ 
	generateAssetId();
}

//============================================================================
FileInfo::FileInfo( const std::string& str )
{ 
	*this = str;
	determineSharedDir();
	generateAssetId();
}

//============================================================================
FileInfo::FileInfo( const std::string& str, uint64_t fileLen, uint8_t fileType )
: m_s64FileLen(fileLen) 
, m_u32Attributes(0) 
, m_u8FileType(fileType)
, m_ContainedInDir("")
, m_IsDirty( true )
{ 
	*this = str;
	determineSharedDir();
	generateAssetId();
}


//============================================================================
FileInfo::FileInfo( const std::string& str, uint64_t fileLen, uint8_t fileType, VxGUID& assetId )
	: m_s64FileLen( fileLen )
	, m_u32Attributes( 0 )
	, m_u8FileType( fileType )
	, m_ContainedInDir( "" )
	, m_IsDirty( true )
	, m_AssetId( assetId )
{
	*this = str;
	determineSharedDir();
	generateAssetId();
}

//============================================================================
FileInfo& FileInfo::operator=( const FileInfo& rhs ) 
{	
	std::string::operator	=( rhs ); 
	m_u32Attributes			= rhs.m_u32Attributes;
	m_s64FileLen			= rhs.m_s64FileLen;
	m_u8FileType			= rhs.m_u8FileType;
	m_ContainedInDir		= rhs.m_ContainedInDir;
	m_AssetId				= rhs.m_AssetId;
	return *this;
}

//============================================================================
FileInfo& FileInfo::operator=(const std::string& str) 
{ 
	std::string::operator =(str);
	determineSharedDir();
	return *this;
}

//============================================================================
FileInfo& FileInfo::operator=(const char * str) 
{ 
	std::string::operator =(str); 
	determineSharedDir();
	return *this;
}

//============================================================================
FileInfo& FileInfo::operator+=(const std::string& str) 
{ 
	std::string::operator +=(str); 
	return *this;
}

//============================================================================
FileInfo& FileInfo::operator+=(const char * str) 
{ 
	std::string::operator +=(str); 
	return *this;
}

//============================================================================
void FileInfo::determineSharedDir( void )
{
	if( 0 == m_ContainedInDir.length() )
	{
		std::string 	strJustFileName;
		RCODE rc = VxFileUtil::seperatePathAndFile(	getLocalFileName(),					
			m_ContainedInDir,		
			strJustFileName );	
		if( 0 != rc )
		{
			LogMsg( LOG_ERROR, "FileInfo::determineSharedDir failed error %d file %s\n", rc, getLocalFileName().c_str() );
		}
	}
}

//============================================================================
bool FileInfo::isDirectory( void )
{
	return ( VXFILE_TYPE_DIRECTORY & m_u8FileType ) ? true : false;
}

//============================================================================
std::string FileInfo::getRemoteFileName( void )
{
	std::string rmtFileName("");
	if( m_ContainedInDir.length() 
		&& ( getLocalFileName().length() > m_ContainedInDir.length() ) )
	{
		const char * lclFileName = getLocalFileName().c_str();
		rmtFileName = &lclFileName[ m_ContainedInDir.length() ];
	}
	else
	{
		std::string 	filePath;
		RCODE rc = VxFileUtil::seperatePathAndFile(	getLocalFileName(),					
													filePath,		
													rmtFileName );	
		if( 0 != rc )
		{
			LogMsg( LOG_ERROR, "FileInfo::getRemoteFileName failed error %d file %s\n", rc, getLocalFileName().c_str() );
		}
	}

	return rmtFileName;
}

//============================================================================
void FileInfo::updateFileInfo( VxThread * callingThread )
{
	return;
}

//============================================================================
void FileInfo::generateAssetId( void )
{
	if( !m_AssetId.isVxGUIDValid() )
	{
		m_AssetId.initializeWithNewVxGUID();
	}
}
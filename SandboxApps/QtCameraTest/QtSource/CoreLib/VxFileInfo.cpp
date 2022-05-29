//============================================================================
// Copyright (C) 2015 Brett R. Jones
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
// http://www.nolimitconnect.org
//============================================================================

#include "config_corelib.h"

#include "VxFileInfo.h"
#include "VxFileIsTypeFunctions.h"
#include "VxFileUtil.h"

//============================================================================
VxFileInfo::VxFileInfo()
: m_s64FileLen(0)
, m_u8FileType( VXFILE_TYPE_UNKNOWN )
, m_IsInLibrary( false )
, m_IsShared( false )
, m_FileName( "" )
{ 
}

//============================================================================
VxFileInfo::VxFileInfo( const char * fileName )
: m_s64FileLen(0)
, m_u8FileType( VXFILE_TYPE_UNKNOWN )
, m_IsInLibrary( false )
, m_IsShared( false )
, m_FileName(fileName)
{ 
}

//============================================================================
VxFileInfo::VxFileInfo(const VxFileInfo& other)
{
	*this = other;
}

//============================================================================
bool VxFileInfo::isDirectory( void )	
{ 
	return (VXFILE_TYPE_DIRECTORY & m_u8FileType)?true:false; 
}

//============================================================================
bool VxFileInfo::isExecutableFile( void )	
{ 
	return (VXFILE_TYPE_EXECUTABLE & m_u8FileType)?true:false; 
}

//============================================================================
bool VxFileInfo::isShortcutFile( void )		
{ 
	return VxIsShortcutFile( getFileName() ); 
}

//============================================================================
void VxFileInfo::assureTrailingDirectorySlash( void )
{
	if( isDirectory() )
	{
		VxFileUtil::assureTrailingDirectorySlash( getFileName() );
	}
}

//============================================================================
const char * VxFileInfo::describeFileType( uint8_t fileType )
{
	switch( fileType )
	{
	case VXFILE_TYPE_PHOTO:
		return "Photo: ";
	case VXFILE_TYPE_AUDIO:
		return "Audio: ";
	case VXFILE_TYPE_VIDEO:
		return "Video: ";
	case VXFILE_TYPE_DOC:
		return "Documents: ";
	case VXFILE_TYPE_ARCHIVE_OR_CDIMAGE:
		return "Archive Or ISO: ";
	case VXFILE_TYPE_EXECUTABLE:
		return "Executable: ";
	case VXFILE_TYPE_DIRECTORY:
		return "Folder: ";
	default:
		return "Other: ";
	}
}

//============================================================================
VxFileInfo& VxFileInfo::operator=(const VxFileInfo& rhs) 
{	
	if( this != &rhs )
	{
		m_s64FileLen		= rhs.m_s64FileLen;
		m_u8FileType		= rhs.m_u8FileType;
		m_FileHashId		= rhs.m_FileHashId;
		m_IsInLibrary		= rhs.m_IsInLibrary;
		m_IsShared			= rhs.m_IsShared;
		m_FileName			= rhs.m_FileName;
	}

	return *this;
}

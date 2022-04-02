//============================================================================
// Copyright (C) 2013 Brett R. Jones
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

#include "FileItemInfo.h"
#include "GuiHelpers.h"

#include <PktLib/VxSearchDefs.h>
#include <PktLib/VxCommon.h>
#include <CoreLib/VxParse.h>
#include <CoreLib/VxFileIsTypeFunctions.h>

//============================================================================
FileItemInfo::FileItemInfo( uint8_t u8FileType, const char * pFileName, uint64_t u64FileLen, VxGUID assetId, VxNetIdent * netIdent, uint8_t * fileHashId )
: m_FileInfo( pFileName, u8FileType, u64FileLen, assetId, fileHashId )
, m_Widget( 0 )
, m_IsShared( false )
, m_IsInLibrary( false )
{
}

//============================================================================
FileItemInfo::FileItemInfo( VxMyFileInfo& fileInfo, VxNetIdent * netIdent, bool isShared, bool isInLibrary )
: m_FileInfo( fileInfo )
, m_Widget( 0 )
, m_IsShared( isShared )
, m_IsInLibrary( isInLibrary )
{
}

//============================================================================
QString FileItemInfo::describeFileType()
{
	switch( getFileType() )
	{
	case VXFILE_TYPE_PHOTO:
		return QObject::tr("Photo: ");
	case VXFILE_TYPE_AUDIO:
		return QObject::tr("Audio: ");
	case VXFILE_TYPE_VIDEO:
		return QObject::tr("Video: ");
	case VXFILE_TYPE_DOC:
		return QObject::tr("Documents: ");
	case VXFILE_TYPE_ARCHIVE_OR_CDIMAGE:
		return QObject::tr("Archive Or ISO: ");
	case VXFILE_TYPE_EXECUTABLE:
		return QObject::tr("Executable: ");
	case VXFILE_TYPE_DIRECTORY:
		return QObject::tr("Folder: ");
	default:
		return QObject::tr("Other: ");
	}
}

//============================================================================
QString FileItemInfo::describeFileLength()
{
    std::string strLen;
	uint64_t fileLen = getFileLength();
    if( fileLen >= 1000000000000ULL )
    {
        StdStringFormat( strLen, "%3.1f TB ", (double)(fileLen) / 1000000000000.0);
    }
    else if( fileLen >= 1000000000ULL )
    {
        StdStringFormat( strLen, "%3.1f GB ", (double)(fileLen) / 1000000000.0);
    }
    else if( fileLen >= 1000000 )
    {
        StdStringFormat( strLen, "%3.1f MB ", (double)(fileLen) / 1000000.0);
    }
    else if( fileLen >= 1000 )
    {
        StdStringFormat( strLen, "%3.1f KB ", (double)(fileLen) / 1000.0);
    }
    else
    {
        StdStringFormat( strLen, "%3.1f Bytes ", (double)fileLen );
    }

    QString strFormatedLen = strLen.c_str();
    return strFormatedLen;
}

//============================================================================
bool FileItemInfo::toggleIsShared( void )
{
	m_IsShared = !m_IsShared;
	return m_IsShared;
}

//============================================================================
bool FileItemInfo::toggleIsInLibrary( void )
{
	m_IsInLibrary = !m_IsInLibrary;
	return m_IsInLibrary;
}

//============================================================================
void FileItemInfo::updateWidget( void )
{
	if( 0 != m_Widget )
	{
		m_Widget->update();
	}
}

//============================================================================
bool FileItemInfo::shouldOpenFile( void )
{
	return VxShouldOpenFile( getFileType() );
}


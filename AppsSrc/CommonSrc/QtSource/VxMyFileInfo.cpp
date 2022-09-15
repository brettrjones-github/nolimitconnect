//============================================================================
// Copyright (C) 2014 Brett R. Jones
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

#include "VxMyFileInfo.h"

#include "AppCommon.h"
#include "GuiHelpers.h"
#include "GuiPlayerMgr.h"

#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxParse.h>
#include <CoreLib/VxFileIsTypeFunctions.h>

#include <QWidget>
#include <QFileInfo>
#include <QDesktopServices>
#include <QUrl>

//============================================================================
VxMyFileInfo::VxMyFileInfo(QWidget *parent)
: QObject(parent)
{
    setObjectName( "VxMyFileInfo" );
}

//============================================================================
VxMyFileInfo::VxMyFileInfo( const VxFileInfo &rhs )
: QObject()
{
	*this = rhs;
}

//============================================================================
VxMyFileInfo::VxMyFileInfo( QString fileName, uint8_t fileType, uint64_t fileLen, VxGUID& assetId, VxSha1Hash& fileHashId )
: QObject()
, m_FullFileName( fileName )
, m_FileType( fileType )
, m_FileLen( fileLen )
, m_FileHashId( fileHashId )
, m_IsInLibrary( false )
, m_IsShared( false )
, m_AssetId( assetId )
{
	updateJustFileName();
	generateAssetId();
}

//============================================================================
VxMyFileInfo::VxMyFileInfo( QString fileName, uint8_t fileType, uint64_t fileLen, VxGUID& assetId, uint8_t * fileHashData )
: QObject()
, m_FullFileName( fileName )
, m_FileType( fileType )
, m_FileLen( fileLen )
, m_FileHashId( fileHashData )
, m_IsInLibrary( false )
, m_IsShared( false )
, m_AssetId( assetId )
{
	updateJustFileName();
	generateAssetId();
}

//============================================================================
VxMyFileInfo::VxMyFileInfo( const VxMyFileInfo& rhs )
: QObject()
{
	*this = rhs;
}

//============================================================================
VxMyFileInfo::~VxMyFileInfo()
{
}

//============================================================================
void VxMyFileInfo::setFullFileName( QString fileName )
{
	m_FullFileName		= fileName;
	updateJustFileName();
}

//============================================================================
void VxMyFileInfo::updateJustFileName( void )
{
	if( m_FullFileName.isEmpty() )
	{
		m_JustFileName = "";
		m_FilePath = "";
		return;
	}

	GuiHelpers::splitPathAndFileName( m_FullFileName, m_JustFileName, m_FilePath );
}

//============================================================================
VxMyFileInfo& VxMyFileInfo::operator =( const VxFileInfo& rhs )
{
	m_FullFileName		= rhs.getFileNameConstString();
	m_FileType			= rhs.getFileType();
	m_FileLen			= rhs.getFileLength();
	m_IsInLibrary		= rhs.getIsInLibrary();
	m_IsShared			= rhs.getIsShared();
	m_FileHashId		= ((VxFileInfo&)rhs).getFileHashId();
	updateJustFileName();
	generateAssetId();
	return *this;
}

//============================================================================
VxMyFileInfo& VxMyFileInfo::operator =( const VxMyFileInfo &rhs )
{
	if( this != &rhs )   
	{
		m_FullFileName		= rhs.m_FullFileName;
		m_FileType			= rhs.m_FileType;
		m_FileLen			= rhs.m_FileLen;
		m_FileHashId		= rhs.m_FileHashId;
		m_IsInLibrary		= rhs.m_IsInLibrary;
		m_IsShared			= rhs.m_IsShared;
		m_AssetId			= rhs.m_AssetId;
		updateJustFileName();
	}

	return *this;
}

//============================================================================
bool VxMyFileInfo::operator == ( const VxMyFileInfo &rhs ) const
{
	return ( m_FullFileName == rhs.m_FullFileName );
}

//============================================================================
bool VxMyFileInfo::operator != ( const VxMyFileInfo &a ) const
{
	return ( m_FullFileName != a.m_FullFileName );
}

//============================================================================
bool VxMyFileInfo::operator < ( const VxMyFileInfo &b ) const
{
	return ( m_FullFileName < b.m_FullFileName );
}

//============================================================================
bool VxMyFileInfo::operator <= ( const VxMyFileInfo &b ) const
{
	return ( m_FullFileName <= b.m_FullFileName );
}

//============================================================================
bool VxMyFileInfo::operator > ( const VxMyFileInfo &b ) const
{
	return ( m_FullFileName > b.m_FullFileName );
}

//============================================================================
bool VxMyFileInfo::operator >= ( const VxMyFileInfo &b ) const
{
	return ( m_FullFileName >= b.m_FullFileName );
}

//============================================================================
QString VxMyFileInfo::describeFileLength()
{
	std::string strLen;
	if( m_FileLen >= 1000000000000ULL )
	{
		StdStringFormat( strLen, "%3.1f TB", (double)(m_FileLen) / 1000000000000.0);
	}
	else if( m_FileLen >= 1000000000ULL )
	{
		StdStringFormat( strLen, "%3.1f GB", (double)(m_FileLen) / 1000000000.0);
	}
	else if( m_FileLen >= 1000000 )
	{
		StdStringFormat( strLen, "%3.1f MB", (double)(m_FileLen) / 1000000.0);
	}
	else if( m_FileLen >= 1000 )
	{
		StdStringFormat( strLen, "%3.1f KB", (double)(m_FileLen) / 1000.0);
	}
	else
	{
		StdStringFormat( strLen, "%3.1f Bytes", (double)m_FileLen );
	}

	QString strFormatedLen = strLen.c_str();
	return strFormatedLen;
}

//============================================================================
QString VxMyFileInfo::describeFileType( void )
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
void VxMyFileInfo::playFile( AppCommon& myApp )
{
    GetAppInstance().getPlayerMgr().playFile( getFullFileName(), 0 );
}

//============================================================================
void VxMyFileInfo::generateAssetId( bool ifNotValid )
{
	if( !ifNotValid || (ifNotValid && !m_AssetId.isVxGUIDValid()) )
	{
		m_AssetId.initializeWithNewVxGUID();
	}
}
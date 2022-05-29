#pragma once
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

#include "VxSha1Hash.h"
#include "VxFileTypeMasks.h"

#include <string>

class VxFileInfo
{
public:
	VxFileInfo();
	VxFileInfo( const char * fileName );
	VxFileInfo(const VxFileInfo& other);
	VxFileInfo& operator=( const VxFileInfo& rhs ); 

	void						setFileName( const char * fileName )	{ m_FileName = fileName; }
	void						setFileName( std::string fileName )		{ m_FileName = fileName; }
	std::string&				getFileName( void )						{ return m_FileName; }
	const char *				getFileNameConstString( void ) const	{ return m_FileName.c_str(); }
	void						setFileType( uint8_t	fileType )		{ m_u8FileType = fileType; }
	uint8_t						getFileType( void ) const				{ return m_u8FileType; }
	void						setFileLength( int64_t fileLen )		{ m_s64FileLen = fileLen; }
	int64_t						getFileLength( void ) const				{ return m_s64FileLen; }
	VxSha1Hash&					getFileHashId( void )					{ return m_FileHashId; }
	void						setFileHashId( VxSha1Hash& id )			{ m_FileHashId = id; }
	void						setFileHashData( uint8_t * hashData )	{ m_FileHashId.setHashData( hashData ); }

	void						setIsInLibrary( bool inLibaray )		{ m_IsInLibrary = inLibaray; }
	bool						getIsInLibrary( void ) const			{ return m_IsInLibrary; }
	void						setIsShared( bool isShared )			{ m_IsShared = isShared; }
	bool						getIsShared( void ) const				{ return m_IsShared; }

	bool						isExecutableFile( void );
	bool						isShortcutFile( void );
	bool						isDirectory( void );
	void						assureTrailingDirectorySlash( void );
	static const char *			describeFileType( uint8_t fileType );




protected:
	//=== vars ===//
	int64_t						m_s64FileLen;
	uint8_t						m_u8FileType;
	VxSha1Hash					m_FileHashId;
	bool						m_IsInLibrary;
	bool						m_IsShared;
	std::string					m_FileName;
};
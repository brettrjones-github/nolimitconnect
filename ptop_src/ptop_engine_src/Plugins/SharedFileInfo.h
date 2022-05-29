#pragma once
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
// http://www.nolimitconnect.org
//============================================================================

#include <CoreLib/VxDefs.h>
#include <CoreLib/VxSha1Hash.h>
#include <CoreLib/VxGUID.h>

#include <string>

class VxThread;

class SharedFileInfo
{
public:
	SharedFileInfo();
	SharedFileInfo( const std::string& str );
	SharedFileInfo( const std::string& str, uint64_t fileLen, uint8_t fileType );
	
	SharedFileInfo&				operator=( const SharedFileInfo& oInfo ); 
	SharedFileInfo&				operator=( const std::string& str ); 
	SharedFileInfo&				operator=( const char * str ); 
	//SharedFileInfo&				operator+=( const std::string& str ); 
	//SharedFileInfo&				operator+=( const char * str ); 

	bool						isDirectory( void );
	void						setIsDirty( bool isDirty )				{ m_IsDirty = isDirty; }
	bool						getIsDirty( void )						{ return m_IsDirty; }
	std::string&				getLocalFileName( void )				{ return m_FullFileName; }
	std::string					getRemoteFileName( void );

	void						setFileName( const char * fileName )	{ m_FullFileName = ( 0 == fileName ) ? "" : fileName; }
	void						setFileName( std::string& fileName )	{ m_FullFileName = fileName; }
	std::string&				getFileName( void )						{ return m_FullFileName; }

	void						setFileType( uint8_t fileType )			{ m_u8FileType = fileType; }
	uint8_t						getFileType( void )						{ return m_u8FileType; }
	void						setFileLength( int64_t fileLength )		{ m_s64FileLen = fileLength; }
	int64_t						getFileLength( void )					{ return m_s64FileLen; }

	void						setFileHashId( VxSha1Hash& id )			{ m_FileHash = id; }
	void						setFileHashData( uint8_t * id )			{ m_FileHash.setHashData( id ); }
	VxSha1Hash&					getFileHashId( void )					{ return m_FileHash; }

	VxGUID&						getAssetId( void )						{ return m_AssetId; }

	void						updateFileInfo( VxThread * callingThread );

private:
	void						determineSharedDir( void );
	void						generateAssetId( void );

public:
	//=== vars ===//
	std::string					m_FullFileName;
	std::string					m_ContainedInDir;
	int64_t						m_s64FileLen;
	uint32_t					m_u32Attributes;
	uint8_t						m_u8FileType;
	VxSha1Hash					m_FileHash;
	bool						m_IsDirty;
	VxGUID						m_AssetId;
};
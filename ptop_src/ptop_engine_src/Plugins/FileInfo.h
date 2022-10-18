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

class AssetBaseInfo;
class OfferBaseInfo;
class VxFileXferInfo;

class FileInfo 
{
public:
	static const int FILE_INFO_SHORTEST_SEARCH_TEXT_LEN = 3;
	static const int FILE_INFO_LONGEST_SEARCH_TEXT_LEN = 512;

	FileInfo();
    FileInfo( const FileInfo& rhs );
	FileInfo( VxGUID& onlineId, const std::string& fullFileName );
	FileInfo( VxGUID& onlineId, const std::string& fullFileName, uint64_t fileLen, uint8_t fileType );
	FileInfo( VxGUID& onlineId, const std::string& fullFileName, uint64_t fileLen, uint8_t fileType, VxGUID& assetId );
	FileInfo( VxGUID& onlineId, const std::string& fullFileName, uint64_t fileLen, uint8_t fileType, VxGUID& assetId, VxSha1Hash& sha1Hash );
	FileInfo( AssetBaseInfo& assetInfo );
	FileInfo( AssetBaseInfo& assetInfo, VxSha1Hash& sha1Hash );
	FileInfo( OfferBaseInfo& offerInfo );
	FileInfo( VxFileXferInfo& xferInfo, VxGUID& onlineId );

	FileInfo&					operator=( const FileInfo& rhs );

	bool						isValid( bool includeHashValid = true );

	bool						isDirectory( void );

	void						setIsInLibrary( bool inLibrary )		{ m_IsInLibrary = inLibrary; }
	bool						getIsInLibrary( void )					{ return m_IsInLibrary; }

	void						setIsSharedFile( bool isSharedFile )	{ m_IsSharedFile = isSharedFile; }
	bool						getIsSharedFile( void )				    { return m_IsSharedFile; }

	void						setFullFileName( std::string fileName );
	std::string&				getFullFileName( void )					{ return m_FullFileName; }
	std::string&				getJustFileName( void )					{ return m_ShortFileName; }
	std::string&				getShortFileName( void )				{ return m_ShortFileName; }

	std::string&				getLocalFileName( void )				{ return m_FullFileName; }
	std::string					getRemoteFileName( void )				{ return m_ShortFileName; }

	std::string&				getFilePath( void );

	void						setXferSessionId( VxGUID& sessionId )	{ m_XferSessionId = sessionId; }
	VxGUID&						getXferSessionId( void )				{ return m_XferSessionId; }
	VxGUID&						initializeNewXferSessionId( void ); // init and return unique m_XferSessionId

	void						setOnlineId( VxGUID& assetId )			{ m_OnlineId = assetId; }
	VxGUID&						getOnlineId( void )						{ return m_OnlineId; }

	void						setAssetId( VxGUID& assetId )			{ m_AssetId = assetId; }
	VxGUID&						getAssetId( void )						{ return m_AssetId; }

	void						setThumbId( VxGUID& assetId )			{ m_ThumbId = assetId; }
	VxGUID&						getThumbId( void )						{ return m_ThumbId; }

	void						setFileName( const char* fileName )	    { m_FullFileName = fileName; }
	void						setFileName( std::string& fileName )	{ m_FullFileName = fileName; }
	std::string&				getFileName( void )						{ return m_FullFileName; }

	void						setFileType( uint8_t fileType )			{ m_u8FileType = fileType; }
	uint8_t						getFileType( void )	const				{ return m_u8FileType; }
	void						setFileLength( int64_t fileLength )		{ m_s64FileLen = fileLength; }
	int64_t						getFileLength( void ) const				{ return m_s64FileLen; }

	void						setFileTime( int64_t fileTime )			{ m_FileTime = fileTime; }
	int64_t						getFileTime( void )	const				{ return m_FileTime; }

	void						setFileHashId( VxSha1Hash& id )			{ m_FileHash = id; }
	void						setFileHashId( uint8_t * id )			{ m_FileHash.setHashData( id ); }
	VxSha1Hash&					getFileHashId( void )					{ return m_FileHash; }

	bool						determineShortName( std::string containingDir = "" );
	bool						determineFullFileName( std::string containingDir = "" );
	bool						determineFilePath( void );

	bool						matchTextAndType( std::string& searchStr, uint8_t fileType );
	bool						matchText( std::string& searchStr );

	int							calcBlobLen( void );
	bool						addToBlob( PktBlobEntry& blob );
	bool						extractFromBlob( PktBlobEntry& blob );

	void						assureValidAssetId( void );

private:
	void						generateAssetId( void );

public:
	//=== vars ===//
	VxGUID						m_OnlineId;
	std::string					m_FullFileName{ ""};
	std::string					m_ShortFileName{ "" };
	int64_t						m_s64FileLen{ 0 };
	uint32_t					m_u32Attributes{ 0 };
	uint8_t						m_u8FileType{ 0 };
	VxSha1Hash					m_FileHash;
	std::string					m_ContainedInDir{ "" };
	VxGUID						m_AssetId;
	VxGUID						m_ThumbId;
	int64_t						m_FileTime{ 0 };

	VxGUID						m_XferSessionId; // temporary.. not in db and does not get added or extracted from blob
	bool						m_IsInLibrary{ true }; // temporary.. not in db does not get added or extracted from blob
	bool						m_IsSharedFile{ true }; // temporary.. not in db does not get added or extracted from blob
};

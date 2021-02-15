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
// http://www.nolimitconnect.com
//============================================================================

#include <config_gotvcore.h>

#include <CoreLib/BlobDefs.h>
#include <CoreLib/VxSha1Hash.h>
#include <CoreLib/VxGUID.h>

#include <string>

#define ASSET_LOC_FLAG_PERSONAL_RECORD			0x0001
#define ASSET_LOC_FLAG_LIBRARY					0x0002
#define ASSET_LOC_FLAG_SHARED_FILE				0x0004

class VxThread;

class BlobInfo 
{
public:
	BlobInfo();
	BlobInfo( const BlobInfo& rhs );
	BlobInfo( const std::string& fileName );
    BlobInfo( const char * fileName, uint64_t fileLen, uint16_t assetType );

	BlobInfo&				    operator=( const BlobInfo& rhs ); 

	bool						isValid( void );
	bool						isMine( void );

	bool						isDirectory( void );
	bool						isUnknownBlob( void )						    { return ( 0 == m_u16BlobType) ? true : false; }
	bool						isChatTextBlob( void )						    { return ( eBlobTypeChatText & m_u16BlobType) ? true : false; }
	bool						isChatFaceBlob( void )						    { return ( ( eBlobTypeChatFace ) & m_u16BlobType ) ? true : false; }
	bool						isChatAvatarBlob( void )					    { return ( ( eBlobTypeChatStockAvatar | eBlobTypeChatCustomAvatar ) & m_u16BlobType ) ? true : false; }
	bool						isPhotoBlob( void )						        { return ( eBlobTypePhoto & m_u16BlobType ) ? true : false; }
    bool						isThumbBlob( void )						        { return ( eBlobTypeThumbnail & m_u16BlobType ) ? true : false; }
	bool						isAudioBlob( void )						        { return ( eBlobTypeAudio & m_u16BlobType ) ? true : false; }
	bool						isVideoBlob( void )						        { return ( eBlobTypeVideo & m_u16BlobType ) ? true : false; }
	bool						getIsFileBlob( void );
	bool						hasFileName( void );

	void						setIsPersonalRecord( bool isRecord )		    { if(isRecord) m_LocationFlags|=ASSET_LOC_FLAG_PERSONAL_RECORD;else m_LocationFlags&=~ASSET_LOC_FLAG_PERSONAL_RECORD; }
	bool						getIsPersonalRecord( void )					    { return m_LocationFlags&ASSET_LOC_FLAG_PERSONAL_RECORD?true:false; }
	void						setIsInLibary( bool isInLibrary )			    { if(isInLibrary) m_LocationFlags|=ASSET_LOC_FLAG_LIBRARY;else m_LocationFlags&=~ASSET_LOC_FLAG_LIBRARY; }
	bool						getIsInLibary( void )						    { return m_LocationFlags&ASSET_LOC_FLAG_LIBRARY?true:false; }
	void						setIsSharedFileBlob( bool isSharedBlob )	    { if(isSharedBlob) m_LocationFlags|=ASSET_LOC_FLAG_SHARED_FILE;else m_LocationFlags&=~ASSET_LOC_FLAG_SHARED_FILE; }
	bool						getIsSharedFileBlob( void )				        { return m_LocationFlags&ASSET_LOC_FLAG_SHARED_FILE?true:false; }

	void						setBlobName( const char * assetName );
	void						setBlobName( std::string& assetName )		    { m_BlobName = assetName; }
	std::string&				getBlobName( void )						        { return m_BlobName; }
	std::string					getRemoteBlobName( void );

	void						setBlobTag( const char * assetTag );
	std::string&				getBlobTag( void )							    { return m_BlobTag; }

	void						setBlobType( EBlobType assetType )		        { m_u16BlobType = (uint16_t)assetType; }
	EBlobType				    getBlobType( void )						        { return (EBlobType)m_u16BlobType; }

	void						setBlobLength( int64_t assetLength )		    { m_s64BlobLen = assetLength; }
	int64_t						getBlobLength( void )						    { return m_s64BlobLen; }

	void						setBlobHashId( VxSha1Hash& id )			        { m_BlobHash = id; }
	void						setBlobHashId( uint8_t * id )				    { m_BlobHash.setHashData( id ); }
	VxSha1Hash&					getBlobHashId( void )						    { return m_BlobHash; }

	void						setBlobUniqueId( VxGUID& uniqueId )		        { m_UniqueId = uniqueId; }
	void						setBlobUniqueId( const char * guid )		    { m_UniqueId.fromVxGUIDHexString( guid ); }
	VxGUID&						getBlobUniqueId( void )					        { return m_UniqueId; }
	VxGUID&						generateNewUniqueId( void ); // generates unique id, assigns it to asset and returns reference to it

	void						setCreatorId( VxGUID& creatorId )			    { m_CreatorId = creatorId; }
	void						setCreatorId( const char * creatorId )		    { m_CreatorId.fromVxGUIDHexString( creatorId ); }
	VxGUID&						getCreatorId( void )						    { return m_CreatorId; }

	void						setHistoryId( VxGUID& historyId )			    { m_HistoryId = historyId; }
	void						setHistoryId( const char * historyId )		    { m_HistoryId.fromVxGUIDHexString( historyId ); }
	VxGUID&						getHistoryId( void )						    { return m_HistoryId; }

	void						setLocationFlags( uint32_t locFlags )		    { m_LocationFlags = locFlags; }
	uint32_t					getLocationFlags( void )					    { return m_LocationFlags; }

	void						setBlobSendState( EBlobSendState sendState )	{ m_BlobSendState = sendState; }
	EBlobSendState			    getBlobSendState( void )					    { return m_BlobSendState; }
	
	void						setCreationTime( time_t timestamp )			    { m_CreationTime = timestamp; }
	time_t						getCreationTime( void )						    { return m_CreationTime; }

    void						setPlayPosition( int pos0to100000 )             { m_PlayPosition0to100000 = pos0to100000; }
    int						    getPlayPosition( void )                         { return m_PlayPosition0to100000; }

	static const char *			getDefaultFileExtension( EBlobType assetType );
	static const char *			getSubDirectoryName( EBlobType assetType );
	void						updateBlobInfo( VxThread * callingThread );
	bool						needsHashGenerated( void );

private:
	void						determineBlobDir( void );

public:
	//=== vars ===//
	std::string					m_BlobName; // usually file name
	std::string					m_BlobTag;
	VxGUID						m_UniqueId;
	VxGUID						m_CreatorId;
	VxGUID						m_HistoryId; 
	VxSha1Hash					m_BlobHash;
	int64_t						m_s64BlobLen;
	uint16_t					m_u16BlobType;
	uint32_t					m_LocationFlags;
	EBlobSendState			    m_BlobSendState;
	time_t						m_CreationTime;
    int						    m_PlayPosition0to100000;
};

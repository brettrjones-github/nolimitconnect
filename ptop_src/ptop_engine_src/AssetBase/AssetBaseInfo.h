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

#include <ptop_src/ptop_engine_src/BaseInfo/BaseInfo.h>

#include <CoreLib/AssetDefs.h>
#include <CoreLib/VxFileTypeMasks.h>
#include <CoreLib/VxSha1Hash.h>
#include <CoreLib/VxGUID.h>

#include <string>

#define ASSET_LOC_FLAG_PERSONAL_RECORD			0x0001
#define ASSET_LOC_FLAG_LIBRARY					0x0002
#define ASSET_LOC_FLAG_SHARED_FILE				0x0004
#define ASSET_LOC_FLAG_CHAT_ROOM				0x0008

#define ASSET_ATTRIB_FLAG_CIRCULAR			    0x0001
#define ASSET_ATTRIB_TEMPORARY			        0x0002
#define ASSET_ATTRIB_DELETED			        0x0004

class VxThread;

class AssetBaseInfo : public BaseInfo
{
public:
    AssetBaseInfo();
    AssetBaseInfo( const AssetBaseInfo& rhs );
    AssetBaseInfo( enum EAssetType assetType );
    AssetBaseInfo( enum EAssetType assetType, VxGUID& onlineId, int64_t modifiedTime = 0 );
    AssetBaseInfo( enum EAssetType assetType, VxGUID& onlineId, VxGUID& assetId, int64_t modifiedTime = 0 );
    AssetBaseInfo( enum EAssetType assetType, const std::string& fileName );
    AssetBaseInfo( enum EAssetType assetType, const std::string& fileName, VxGUID& assetId );
    AssetBaseInfo( enum EAssetType assetType, const char* fileName, uint64_t fileLen );
    AssetBaseInfo( enum EAssetType assetType, const char* fileName, uint64_t fileLen, VxGUID& assetId );
    AssetBaseInfo( enum EAssetType assetType, VxGUID& creatorId, VxGUID& assetId );
    virtual ~AssetBaseInfo() = default;

    AssetBaseInfo&				operator=( const AssetBaseInfo& rhs );

    virtual bool				isValid( void );
    virtual bool				isValidFile( void );
    virtual bool				isValidThumbnail( void );
    virtual bool				isMine( void );
    virtual bool                validateAssetExist( void );

    virtual bool				isDirectory( void );
    virtual bool				isUnknownAsset( void )                          { return ( 0 == m_u16AssetType ) ? true : false; }
    virtual bool				isChatTextAsset( void )                         { return ( eAssetTypeChatText & m_u16AssetType ) ? true : false; }
    virtual bool				isChatFaceAsset( void )                         { return ( eAssetTypeChatFace & m_u16AssetType ) ? true : false; }
    virtual bool				isPhotoAsset( void )                            { return ( eAssetTypePhoto & m_u16AssetType ) ? true : false; }
    virtual bool				isThumbAsset( void )                            { return ( eAssetTypeThumbnail & m_u16AssetType ) ? true : false; }
    virtual bool				isAudioAsset( void )                            { return ( eAssetTypeAudio & m_u16AssetType ) ? true : false; }
    virtual bool				isVideoAsset( void )                            { return ( eAssetTypeVideo & m_u16AssetType ) ? true : false; }
    virtual bool				isFileAsset( void );
    virtual bool				hasFileName( void );

    void						setIsChatRoomRecord( bool isSharedAsset )	    { if( isSharedAsset ) m_LocationFlags |= ASSET_LOC_FLAG_CHAT_ROOM; else m_LocationFlags &= ~ASSET_LOC_FLAG_CHAT_ROOM; }
    bool						isChatRoomRecord( void )				        { return m_LocationFlags & ASSET_LOC_FLAG_CHAT_ROOM ? true : false; }
    virtual void				setIsPersonalRecord( bool isRecord )            { if( isRecord ) m_LocationFlags |= ASSET_LOC_FLAG_PERSONAL_RECORD; else m_LocationFlags &= ~ASSET_LOC_FLAG_PERSONAL_RECORD; }
    virtual bool				isPersonalRecord( void )                        { return m_LocationFlags & ASSET_LOC_FLAG_PERSONAL_RECORD ? true : false; }
    virtual void				setIsInLibary( bool isInLibrary )               { if( isInLibrary ) m_LocationFlags |= ASSET_LOC_FLAG_LIBRARY; else m_LocationFlags &= ~ASSET_LOC_FLAG_LIBRARY; }
    virtual bool				isInLibary( void )                              { return m_LocationFlags & ASSET_LOC_FLAG_LIBRARY ? true : false; }
    virtual void				setIsSharedFileAsset( bool isSharedAsset )      { if( isSharedAsset ) m_LocationFlags |= ASSET_LOC_FLAG_SHARED_FILE; else m_LocationFlags &= ~ASSET_LOC_FLAG_SHARED_FILE; }
    virtual bool				isSharedFileAsset( void )                       { return m_LocationFlags & ASSET_LOC_FLAG_SHARED_FILE ? true : false; }

    virtual void				setIsCircular( bool isCircular )                { if( isCircular ) m_AttributeFlags |= ASSET_ATTRIB_FLAG_CIRCULAR; else m_AttributeFlags &= ~ASSET_ATTRIB_FLAG_CIRCULAR; }
    virtual bool				isCircular( void )                              { return m_AttributeFlags & ASSET_ATTRIB_FLAG_CIRCULAR ? true : false; }
    virtual void				setIsTemporary( bool isTemp )                   { if( isTemp ) m_AttributeFlags |= ASSET_ATTRIB_TEMPORARY; else m_AttributeFlags &= ~ASSET_ATTRIB_TEMPORARY; }
    virtual bool				isTemporary( void )                             { return m_AttributeFlags & ASSET_ATTRIB_TEMPORARY ? true : false; }
    virtual bool				isPermanent( void )                             { return !isTemporary(); }
    virtual void				setIsDeleted( bool isDeleted )                  { if( isDeleted ) m_AttributeFlags |= ASSET_ATTRIB_DELETED; else m_AttributeFlags &= ~ASSET_ATTRIB_DELETED; }
    virtual bool				isDeleted( void )                               { return m_AttributeFlags & ASSET_ATTRIB_DELETED ? true : false; }

    // assetName is usually the file name
    virtual void				setAssetName( const char* assetName );
    virtual void				setAssetName( std::string& assetName )          { m_AssetName = assetName; }
    virtual std::string&		getAssetName( void )                            { return m_AssetName; }
    virtual std::string			getRemoteAssetName( void );

    virtual void				setAssetTag( const char* assetTag );
    virtual std::string&		getAssetTag( void ) { return m_AssetTag; }

    virtual void				setAssetType( enum EAssetType assetType )       { m_u16AssetType = (uint16_t)assetType; }
    virtual EAssetType			getAssetType( void )                            { return (EAssetType)m_u16AssetType; }

    virtual void				setAssetLength( int64_t assetLength )           { m_s64AssetLen = assetLength; }
    virtual int64_t				getAssetLength( void )                          { return m_s64AssetLen; }
    virtual void				updateAssetLength( int64_t assetLength );

    virtual void				setAssetHashId( VxSha1Hash& id )                { m_AssetHash = id; }
    virtual void				setAssetHashId( uint8_t * id )                  { m_AssetHash.setHashData( id ); }
    virtual VxSha1Hash&			getAssetHashId( void )                          { return m_AssetHash; }

    virtual void				setAssetUniqueId( VxGUID& uniqueId )            { m_UniqueId = uniqueId; }
    virtual void				setAssetUniqueId( const char* guid )           { m_UniqueId.fromVxGUIDHexString( guid ); }
    virtual VxGUID&				getAssetUniqueId( void )                        { return m_UniqueId; }
    virtual VxGUID&				generateNewUniqueId( bool ifNotValid = false ); // generates unique id, assigns it to asset and returns reference to it

    virtual void				setCreatorId( VxGUID creatorId )                { BaseInfo::setOnlineId( creatorId ); }
    virtual void				setCreatorId( const char* creatorId )          { BaseInfo::setOnlineId( creatorId ); }
    virtual VxGUID&				getCreatorId( void )                            { return BaseInfo::getOnlineId(); }

    virtual void				setHistoryId( VxGUID& historyId )               { m_HistoryId = historyId; }
    virtual void				setHistoryId( const char* historyId )          { m_HistoryId.fromVxGUIDHexString( historyId ); }
    virtual VxGUID&				getHistoryId( void )                            { return m_HistoryId; }

    virtual void				setLocationFlags( uint32_t locFlags )           { m_LocationFlags = locFlags; }
    virtual uint32_t			getLocationFlags( void )                        { return m_LocationFlags; }

    virtual void				setAttributeFlags( uint16_t locFlags )          { m_AttributeFlags = locFlags; }
    virtual uint16_t			getAttributeFlags( void )                       { return m_AttributeFlags; }

    virtual void				setAssetSendState( enum EAssetSendState sendState )  { m_AssetSendState = sendState; }
    virtual EAssetSendState		getAssetSendState( void )                       { return m_AssetSendState; }

    virtual void				setCreationTime( uint64_t timestamp )           { m_CreationTime = timestamp; m_InfoModifiedTime = timestamp; m_AccessedTime = timestamp; }
    virtual uint64_t			getCreationTime( void )                         { return m_CreationTime; }

    virtual void				setModifiedTime( uint64_t timestamp )           { BaseInfo::setInfoModifiedTime( timestamp ); m_AccessedTime = timestamp; }
    virtual uint64_t			getModifiedTime( void )                         { return BaseInfo::getInfoModifiedTime(); }

    virtual void				setAccessedTime( uint64_t timestamp )           { m_AccessedTime = timestamp; }
    virtual uint64_t			getAccessedTime( void )                         { return m_AccessedTime; }

    virtual void				setExpiresTime( uint64_t timestamp )            { m_ExpiresTime = timestamp; }
    virtual uint64_t			getExpiresTime( void )                          { return m_ExpiresTime; }

    virtual void				setPlayPosition( int pos0to100000 )             { m_PlayPosition0to100000 = pos0to100000; }
    virtual int					getPlayPosition( void )                         { return m_PlayPosition0to100000; }

    virtual void				updateAssetInfo( VxThread* callingThread );
    virtual bool				needsHashGenerated( void );

    static const char*			getDefaultFileExtension( enum EAssetType assetType );
    static const char*			getSubDirectoryName( enum EAssetType assetType );

    virtual void                print( void ) const;

public:
    //=== vars ===//
    std::string					m_AssetName{""}; // usually file name
	std::string					m_AssetTag{""};
	VxGUID						m_UniqueId;
	VxGUID						m_HistoryId; 
	VxSha1Hash					m_AssetHash;
    int64_t						m_s64AssetLen{ 0 };
    uint16_t					m_u16AssetType{ VXFILE_TYPE_UNKNOWN };
    uint16_t					m_AttributeFlags{ 0 };
	uint32_t					m_LocationFlags{ 0 };
    int64_t						m_CreationTime{ 0 };
    int64_t						m_AccessedTime{ 0 };
    int64_t						m_ExpiresTime{ 0 }; // time when will be removed. 0 = never
	EAssetSendState			    m_AssetSendState{ eAssetSendStateNone };
    int						    m_PlayPosition0to100000{ 0 };
};

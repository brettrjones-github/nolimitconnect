#pragma once
//============================================================================
// Copyright (C) 2021 Brett R. Jones
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

#include <GoTvInterface/IDefs.h>
#include <CoreLib/VxFileTypeMasks.h>
#include <CoreLib/VxSha1Hash.h>
#include <CoreLib/VxGUID.h>

#include <string>

#define OFFER_LOC_FLAG_PERSONAL_RECORD			0x0001
#define OFFER_LOC_FLAG_LIBRARY					0x0002
#define OFFER_LOC_FLAG_SHARED_FILE				0x0004
#define OFFER_LOC_FLAG_CHAT_ROOM				0x0008

#define OFFER_ATTRIB_FLAG_CIRCULAR			    0x0001
#define OFFER_ATTRIB_TEMPORARY			        0x0002

class VxThread;

class OfferBaseInfo
{
public:
    OfferBaseInfo();
    OfferBaseInfo( const OfferBaseInfo& rhs );
    OfferBaseInfo( const std::string& fileName );
    OfferBaseInfo( const char * fileName, uint64_t fileLen, uint16_t assetType );

    OfferBaseInfo&				operator=( const OfferBaseInfo& rhs );

    virtual bool				isValid( void );
    virtual bool				isMine( void );

    virtual bool				isDirectory( void );
    virtual bool				isUnknownOffer( void )                          { return ( 0 == m_u16OfferType ) ? true : false; }
    virtual bool				isChatTextOffer( void )                         { return ( eOfferTypeChatText & m_u16OfferType ) ? true : false; }
    virtual bool				isChatFaceOffer( void )                         { return ( (eOfferTypeChatFace)& m_u16OfferType ) ? true : false; }
    virtual bool				isChatAvatarOffer( void )                       { return ( ( eOfferTypeChatStockAvatar | eOfferTypeChatCustomAvatar ) & m_u16OfferType ) ? true : false; }
    virtual bool				isPhotoOffer( void )                            { return ( eOfferTypePhotoFile & m_u16OfferType ) ? true : false; }
    virtual bool				isThumbOffer( void )                            { return ( eOfferTypeThumbnail & m_u16OfferType ) ? true : false; }
    virtual bool				isAudioOffer( void )                            { return ( eOfferTypeAudioFile & m_u16OfferType ) ? true : false; }
    virtual bool				isVideoOffer( void )                            { return ( eOfferTypeVideoFile & m_u16OfferType ) ? true : false; }
    virtual bool				isFileOffer( void );
    virtual bool				hasFileName( void );

    void						setIsChatRoomRecord( bool isSharedOffer )	    { if( isSharedOffer ) m_LocationFlags |= OFFER_LOC_FLAG_CHAT_ROOM; else m_LocationFlags &= ~OFFER_LOC_FLAG_CHAT_ROOM; }
    bool						isChatRoomRecord( void )				        { return m_LocationFlags & OFFER_LOC_FLAG_CHAT_ROOM ? true : false; }
    virtual void				setIsPersonalRecord( bool isRecord )            { if( isRecord ) m_LocationFlags |= OFFER_LOC_FLAG_PERSONAL_RECORD; else m_LocationFlags &= ~OFFER_LOC_FLAG_PERSONAL_RECORD; }
    virtual bool				isPersonalRecord( void )                        { return m_LocationFlags & OFFER_LOC_FLAG_PERSONAL_RECORD ? true : false; }
    virtual void				setIsInLibary( bool isInLibrary )               { if( isInLibrary ) m_LocationFlags |= OFFER_LOC_FLAG_LIBRARY; else m_LocationFlags &= ~OFFER_LOC_FLAG_LIBRARY; }
    virtual bool				isInLibary( void )                              { return m_LocationFlags & OFFER_LOC_FLAG_LIBRARY ? true : false; }
    virtual void				setIsSharedFileOffer( bool isSharedOffer )      { if( isSharedOffer ) m_LocationFlags |= OFFER_LOC_FLAG_SHARED_FILE; else m_LocationFlags &= ~OFFER_LOC_FLAG_SHARED_FILE; }
    virtual bool				isSharedFileOffer( void )                       { return m_LocationFlags & OFFER_LOC_FLAG_SHARED_FILE ? true : false; }

    virtual void				setIsCircular( bool isCircular )                { if( isCircular ) m_AttributeFlags |= OFFER_ATTRIB_FLAG_CIRCULAR; else m_AttributeFlags &= ~OFFER_ATTRIB_FLAG_CIRCULAR; }
    virtual bool				isCircular( void )                              { return m_AttributeFlags & OFFER_ATTRIB_FLAG_CIRCULAR ? true : false; }
    virtual void				setIsTemporary( bool isTemp )                   { if( isTemp ) m_AttributeFlags |= OFFER_ATTRIB_TEMPORARY; else m_AttributeFlags &= ~OFFER_ATTRIB_TEMPORARY; }
    virtual bool				isTemporary( void )                             { return m_AttributeFlags & OFFER_ATTRIB_TEMPORARY ? true : false; }
    virtual bool				isPermanent( void )                             { return !isTemporary(); }

    virtual void				setOfferName( const char * assetName );
    virtual void				setOfferName( std::string& assetName )          { m_OfferName = assetName; }
    virtual std::string&		getOfferName( void )                            { return m_OfferName; }
    virtual std::string			getRemoteOfferName( void );

    virtual void				setOfferTag( const char * assetTag );
    virtual std::string&		getOfferTag( void ) { return m_OfferTag; }

    virtual void				setOfferType( EOfferType assetType )            { m_u16OfferType = (uint16_t)assetType; }
    virtual EOfferType			getOfferType( void )                            { return (EOfferType)m_u16OfferType; }

    virtual void				setOfferLength( int64_t assetLength )           { m_s64OfferLen = assetLength; }
    virtual int64_t				getOfferLength( void )                          { return m_s64OfferLen; }

    virtual void				setOfferId( VxGUID& uniqueId )                  { m_OfferId = uniqueId; }
    virtual void				setOfferId( const char * guid )                 { m_OfferId.fromVxGUIDHexString( guid ); }
    virtual VxGUID&				getOfferId( void )                              { return m_OfferId; }
    virtual VxGUID&				generateNewOfferId( void ); // generates unique id, assigns it to offer id and returns reference to it

    virtual void				setOfferHashId( VxSha1Hash& id )                { m_OfferHash = id; }
    virtual void				setOfferHashId( uint8_t * id )                  { m_OfferHash.setHashData( id ); }
    virtual VxSha1Hash&			getOfferHashId( void )                          { return m_OfferHash; }

    virtual void				setCreatorId( VxGUID& creatorId )               { m_CreatorId = creatorId; }
    virtual void				setCreatorId( const char * creatorId )          { m_CreatorId.fromVxGUIDHexString( creatorId ); }
    virtual VxGUID&				getCreatorId( void )                            { return m_CreatorId; }

    virtual void				setHistoryId( VxGUID& historyId )               { m_HistoryId = historyId; }
    virtual void				setHistoryId( const char * historyId )          { m_HistoryId.fromVxGUIDHexString( historyId ); }
    virtual VxGUID&				getHistoryId( void )                            { return m_HistoryId; }

    virtual void				setThumbId( VxGUID& historyId )                 { m_ThumbId = historyId; }
    virtual void				setThumbId( const char * historyId )            { m_ThumbId.fromVxGUIDHexString( historyId ); }
    virtual VxGUID&				getThumbId( void )                              { return m_ThumbId; }

    virtual void				setLocationFlags( uint32_t locFlags )           { m_LocationFlags = locFlags; }
    virtual uint32_t			getLocationFlags( void )                        { return m_LocationFlags; }

    virtual void				setAttributeFlags( uint16_t locFlags )          { m_AttributeFlags = locFlags; }
    virtual uint16_t			getAttributeFlags( void )                       { return m_AttributeFlags; }

    virtual void				setOfferSendState( EOfferSendState sendState )  { m_OfferSendState = sendState; }
    virtual EOfferSendState		getOfferSendState( void )                       { return m_OfferSendState; }

    virtual void				setCreationTime( uint64_t timestamp )           { m_CreationTime = timestamp; m_ModifiedTime = timestamp; m_AccessedTime = timestamp; }
    virtual uint64_t			getCreationTime( void )                         { return m_CreationTime; }

    virtual void				setModifiedTime( uint64_t timestamp )           { m_ModifiedTime = timestamp; m_AccessedTime = timestamp; }
    virtual uint64_t			getModifiedTime( void )                         { return m_ModifiedTime; }

    virtual void				setAccessedTime( uint64_t timestamp )           { m_AccessedTime = timestamp; }
    virtual uint64_t			getAccessedTime( void )                         { return m_AccessedTime; }

    virtual void				setPlayPosition( int pos0to100000 )             { m_PlayPosition0to100000 = pos0to100000; }
    virtual int					getPlayPosition( void )                         { return m_PlayPosition0to100000; }

    virtual void				updateOfferInfo( VxThread * callingThread );
    virtual bool				needsHashGenerated( void );

    static const char *			getDefaultFileExtension( EOfferType assetType );
    static const char *			getSubDirectoryName( EOfferType assetType );

public:
    //=== vars ===//
    std::string					m_OfferName{""}; // usually file name
	std::string					m_OfferTag{""};
	VxGUID						m_OfferId;
	VxGUID						m_CreatorId;
	VxGUID						m_HistoryId; 
    VxGUID						m_ThumbId; 
	VxSha1Hash					m_OfferHash;
    int64_t						m_s64OfferLen{ 0 };
    uint16_t					m_u16OfferType{ VXFILE_TYPE_UNKNOWN };
    uint16_t					m_AttributeFlags{ 0 };
	uint32_t					m_LocationFlags{ 0 };
    int64_t						m_CreationTime{ 0 };
    int64_t						m_ModifiedTime{ 0 };
    int64_t						m_AccessedTime{ 0 };
	EOfferSendState			    m_OfferSendState{ eOfferSendStateNone };
    int						    m_PlayPosition0to100000{ 0 };
};

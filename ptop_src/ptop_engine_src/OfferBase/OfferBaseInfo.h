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
// http://www.nolimitconnect.org
//============================================================================

#include <ptop_src/ptop_engine_src/AssetMgr/AssetInfo.h>

class FileInfo;

class OfferBaseInfo : public AssetInfo
{
public:
    OfferBaseInfo() = default;
    OfferBaseInfo( const OfferBaseInfo& rhs );
    OfferBaseInfo( std::string fileName );
    OfferBaseInfo( std::string fileName, uint64_t assetLen, uint16_t assetType );
    OfferBaseInfo( FileInfo& fileInfo );

    OfferBaseInfo& operator=( const OfferBaseInfo& rhs );

    virtual bool                addToBlob( PktBlobEntry& blob ) override;
    virtual bool                extractFromBlob( PktBlobEntry& blob ) override;

	virtual void				setOfferType( EOfferType assetType )	{ setAssetType( (EAssetType)assetType ); }
	virtual EOfferType			getOfferType( void )					{ return (EOfferType)getAssetType(); }

    virtual void				setOfferName( std::string& assetName )  { setAssetName( assetName ); }
    virtual std::string&        getOfferName( void )                    { return getAssetName(); }

    virtual void				setOfferLength( int64_t assetLength )   { setAssetLength( assetLength ); }
    virtual int64_t				getOfferLength( void )                  { return getAssetLength(); }

    virtual void				setOfferHashId( VxSha1Hash& hashId )    { setAssetHashId( hashId ); }
    virtual void				setOfferHashId( uint8_t* hashId )       { setAssetHashId( hashId ); }
    virtual VxSha1Hash&         getOfferHashId( void )                  { return getAssetHashId(); }

    virtual void				setOfferTag( std::string assetTag )     { setAssetTag( assetTag.c_str() ); }
    virtual std::string&        getOfferTag( void )                     { return getAssetTag(); }

    virtual void				setOfferSendState( EOfferSendState sendState ) { setAssetSendState( (EAssetSendState)sendState ); }
    virtual EOfferSendState		getOfferSendState( void )               { return (EOfferSendState)getAssetSendState(); }

    virtual std::string         getRemoteOfferName( void )              { return getRemoteAssetName(); }

    virtual bool                isFileOffer( void )                     { return isFileAsset(); }

    virtual void				setIsSharedFileOffer( bool isSharedOffer ) { setIsSharedFileAsset( isSharedOffer ); }
    virtual bool				isSharedFileOffer( void )               { return isSharedFileAsset(); }

    virtual void				setOfferMsg( std::string offerMsg )     { m_OfferMsg = offerMsg; }
    virtual std::string&		getOfferMsg( void )                     { return m_OfferMsg; }

    virtual void				setOfferExpireTime( int64_t expireTime ) { m_OfferExpireTime = expireTime; }
    virtual int64_t             getOfferExpireTime( void )              { return m_OfferExpireTime; }

    virtual void                setPluginType( EPluginType pluginType ) { m_PluginType = pluginType; }
    virtual EPluginType         getPluginType( void )                   { return m_PluginType; }

    virtual void				setOfferId( VxGUID& sessionId )         { m_OfferId = sessionId; }
    virtual void				setOfferId( const char* sessionId )     { m_OfferId.fromVxGUIDHexString( sessionId ); }
    virtual VxGUID&             getOfferId( void )                      { return m_OfferId; }

    virtual void                setOfferResponse( EOfferResponse offerResponse ) { m_OfferResponse = offerResponse; }
    virtual EOfferResponse      getOfferResponse( void )                { return m_OfferResponse; }

    virtual void                setOfferMgr( EOfferMgrType offerMgr )   { m_OfferMgr = offerMgr; }
    virtual EOfferMgrType       getOfferMgr( void )                     { return m_OfferMgr; }

protected:
    EPluginType                 m_PluginType{ePluginTypeInvalid};
    std::string                 m_OfferMsg{ "" };
    int64_t                     m_OfferExpireTime{ 0 };
    VxGUID                      m_OfferId;
    EOfferResponse              m_OfferResponse{ eOfferResponseNotSet };
    EOfferMgrType               m_OfferMgr{ eOfferMgrNotSet };
};

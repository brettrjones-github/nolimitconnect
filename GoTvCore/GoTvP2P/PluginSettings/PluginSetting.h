//============================================================================
// Copyright (C) 2019 Brett R. Jones
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
#pragma once

#include <GoTvInterface/IDefs.h>
#include <CoreLib/VxGUID.h>
#include <CoreLib/BinaryBlob.h>

#include <string>
#include <vector>

#pragma pack(push)
#pragma pack(1)


// do not add any virtual functions so can be used in network packets directly
class PluginSettingHdr
{
public:
    PluginSettingHdr(){};  // default constructor
    ~PluginSettingHdr(){};

    //! copy constructor
    PluginSettingHdr( const PluginSettingHdr & rhs ) = default;
    //! copy operator
    PluginSettingHdr& operator =( const PluginSettingHdr & rhs ) = default;
    //! move operator
    PluginSettingHdr& operator =( PluginSettingHdr && rhs ) = default;

    void                        setPluginType( EPluginType pluginType )             { m_PluginType = ( uint16_t )pluginType; }
    EPluginType                 getPluginType( void )                               { return ( EPluginType )m_PluginType; }
    void                        setThumnailId( VxGUID  thumbAssetId )               { m_PluginThumb = thumbAssetId; }
    VxGUID&                     getThumnailId( void )                               { return m_PluginThumb; }
    void                        setThumnailIsCircle( bool isCircle )                { m_PluginThumbIsCircular = isCircle; }
    bool                        getThumnailIsCircle( void )                         { return m_PluginThumbIsCircular; }

    void                        setLanguage( ELanguageType language )               { m_Language = ( uint16_t )language; }
    ELanguageType               getLanguage( void )                                 { return ( ELanguageType )m_Language; }
    void                        setContentRating( EContentRating contentType )      { m_ContentRating = ( uint16_t )contentType; }
    EContentRating              getContentRating( void )                            { return ( EContentRating )m_ContentRating; }
    void                        setContentCatagory( EContentCatagory contentCat )   { m_ContentCatagory = ( uint16_t )contentCat; }
    EContentCatagory            getContentCatagory( void )                           { return ( EContentCatagory )m_ContentCatagory; }

    void                        setMaxConnectPerUser( int maxConnect )              { m_MaxConnectionsPerUser = (uint32_t)maxConnect; }
    int                         getMaxConnectPerUser( void )                        { return m_MaxConnectionsPerUser; }
    void                        setMaxStoreAndForwardPerUser( int maxStoreCnt )     { m_MaxStoreAndForwardPerUser = (uint32_t)maxStoreCnt; }
    int                         getMaxStoreAndForwardPerUser( void )                { return m_MaxStoreAndForwardPerUser; }

    void                        setSecondaryPermissionLevel( int permLevel )        { m_SecondaryPermissionLevel = (uint8_t)permLevel; }
    int                         getSecondaryPermissionLevel( void )                 { return m_SecondaryPermissionLevel; }
    void                        setSecondaryPluginType( EPluginType pluginType )    { m_SecondaryPluginType = ( uint16_t )pluginType; }
    EPluginType                 getSecondaryPluginType( void )                      { return ( EPluginType )m_SecondaryPluginType; }
    void                        setSecondaryThumnailId( VxGUID&  thumbAssetId )     { m_SecondaryPluginThumb = thumbAssetId; }
    VxGUID&                     getSecondaryThumnailId( void )                      { return m_SecondaryPluginThumb; }
    void                        setSecondaryIdentGuid( VxGUID&  identGuid )         { m_SecondaryIdentGuid = identGuid; }
    VxGUID&                     getSecondaryIdentGuid( void )                       { return m_SecondaryIdentGuid; }
    void                        setAnnounceToHost( bool announce )                  { m_AnnounceToHost = ( uint8_t )announce; }
    bool                        getAnnounceToHost( void )                           { return m_AnnounceToHost; }

    void                        setRes1( uint32_t res1 )                            { m_Reserve1Setting = (uint32_t)res1; }
    uint32_t                    getRes1( void )                                     { return m_Reserve1Setting; }
    void                        setRes2( uint32_t res2 )                            { m_Reserve2Setting = (uint32_t)res2; }
    uint32_t                    getRes2( void )                                     { return m_Reserve2Setting; }

    void                        setUpdateTimestampToNow( void );
    void                        setLastUpdateTimestamp( int64_t timeStamp )         { m_UpdateTimestamp = timeStamp; }
    int64_t                     getLastUpdateTimestamp( void )                      { return m_UpdateTimestamp; }

protected:
    uint16_t                    m_BlobStorageVersion = BLOB_PLUGIN_SETTING_STORAGE_VERSION;
    uint16_t                    m_SecondaryPermissionLevel = 0;
    uint16_t                    m_PluginType = ( uint16_t )ePluginTypeInvalid;
    uint16_t                    m_SecondaryPluginType = ( uint16_t )ePluginTypeInvalid;
    int64_t                     m_UpdateTimestamp = 0;
    VxGUID                      m_PluginThumb;
    VxGUID                      m_SecondaryPluginThumb;
    VxGUID                      m_SecondaryIdentGuid;

    uint16_t                    m_Language = 0;
    uint16_t                    m_ContentRating = 0;
    uint16_t                    m_ContentCatagory = 0;
    uint16_t                    m_ContentSubCatagory = 0;
    uint16_t                    m_MaxConnectionsPerUser = 0;
    uint16_t                    m_MaxStoreAndForwardPerUser = 0;
    uint8_t                     m_AnnounceToHost = 0;
    uint8_t                     m_PluginThumbIsCircular = 0;
    uint16_t                    m_ResBw1 = 0;
    uint32_t                    m_Reserve1Setting = 0;
    uint32_t                    m_Reserve2Setting = 0;

};

#pragma pack(pop)

class BinaryBlob;
class P2PEngine;

class PluginSetting : public PluginSettingHdr
{
public:
    PluginSetting() = default;
    virtual ~PluginSetting() = default;
    //! copy constructor
    PluginSetting( const PluginSetting & rhs ) = default;
    //! copy operator
    PluginSetting& operator =( const PluginSetting & rhs ) = default;
    //! move operator
    PluginSetting& operator =( PluginSetting && rhs ) = default;

    //! set to default values for case use has not set anything but has enabled the plugin
    bool                        setDefaultValues( P2PEngine& engine, EPluginType pluginType );

    void                        setTitle( std::string title )                       { m_PluginTitle = title; }
    std::string&                getTitle( void )                                    { return m_PluginTitle; }
    void                        setDescription( std::string desc )                  { m_PluginDesc = desc; }
    std::string&                getDescription( void )                              { return m_PluginDesc; }
    void                        setPluginUrl( std::string  url )                    { m_PluginUrl = url; }
    std::string&                getPluginUrl( void )                                { return m_PluginUrl; }
    void                        setKeyWords( std::string keyWords )                 { m_KeyWords = keyWords; }
    std::string&                getKeyWords( void )                                 { return m_KeyWords; }
    void                        setSecondaryTitle( std::string title )              { m_SecondaryPluginTitle = title; }
    std::string&                getSecondaryTitle( void )                           { return m_SecondaryPluginTitle; }
    void                        setSecondaryDescription( std::string desc )         { m_SecondaryPluginDesc = desc; }
    std::string&                getSecondaryDescription( void )                     { return m_SecondaryPluginDesc; }
    void                        setSecondaryUrl( std::string  url )                 { m_SecondaryUrl = url; }
    std::string&                getSecondaryUrl( void )                             { return m_SecondaryUrl; }

    bool                        toBinary( BinaryBlob& binarySetting, bool networkOrder = false );
    bool                        fromBinary( BinaryBlob& binarySetting, bool networkOrder = false );

protected:
    bool                        setStringList( std::vector<std::string>& stringList );
    bool                        getStringList( std::vector<std::string>& stringList );

    std::string                 m_PluginTitle;
    std::string                 m_PluginUrl;
    std::string                 m_PluginDesc;
    std::string                 m_KeyWords;
    std::string                 m_SecondaryPluginTitle;
    std::string                 m_SecondaryPluginDesc;
    std::string                 m_SecondaryUrl;

};

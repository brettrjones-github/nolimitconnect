#pragma once
//============================================================================
// Copyright (C) 2020 Brett R. Jones
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

#include <inttypes.h>
#include <string>
#include <vector>
#include <GuiInterface/IDefs.h>

// a class very similar to <CoreLib/BinaryBlob.h>
// but a fixed length and pragma packed for network packet use
// the length assumes the base packet is 2K or less in size

// defines for pkt blob
#define PKT_BLOB_MAX_STORAGE_LEN (1024 * 14 - 16) // approx 14K to fit within MAX_PKT_LEN - 2K.. -16 for packet alignment
#define PKT_BLOB_STORAGE_VERSION 1
#define PKT_BLOB_MAX_ONLINE_IDS (PKT_BLOB_MAX_STORAGE_LEN / 16) // max number of guids a empty blob can hold.. this is search result limit (895 entries)
#define PKT_BLOB_MAX_PLUGIN_IDS (PKT_BLOB_MAX_STORAGE_LEN / 17) // max number of guid and plugin pair a empty blob can hold.. this is search result limit (842 entries)

class VxGUID;
class PluginId;
class VxSha1Hash;

#pragma pack(push)
#pragma pack(1)
class PktBlobEntry
{
public:
    PktBlobEntry();
    ~PktBlobEntry() = default;

    // gets blob len including member variables (does not round to 16 byte boundry)
    int16_t                     getTotalBlobLen( void )         { return sizeof(PktBlobEntry) - sizeof(m_BlobData) + getBlobLen(); }

    void                        setUseNetworkOrder( bool useNetOrder ) { m_UseNetworkOrder = useNetOrder; }
    bool                        getUseNetworkOrder( void )      { return m_UseNetworkOrder; }

    void                        setBlobObjectVersion( int ver ) { m_BlobObjVersion = (uint8_t)ver; }
    uint8_t                     getBlobObjectVersion( void )    { return m_BlobObjVersion; }

    void                        setDataVersion( int ver )       { m_DataVersion = (uint8_t)ver; }
    uint8_t                     getDataVersion( void )          { return m_DataVersion; }

    void                        setBlobDataType( int dataType );
    uint16_t                    getBlobDataType( void );

    void                        setBlobLen( int len );
    int16_t                     getBlobLen( void ) const;
    int16_t                     getRemainingStorageLen( void ) const { return getMaxBlobLen() - getBlobLen(); }

    bool                        setBlobData( uint8_t* blob, int len );                   
    uint8_t*                    getBlobData( void )             { return m_BlobData; }

    void                        resetWrite()                    { m_DataIdx = 0; m_PastEnd = 0; }
    bool                        haveRoom( int valSize )         { return (int)( getMaxBlobLen() - getDataIdx() ) >= valSize; }
    bool                        incDataWrite( int valSize );

    void                        resetRead()                     { m_DataIdx = 0; m_PastEnd = 0; }
    bool                        haveData( int valSize )         { return (int)( getBlobLen() - getDataIdx() ) >= valSize; }
    bool                        incDataRead( int valSize );

    void                        setMaxBlobLen( int len );
    int16_t                     getMaxBlobLen( void ) const;
    void                        setDataIdx( int len );
    int16_t                     getDataIdx( void ) const;

    bool                        isPastEnd()                     { return m_PastEnd; }

    //! set value 
    bool                        setValue( enum EAgeType& eValue );
    bool                        setValue( enum EContentRating& eValue );
    bool                        setValue( enum EGenderType& eValue );
    bool                        setValue( enum EHostType& eValue );
    bool                        setValue( enum ELanguageType& eValue );
    bool                        setValue( enum EPluginType& eValue );
    bool                        setValue( enum ESearchType& eValue );
    bool                        setValue( const PluginId& pluginId );
    bool                        setValue( VxGUID& guid );
    bool                        setValue( VxSha1Hash& sha1Hash );
    bool                        setValue( bool& bValue );
    bool                        setValue( int8_t& s8Value );
    bool                        setValue( uint8_t& u8Value );
    bool                        setValue( int16_t& s16Value );
    bool                        setValue( uint16_t& u16Value );
    bool                        setValue( int32_t& s32Value );
    bool                        setValue( uint32_t& u32Value );
    bool                        setValue( int64_t& s64Value );
    bool                        setValue( uint64_t& u64Value );
    bool                        setValue( float& f32Value );
    bool                        setValue( double& f64Value );
    bool                        setValue( std::string& strValue );
    bool                        setValue( const char* pRetBuf, int iBufLen );
    bool                        setValue( std::vector<std::string>& aoStrValues );
    bool                        setValue( void * pvRetBuf, int iBufLen );

    //! get value 
    bool                        getValue( EAgeType& eValue );
    bool                        getValue( EContentRating& eValue );
    bool                        getValue( EGenderType& eValue );
    bool                        getValue( EHostType& eValue );
    bool                        getValue( ELanguageType& eValue );
    bool                        getValue( EPluginType& eValue );
    bool                        getValue( ESearchType& eValue );
    bool                        getValue( PluginId& pluginId );
    bool                        getValue( VxGUID& guid );
    bool                        getValue( VxSha1Hash& sha1Hash );
    bool                        getValue( bool& bValue );
    bool                        getValue( int8_t& s8Value );
    bool                        getValue( uint8_t& u8Value );
    bool                        getValue( int16_t& s16Value );
    bool                        getValue( uint16_t& u16Value );
    bool                        getValue( int32_t& s32Value );
    bool                        getValue( uint32_t& u32Value );
    bool                        getValue( int64_t& s64Value );
    bool                        getValue( uint64_t& u64Value );
    bool                        getValue( float& f32Value );
    bool                        getValue( double& f64Value );
    bool                        getValue( std::string& strValue );
    bool                        getValue( char * pRetBuf, int& iBufLen );
    bool                        getValue( std::vector<std::string>& aoStrValues );
    bool                        getValue( void * pvRetBuf, int& iBufLen );

protected:
    //=== vars ===//
    uint8_t                     m_UseNetworkOrder{ true };
    uint8_t                     m_BlobObjVersion{ PKT_BLOB_STORAGE_VERSION };
    uint8_t                     m_PastEnd{ 0 };  
    uint8_t                     m_DataVersion{ 0 };     // can be set by caller for versioning
    uint16_t                    m_DataType{ 0 };        // can be set by caller for the type of data contained in blob
    int16_t                     m_DataIdx{ 0 };
    // 8 bytes to here
    int16_t                     m_BlobLen{ 0 };
    int16_t                     m_MaxBlobLen{ PKT_BLOB_MAX_STORAGE_LEN };
    int32_t                     m_BlobRes3{ 0 };        // reserved for alignment
    // 16 bytes to here

    uint8_t                     m_BlobData[PKT_BLOB_MAX_STORAGE_LEN + 16];
};

#pragma pack(pop)

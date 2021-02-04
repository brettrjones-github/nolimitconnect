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
// http://www.nolimitconnect.com
//============================================================================

#include <inttypes.h>
#include <string>
#include <vector>

// a class very similar to <CoreLib/BinaryBlob.h>
// but a fixed length and pragma packed for network packet use
// the length assumes the base packet is 2K or less in size

// defines for pkt blob
#define PKT_BLOB_MAX_STORAGE_LEN (1024 * 14 - 16) // approx 14K to fit within MAX_PKT_LEN - 2K.. -16 for packet alignment
#define PKT_BLOB_STORAGE_VERSION 1

class VxGUID;

#pragma pack(push)
#pragma pack(1)
class PktBlob
{
public:
    PktBlob() = default;
    ~PktBlob() = default;

    void                        setBlobVersion( int ver )       { m_BlobVersion = (uint8_t)ver; }
    uint8_t                     getBlobVersion( void )          { return m_BlobVersion; }

    void                        setBlobLen( int len )           { m_BlobLen = (int16_t)len; }
    int16_t                     getBlobLen( void ) const        { return m_BlobLen; }
    int16_t                     getRemainingStorageLen( void ) const { return PKT_BLOB_MAX_STORAGE_LEN - getBlobLen(); }

    bool                        setBlobData( uint8_t* blob, int len );                   
    uint8_t*                    getBlobData( void )             { return m_BlobData; }

    void                        setUseNetworkOrder( bool useNetOrder ) { m_UseNetworkOrder = useNetOrder; }
    bool                        getUseNetworkOrder( void )      { return m_UseNetworkOrder; }

    void                        resetWrite()                    { m_DataIdx = 0; m_PastEnd = 0; }
    bool                        haveRoom( int valSize )         { return (int)( m_MaxBlobLen - m_DataIdx ) >= valSize; }
    bool                        incDataWrite( int valSize );

    void                        resetRead()                     { m_DataIdx = 0; m_PastEnd = 0; }
    bool                        haveData( int valSize )         { return (int)( m_BlobLen - m_DataIdx ) >= valSize; }
    bool                        incDataRead( int valSize );

    bool                        isPastEnd()                     { return m_PastEnd; }

   

    //! set value 
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
    bool                        setValue( const char * pRetBuf, int iBufLen );
    bool                        setValue( std::vector<std::string>& aoStrValues );
    bool                        setValue( void * pvRetBuf, int iBufLen );
    bool                        setValue( VxGUID& guid );

    //! get value 
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
    bool                        getValue( VxGUID& guid );

protected:
    //=== vars ===//
    uint8_t                     m_BlobVersion{ PKT_BLOB_STORAGE_VERSION };
    uint8_t                     m_DataVersion{ 0 };     // can be set by caller for versioning
    uint8_t                     m_DataType{ 0 };        // can be set by caller for the type of data contained in blob
    uint8_t                     m_UseNetworkOrder{ false };
    int16_t                     m_DataIdx{ 0 };
    uint8_t                     m_PastEnd{ 0 };  

    uint8_t                     m_BlobRes1{ 0 };        // reserved for alignment
    int16_t                     m_BlobRes2{ 0 };        // reserved for alignment
    int32_t                     m_BlobRes3{ 0 };        // reserved for alignment

    int16_t                     m_BlobLen{ 0 };
    int16_t                     m_MaxBlobLen{ PKT_BLOB_MAX_STORAGE_LEN };

    uint8_t                     m_BlobData[PKT_BLOB_MAX_STORAGE_LEN + 16];
};

#pragma pack(pop)

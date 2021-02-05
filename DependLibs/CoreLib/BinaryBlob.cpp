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

#include "BinaryBlob.h"
#include "IsBigEndianCpu.h"
#include "VxGUID.h"
#include "VxDebug.h"
#include <memory.h>

//============================================================================
bool BinaryBlob::createStorage( int maxStorageLen, bool useNetworkOrder )
{
    if( maxStorageLen <= 0 )
    {
        LogMsg( LOG_ERROR, "BinaryBlob createStorage invalid param" );
        return false;
    }

    m_UseNetworkOrder = useNetworkOrder;
    m_BlobData = new uint8_t[ maxStorageLen + sizeof( int ) ];
    m_MaxDataLen = maxStorageLen;
    m_BlobLen = 0;
    m_OwnBlob = true;
    m_DataIdx = 0;
    m_PastEnd = false;
    return true;
}

//============================================================================
bool BinaryBlob::setBlobData( uint8_t* blob, int len, bool deleteOnDestruct, bool copyTheData )
{
    if( m_OwnBlob )
    {
        delete m_BlobData;
    }

    m_BlobData = nullptr;
    m_MaxDataLen = len; 
    m_BlobLen = len; 
    if( !copyTheData )
    {
        m_OwnBlob = deleteOnDestruct;
    }
    else
    {
        m_OwnBlob = true;
    }

    if( len )
    {
        if( copyTheData )
        {
            m_BlobData = new uint8_t[ len + sizeof( int ) ];
            memcpy( m_BlobData, blob, len );
        }
        else
        {
            m_BlobData = blob;
        }
    }

    return true;
}

//============================================================================
bool BinaryBlob::incDataWrite( int valSize )
{
    if( ( m_MaxDataLen - m_DataIdx ) >= valSize )
    {
        m_DataIdx += valSize;
        if( m_DataIdx > m_BlobLen )
        {
            m_BlobLen = m_DataIdx;
        }

        return true;
    }
    else
    {
        LogMsg( LOG_ERROR, "BinaryBlob Write past end" );
        m_PastEnd = true;
        return false;
    }
}

//============================================================================
bool BinaryBlob::incDataRead( int valSize )
{
    if( ( m_BlobLen - m_DataIdx ) >= valSize )
    {
        m_DataIdx += valSize;
        return true;
    }
    else
    {
        LogMsg( LOG_ERROR, "BinaryBlob Read past end" );
        m_PastEnd = true;
        return false;
    }
}

//============================================================================
bool BinaryBlob::setValue( bool& bValue )
{
    bool result = false;
    if( haveRoom( sizeof( uint8_t ) ) )
    {
        uint8_t boolVal = bValue ? 1 : 0;
        m_BlobData[ m_DataIdx ] = boolVal;
        incDataWrite( sizeof( uint8_t ) );
        result = true;
    }

    return result;
}

//============================================================================
bool BinaryBlob::setValue( int8_t& s8Value )
{
    bool result = false;
    if( haveRoom( sizeof( int8_t ) ) )
    {
        m_BlobData[ m_DataIdx ] = s8Value;
        incDataWrite( 1 );
        result = true;
    }

    return result;
}

//============================================================================
bool BinaryBlob::setValue( uint8_t& u8Value )
{
    bool result = false;
    if( haveRoom( sizeof( uint8_t ) ) )
    {
        m_BlobData[ m_DataIdx ] = u8Value;
        incDataWrite( 1 );
        result = true;
    }

    return result;
}

//============================================================================
bool BinaryBlob::setValue( int16_t& s16Value )
{
    bool result = false;
    if( haveRoom( sizeof( int16_t ) ) )
    {
        int16_t* data = ( int16_t* )(&m_BlobData[ m_DataIdx ]);
        if( m_UseNetworkOrder )
        {
            *data = htons(s16Value);
        }
        else
        {
            *data = s16Value;
        }

        incDataWrite( sizeof( int16_t ) );
        result = true;
    }

    return result;
}

//============================================================================
bool BinaryBlob::setValue( uint16_t& u16Value )
{
    bool result = false;
    if( haveRoom( sizeof( uint16_t ) ) )
    {
        uint16_t* data = ( uint16_t* )( &m_BlobData[ m_DataIdx ] );
        if( m_UseNetworkOrder )
        {
            *data = htons( u16Value );
        }
        else
        {
            *data = u16Value;
        }

        incDataWrite( sizeof( uint16_t ) );
        result = true;
    }

    return result;
}

//============================================================================
bool BinaryBlob::setValue( int32_t& s32Value )
{
    bool result = false;
    if( haveRoom( sizeof( int32_t ) ) )
    {
        int32_t* data = ( int32_t* )( &m_BlobData[ m_DataIdx ] );
        if( m_UseNetworkOrder )
        {
            *data = htonl( s32Value );
        }
        else
        {
            *data = s32Value;
        }

        incDataWrite( sizeof( int32_t ) );
        result = true;
    }

    return result;
}

//============================================================================
bool BinaryBlob::setValue( uint32_t& u32Value )
{
    bool result = false;
    if( haveRoom( sizeof( uint32_t ) ) )
    {
        uint32_t* data = ( uint32_t* )( &m_BlobData[ m_DataIdx ] );
        if( m_UseNetworkOrder )
        {
            *data = htonl( u32Value );
        }
        else
        {
            *data = u32Value;
        }

        incDataWrite( sizeof( uint32_t ) );
        result = true;
    }

    return result;
}

//============================================================================
bool BinaryBlob::setValue( int64_t& s64Value )
{
    bool result = false;
    if( haveRoom( sizeof( int64_t ) ) )
    {
        int64_t* data = ( int64_t* )( &m_BlobData[ m_DataIdx ] );
        if( m_UseNetworkOrder )
        {
            *data = htonU64( s64Value );
        }
        else
        {
            *data = s64Value;
        }

        incDataWrite( sizeof( int64_t ) );
        result = true;
    }

    return result;
}

//============================================================================
bool BinaryBlob::setValue( uint64_t& u64Value )
{
    bool result = false;
    if( haveRoom( sizeof( uint64_t ) ) )
    {
        uint64_t* data = ( uint64_t* )( &m_BlobData[ m_DataIdx ] );
        if( m_UseNetworkOrder )
        {
            *data = htonU64( u64Value );
        }
        else
        {
            *data = u64Value;
        }

        incDataWrite( sizeof( uint64_t ) );
        result = true;
    }

    return result;
}


//============================================================================
bool BinaryBlob::setValue( float& f32Value )
{
    bool result = false;
    if( haveRoom( sizeof( float ) ) )
    {
        float* data = ( float* )( &m_BlobData[ m_DataIdx ] );
        if( m_UseNetworkOrder )
        {
            *data = htonl( f32Value );
        }
        else
        {
            *data = f32Value;
        }

        incDataWrite( sizeof( float ) );
        result = true;
    }

    return result;
}

//============================================================================
bool BinaryBlob::setValue( double& f64Value )
{
    bool result = false;
    if( haveRoom( sizeof( double ) ) )
    {
        double* data = ( double* )( &m_BlobData[ m_DataIdx ] );
        if( m_UseNetworkOrder )
        {
            *data = htonU64( f64Value );
        }
        else
        {
            *data = f64Value;
        }

        incDataWrite( sizeof( double ) );
        result = true;
    }

    return result;
}

//============================================================================
bool BinaryBlob::setValue( std::string& strValue )
{
    return setValue( strValue.c_str(), strValue.length() + 1 );
}

//============================================================================
bool BinaryBlob::setValue( std::vector<std::string>& aoStrValues )
{
    uint32_t strCnt = ( uint32_t )aoStrValues.size();
    bool status = setValue( strCnt );
    if( status )
    {
        for( uint32_t i = 0; i < strCnt; i++ )
        {
            status &= setValue( aoStrValues[ i ] );
        }
    }

    return status;
}

//============================================================================
bool BinaryBlob::setValue( const char * pRetBuf, int iBufLen )
{
    return setValue( ( void* )pRetBuf, iBufLen );
}

//============================================================================
bool BinaryBlob::setValue( void * pvRetBuf, int iBufLen )
{
    if( haveRoom( iBufLen + sizeof( uint32_t ) ) )
    {
        if( pvRetBuf )
        {
            uint32_t u32Len = ( uint32_t )iBufLen;
            if( setValue( u32Len ) )
            {
                uint8_t* data = ( uint8_t* )( &m_BlobData[ m_DataIdx ] );
                memcpy( data, pvRetBuf, iBufLen );
                incDataWrite( iBufLen );
            }

            return true;
        }
    }

    return false;
}

//============================================================================
bool BinaryBlob::setValue( VxGUID& guid )
{
    uint64_t loPart = guid.getVxGUIDLoPart();
    uint64_t hiPart = guid.getVxGUIDHiPart();
    bool result = setValue( loPart );
    result &= setValue( hiPart );
    return result;
}

//============================================================================
// setValues
//============================================================================

//============================================================================
bool BinaryBlob::getValue( bool& bValue )
{
    if( haveData( 1 ) )
    {
        bValue = m_BlobData[ m_DataIdx ] ? true : false;
        incDataRead( 1 );
        return true;
    }

    return false;
}

//============================================================================
bool BinaryBlob::getValue( int8_t& s8Value )
{
    if( haveData( 1 ) )
    {
        s8Value = m_BlobData[ m_DataIdx ];
        incDataRead( 1 );
        return true;
    }

    return false;
}

//============================================================================
bool BinaryBlob::getValue( uint8_t& u8Value )
{
    if( haveData( 1 ) )
    {
        u8Value = m_BlobData[ m_DataIdx ];
        incDataRead( 1 );
        return true;
    }

    return false;
}

//============================================================================
bool BinaryBlob::getValue( int16_t& s16Value )
{
    if( haveData( sizeof( int16_t ) ) )
    {
        int16_t* data = ( int16_t* )( &m_BlobData[ m_DataIdx ] );
        if( m_UseNetworkOrder )
        {
            s16Value = ntohs(*data);
        }
        else
        {
            s16Value = *data;
        }

        incDataRead( sizeof( uint16_t ) );
        return true;
    }

    return false;
}

//============================================================================
bool BinaryBlob::getValue( uint16_t& u16Value )
{
    if( haveData( sizeof( uint16_t ) ) )
    {
        uint16_t* data = ( uint16_t* )( &m_BlobData[ m_DataIdx ] );
        if( m_UseNetworkOrder )
        {
            u16Value = ntohs(*data);
        }
        else
        {
            u16Value = *data;
        }

        incDataRead( sizeof( uint16_t ) );
        return true;
    }

    return false;
}

//============================================================================
bool BinaryBlob::getValue( int32_t& s32Value )
{
    if( haveData( sizeof( int32_t ) ) )
    {
        int32_t* data = ( int32_t* )( &m_BlobData[ m_DataIdx ] );
        if( m_UseNetworkOrder )
        {
            s32Value = ntohl( *data );
        }
        else
        {
            s32Value = *data;
        }

        incDataRead( sizeof( int32_t ) );
        return true;
    }

    return false;
}

//============================================================================
bool BinaryBlob::getValue( uint32_t& u32Value )
{
    if( haveData( sizeof( uint32_t ) ) )
    {
        uint32_t* data = ( uint32_t* )( &m_BlobData[ m_DataIdx ] );
        if( m_UseNetworkOrder )
        {
            u32Value = ntohl( *data );
        }
        else
        {
            u32Value = *data;
        }

        incDataRead( sizeof( uint32_t ) );
        return true;
    }

    return false;
}

//============================================================================
bool BinaryBlob::getValue( int64_t& s64Value )
{
    if( haveData( sizeof( int64_t ) ) )
    {
        int64_t* data = ( int64_t* )( &m_BlobData[ m_DataIdx ] );
        if( m_UseNetworkOrder )
        {
            s64Value = ntohU64( *data );
        }
        else
        {
            s64Value = *data;
        }

        incDataRead( sizeof( int64_t ) );
        return true;
    }

    return false;
}

//============================================================================
bool BinaryBlob::getValue( uint64_t& u64Value )
{
    if( haveData( sizeof( uint64_t ) ) )
    {
        uint64_t* data = ( uint64_t* )( &m_BlobData[ m_DataIdx ] );
        if( m_UseNetworkOrder )
        {
            u64Value = ntohU64( *data );
        }
        else
        {
            u64Value = *data;
        }

        incDataRead( sizeof( uint64_t ) );
        return true;
    }

    return false;
}

//============================================================================
bool BinaryBlob::getValue( float& f32Value )
{
    if( haveData( sizeof( float ) ) )
    {
        float* data = ( float* )( &m_BlobData[ m_DataIdx ] );
        if( m_UseNetworkOrder )
        {
            f32Value = ntohl( *data );
        }
        else
        {
            f32Value = *data;
        }

        incDataRead( sizeof( float ) );
        return true;
    }

    return false;
}


//============================================================================
bool BinaryBlob::getValue( double& f64Value )
{
    if( haveData( sizeof( double ) ) )
    {
        double* data = ( double* )( &m_BlobData[ m_DataIdx ] );
        if( m_UseNetworkOrder )
        {
            f64Value = ntohU64( *data );
        }
        else
        {
            f64Value = *data;
        }

        incDataRead( sizeof( double ) );
        return true;
    }

    return false;
}

//============================================================================
bool BinaryBlob::getValue( std::string& strValue )
{
    if( haveData( sizeof( uint32_t ) ) )
    {
        uint32_t dataLen = 0;
        getValue( dataLen );
        if( dataLen )
        {
            if( haveData( dataLen ) )
            {
                const char* data = ( const char* )( &m_BlobData[ m_DataIdx ] );
                strValue = data;
                incDataRead( dataLen );
                if( dataLen == ( strValue.length() + 1 ) )
                {
                    return true;
                }
                else
                {
                    LogMsg( LOG_ERROR, "BinaryBlob::getValue string length did not match" );
                }
            }
        }
        else
        {
            strValue = "";
            return true;
        }
    }

    return false;
}

//============================================================================
bool BinaryBlob::getValue( char * pRetBuf, int& iBufLen )
{
    if( haveData( sizeof( uint32_t ) ) )
    {
        uint32_t dataLen = 0;
        getValue( dataLen );
        if( dataLen )
        {
            if( haveData( dataLen ) )
            {
                const char* data = ( const char* )( &m_BlobData[ m_DataIdx ] );
                memcpy( pRetBuf, data, dataLen );
                incDataRead( dataLen );
                return true;
            }
        }
        else
        {
            *pRetBuf = 0;
            iBufLen = 1;
            return true;
        }
    }

    return false;
}

//============================================================================
bool BinaryBlob::getValue( std::vector<std::string>& aoStrValues )
{
    aoStrValues.clear();
    uint32_t strCnt;
    bool status = getValue( strCnt );
    if( status )
    {
        for( uint32_t i = 0; i < strCnt; i++ )
        {
            std::string strVal;
            status &= getValue( strVal );

            if( !status )
            {
                break;
            }
            else
            {
                aoStrValues.push_back( strVal );
            }
        }
    }

    return status;
}

//============================================================================
bool BinaryBlob::getValue( void* pvRetBuf, int& iBufLen )
{
    if( haveData( sizeof( uint32_t ) ) )
    {
        uint32_t dataLen = 0;
        getValue( dataLen );
        if( haveData( dataLen ) )
        {
            
            uint8_t* data = ( uint8_t* )( &m_BlobData[ m_DataIdx ] );
            memcpy( pvRetBuf, data, dataLen );
            iBufLen = ( int )dataLen;
            incDataRead( dataLen );
            return true;
        }
    }

    return false;
}
//============================================================================
bool BinaryBlob::getValue( VxGUID& guid )
{
    uint64_t loPart;
    uint64_t hiPart;
    bool result = getValue( loPart );
    result &= getValue( hiPart );
    if( result )
    {
        guid.setVxGUID( hiPart, loPart );
    }

    return result;
}

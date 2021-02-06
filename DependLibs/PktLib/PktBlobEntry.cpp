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

#include "PktBlobEntry.h"
#include <CoreLib/IsBigEndianCpu.h>
#include <CoreLib/VxGUID.h>
#include <CoreLib/VxDebug.h>
#include <memory.h>

//============================================================================
bool PktBlobEntry::setBlobData( uint8_t* blob, int len )
{
    if( 0 == len || !blob )
    {
        LogMsg( LOG_ERROR, "PktBlobEntry setBlobData invalid param" );
    }
    else if( len <= PKT_BLOB_MAX_STORAGE_LEN )
    {
        setMaxBlobLen( len ); 
        setBlobLen( len ); 
        memcpy( m_BlobData, blob, len );
        return true;
    }
    else
    {
        LogMsg( LOG_ERROR, "PktBlobEntry setBlobData too large" );
    }

    return false;
}

//============================================================================
void PktBlobEntry::setBlobLen( int len )
{
    m_BlobLen = m_UseNetworkOrder ? htons( (int16_t)len ) : (int16_t)len;
}

//============================================================================
int16_t PktBlobEntry::getBlobLen( void ) const        
{ 
    return m_UseNetworkOrder ? htons(m_BlobLen) : m_BlobLen; 
}

//============================================================================
void PktBlobEntry::setMaxBlobLen( int len )
{
    m_MaxBlobLen = m_UseNetworkOrder ? htons( (int16_t)len ) : (int16_t)len;
}

//============================================================================
int16_t PktBlobEntry::getMaxBlobLen( void ) const        
{ 
    return m_UseNetworkOrder ? htons(m_MaxBlobLen) : m_BlobLen; 
}

//============================================================================
void PktBlobEntry::setDataIdx( int len )
{
    m_DataIdx = m_UseNetworkOrder ? htons( (int16_t)len ) : (int16_t)len;
}

//============================================================================
int16_t PktBlobEntry::getDataIdx( void ) const        
{ 
    return m_UseNetworkOrder ? htons(m_DataIdx) : m_BlobLen; 
}

//============================================================================
bool PktBlobEntry::incDataWrite( int valSize )
{
    if( haveRoom( valSize ) )
    {
        setDataIdx( getDataIdx() + valSize );
        if( getDataIdx() > getBlobLen() )
        {
            setBlobLen( getDataIdx() );
        }

        return true;
    }
    else
    {
        LogMsg( LOG_ERROR, "PktBlobEntry Write past end" );
        m_PastEnd = true;
        return false;
    }
}

//============================================================================
bool PktBlobEntry::incDataRead( int valSize )
{
    if( haveData( valSize ) )
    {
        setDataIdx( getDataIdx() + valSize );
        return true;
    }
    else
    {
        LogMsg( LOG_ERROR, "PktBlobEntry Read past end" );
        m_PastEnd = true;
        return false;
    }
}

//============================================================================
bool PktBlobEntry::setValue( bool& bValue )
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
bool PktBlobEntry::setValue( int8_t& s8Value )
{
    bool result = false;
    if( haveRoom( sizeof( int8_t ) ) )
    {
        m_BlobData[ getDataIdx() ] = s8Value;
        incDataWrite( 1 );
        result = true;
    }

    return result;
}

//============================================================================
bool PktBlobEntry::setValue( uint8_t& u8Value )
{
    bool result = false;
    if( haveRoom( sizeof( uint8_t ) ) )
    {
        m_BlobData[ getDataIdx() ] = u8Value;
        incDataWrite( 1 );
        result = true;
    }

    return result;
}

//============================================================================
bool PktBlobEntry::setValue( int16_t& s16Value )
{
    bool result = false;
    if( haveRoom( sizeof( int16_t ) ) )
    {
        int16_t* data = ( int16_t* )(&m_BlobData[ getDataIdx() ]);
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
bool PktBlobEntry::setValue( uint16_t& u16Value )
{
    bool result = false;
    if( haveRoom( sizeof( uint16_t ) ) )
    {
        uint16_t* data = ( uint16_t* )( &m_BlobData[ getDataIdx() ] );
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
bool PktBlobEntry::setValue( int32_t& s32Value )
{
    bool result = false;
    if( haveRoom( sizeof( int32_t ) ) )
    {
        int32_t* data = ( int32_t* )( &m_BlobData[ getDataIdx() ] );
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
bool PktBlobEntry::setValue( uint32_t& u32Value )
{
    bool result = false;
    if( haveRoom( sizeof( uint32_t ) ) )
    {
        uint32_t* data = ( uint32_t* )( &m_BlobData[ getDataIdx() ] );
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
bool PktBlobEntry::setValue( int64_t& s64Value )
{
    bool result = false;
    if( haveRoom( sizeof( int64_t ) ) )
    {
        int64_t* data = ( int64_t* )( &m_BlobData[ getDataIdx() ] );
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
bool PktBlobEntry::setValue( uint64_t& u64Value )
{
    bool result = false;
    if( haveRoom( sizeof( uint64_t ) ) )
    {
        uint64_t* data = ( uint64_t* )( &m_BlobData[ getDataIdx() ] );
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
bool PktBlobEntry::setValue( float& f32Value )
{
    bool result = false;
    if( haveRoom( sizeof( float ) ) )
    {
        float* data = ( float* )( &m_BlobData[ getDataIdx() ] );
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
bool PktBlobEntry::setValue( double& f64Value )
{
    bool result = false;
    if( haveRoom( sizeof( double ) ) )
    {
        double* data = ( double* )( &m_BlobData[ getDataIdx() ] );
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
bool PktBlobEntry::setValue( std::string& strValue )
{
    return setValue( strValue.c_str(), strValue.length() + 1 );
}

//============================================================================
bool PktBlobEntry::setValue( std::vector<std::string>& aoStrValues )
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
bool PktBlobEntry::setValue( const char * pRetBuf, int iBufLen )
{
    return setValue( ( void* )pRetBuf, iBufLen );
}

//============================================================================
bool PktBlobEntry::setValue( void * pvRetBuf, int iBufLen )
{
    if( haveRoom( iBufLen + sizeof( uint32_t ) ) )
    {
        if( pvRetBuf )
        {
            uint32_t u32Len = ( uint32_t )iBufLen;
            if( setValue( u32Len ) )
            {
                uint8_t* data = ( uint8_t* )( &m_BlobData[ getDataIdx() ] );
                memcpy( data, pvRetBuf, iBufLen );
                incDataWrite( iBufLen );
            }

            return true;
        }
    }

    return false;
}

//============================================================================
bool PktBlobEntry::setValue( VxGUID& guid )
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
bool PktBlobEntry::getValue( bool& bValue )
{
    if( haveData( 1 ) )
    {
        bValue = m_BlobData[ getDataIdx() ] ? true : false;
        incDataRead( 1 );
        return true;
    }

    return false;
}

//============================================================================
bool PktBlobEntry::getValue( int8_t& s8Value )
{
    if( haveData( 1 ) )
    {
        s8Value = m_BlobData[ getDataIdx() ];
        incDataRead( 1 );
        return true;
    }

    return false;
}

//============================================================================
bool PktBlobEntry::getValue( uint8_t& u8Value )
{
    if( haveData( 1 ) )
    {
        u8Value = m_BlobData[ getDataIdx() ];
        incDataRead( 1 );
        return true;
    }

    return false;
}

//============================================================================
bool PktBlobEntry::getValue( int16_t& s16Value )
{
    if( haveData( sizeof( int16_t ) ) )
    {
        int16_t* data = ( int16_t* )( &m_BlobData[ getDataIdx() ] );
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
bool PktBlobEntry::getValue( uint16_t& u16Value )
{
    if( haveData( sizeof( uint16_t ) ) )
    {
        uint16_t* data = ( uint16_t* )( &m_BlobData[ getDataIdx() ] );
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
bool PktBlobEntry::getValue( int32_t& s32Value )
{
    if( haveData( sizeof( int32_t ) ) )
    {
        int32_t* data = ( int32_t* )( &m_BlobData[ getDataIdx() ] );
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
bool PktBlobEntry::getValue( uint32_t& u32Value )
{
    if( haveData( sizeof( uint32_t ) ) )
    {
        uint32_t* data = ( uint32_t* )( &m_BlobData[ getDataIdx() ] );
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
bool PktBlobEntry::getValue( int64_t& s64Value )
{
    if( haveData( sizeof( int64_t ) ) )
    {
        int64_t* data = ( int64_t* )( &m_BlobData[ getDataIdx() ] );
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
bool PktBlobEntry::getValue( uint64_t& u64Value )
{
    if( haveData( sizeof( uint64_t ) ) )
    {
        uint64_t* data = ( uint64_t* )( &m_BlobData[ getDataIdx() ] );
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
bool PktBlobEntry::getValue( float& f32Value )
{
    if( haveData( sizeof( float ) ) )
    {
        float* data = ( float* )( &m_BlobData[ getDataIdx() ] );
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
bool PktBlobEntry::getValue( double& f64Value )
{
    if( haveData( sizeof( double ) ) )
    {
        double* data = ( double* )( &m_BlobData[ getDataIdx() ] );
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
bool PktBlobEntry::getValue( std::string& strValue )
{
    if( haveData( sizeof( uint32_t ) ) )
    {
        uint32_t dataLen = 0;
        getValue( dataLen );
        if( dataLen )
        {
            if( haveData( dataLen ) )
            {
                const char* data = ( const char* )( &m_BlobData[ getDataIdx() ] );
                strValue = data;
                incDataRead( dataLen );
                if( dataLen == ( strValue.length() + 1 ) )
                {
                    return true;
                }
                else
                {
                    LogMsg( LOG_ERROR, "PktBlobEntry::getValue string length did not match" );
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
bool PktBlobEntry::getValue( char * pRetBuf, int& iBufLen )
{
    if( haveData( sizeof( uint32_t ) ) )
    {
        uint32_t dataLen = 0;
        getValue( dataLen );
        if( dataLen )
        {
            if( haveData( dataLen ) )
            {
                const char* data = ( const char* )( &m_BlobData[ getDataIdx() ] );
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
bool PktBlobEntry::getValue( std::vector<std::string>& aoStrValues )
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
bool PktBlobEntry::getValue( void* pvRetBuf, int& iBufLen )
{
    if( haveData( sizeof( uint32_t ) ) )
    {
        uint32_t dataLen = 0;
        getValue( dataLen );
        if( haveData( dataLen ) )
        {
            
            uint8_t* data = ( uint8_t* )( &m_BlobData[ getDataIdx() ] );
            memcpy( pvRetBuf, data, dataLen );
            iBufLen = ( int )dataLen;
            incDataRead( dataLen );
            return true;
        }
    }

    return false;
}
//============================================================================
bool PktBlobEntry::getValue( VxGUID& guid )
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
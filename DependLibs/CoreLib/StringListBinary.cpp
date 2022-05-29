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
// http://www.nolimitconnect.org
//============================================================================

#include "StringListBinary.h"
#include "IsBigEndianCpu.h"
#include "VxDebug.h"
#include <string.h>

//============================================================================
StringListBinary::StringListBinary()
{
    initStringStorage();
}

//============================================================================
bool StringListBinary::initStringStorage( void )
{
    m_StringsBinaryLen = (uint16_t *)&m_StringData[ 0 ];
    m_StringCount = (uint16_t *)(&m_StringData[ 2 ]);
    m_StorageVersion = &m_StringData[ 4 ];
    m_StorageReserved1 = &m_StringData[ 5 ];

    *m_StorageVersion = STRING_LIST_BINARY_VERSION;
    *m_StorageReserved1 = 0;

    m_StringDataPtr = &m_StringData[ STRING_LIST_BINARY_HDR_SIZE ];
    setStringStorgeLength( 0 );
    setStringCount( 0 );
    return true;
}

//============================================================================
void StringListBinary::setStringStorgeLength( int strStoreLen )                
{ 
    *m_StringsBinaryLen = htons( ( uint16_t )strStoreLen + STRING_LIST_BINARY_HDR_SIZE ); 
}

//============================================================================
uint16_t StringListBinary::getStringStorgeLength( void )                           
{ 
    return ntohs( *m_StringsBinaryLen ); 
}

//============================================================================
void StringListBinary::setStringCount( int strCnt )                            
{ 
    *m_StringCount = htons( ( uint16_t )strCnt ); 
}

//============================================================================
uint16_t StringListBinary::getStringCount( void )                                  
{ 
    return  ntohs( *m_StringCount ); 
}

//============================================================================
bool StringListBinary::isStringStorageValid( void )
{
    return ( *m_StorageVersion == STRING_LIST_BINARY_VERSION )
            && ( getStringCount() <= MAX_STRINGLIST_STRING_STORAGE )
            && ( *m_StorageReserved1 == 0 )
            && ( getStringStorgeLength() <= MAX_STRINGLIST_TOTAL_STORAGE )
            && ( getStringStorgeLength() >= STRING_LIST_BINARY_HDR_SIZE );
}

//============================================================================
bool StringListBinary::addStrings( std::vector<std::string>& stringList )
{
    bool result = true;
    setStringStorgeLength( 0 );
    setStringCount( 0 );
    int strStorageLen = 0;
    char * ptrStorage = getStringStorage();
    for( std::string& str : stringList )
    {
        int strLen = str.length();
        if( strStorageLen + strLen <= MAX_STRINGLIST_STRING_STORAGE )
        {
            strcpy( ptrStorage, str.c_str() );
            ptrStorage += ( strLen + 1 );
            strStorageLen += ( strLen + 1 );
        }
        else
        {
            LogMsg( LOG_WARNING, "StringListBinary::addStrings not enough storage size" );
            result = false;
            break;
        }
    }

    if( result )
    {
        setStringStorgeLength( strStorageLen );
        setStringCount( stringList.size() );
    }

    return result;
}

//============================================================================
bool StringListBinary::getStrings( std::vector<std::string>& stringList )
{
    bool result = ( STRING_LIST_BINARY_VERSION == *m_StorageVersion );
    if( result )
    {
        if( getStringStorgeLength() <= MAX_STRINGLIST_STRING_STORAGE )
        {
            if( getStringCount() <= MAX_STRINGLIST_STRING_STORAGE )
            {
                stringList.clear();
                int strStorageLen = 0;
                char * ptrStorage = getStringStorage();
                bool extracted = true;
                for( int i = 0; i < getStringCount(); i++ )
                {
                    std::string str = ptrStorage;
                    if( strStorageLen <= MAX_STRINGLIST_STRING_STORAGE )
                    {
                        stringList.push_back( str );
                        strStorageLen += str.length() + 1;
                    }
                    else
                    {
                        LogMsg( LOG_WARNING, "StringListBinary::getStrings corrupted string count" );
                        result = false;
                        break;
                    }

                    ptrStorage += str.length() + 1;
                }
            }
            else
            {
                LogMsg( LOG_WARNING, "StringListBinary::getStrings corrupted string count" );
                result = false;
            }
        }
        else
        {
            LogMsg( LOG_WARNING, "StringListBinary::getStrings corrupted length" );
            result = false;
        }
    }
    else
    {
        LogMsg( LOG_WARNING, "StringListBinary::getStrings corrupted list" );
    }


    return result;
}

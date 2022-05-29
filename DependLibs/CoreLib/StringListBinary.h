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
#pragma once

#include <vector>
#include <string>

#define MAX_STRINGLIST_TOTAL_STORAGE 14000 // maximum number of bytes for string storage
#define STRING_LIST_BINARY_VERSION 1
#define STRING_LIST_BINARY_HDR_SIZE 6
#define MAX_STRINGLIST_STRING_STORAGE ( MAX_STRINGLIST_TOTAL_STORAGE - STRING_LIST_BINARY_HDR_SIZE )

class StringListBinary
{
public:
    StringListBinary();

    bool                        initStringStorage( void );
    bool                        isStringStorageValid( void );

    uint8_t *                   getStorageData( void )                                  { return m_StringData; }
    void                        setStringStorgeLength( int strStoreLen );
    uint16_t                    getStringStorgeLength( void );

    void                        setStringCount( int strCnt );
    uint16_t                    getStringCount( void );

    bool                        addStrings( std::vector<std::string>& stringList );
    bool                        getStrings( std::vector<std::string>& stringList );

protected:
    char *                      getStringStorage( void )                                  { return ( char *)m_StringData; }

    uint16_t *                  m_StringsBinaryLen = nullptr;
    uint16_t *                  m_StringCount = nullptr;
    uint8_t *                   m_StorageVersion = nullptr;
    uint8_t *                   m_StorageReserved1 = nullptr;

    uint8_t *                   m_StringDataPtr = nullptr;

    uint8_t                     m_StringData[ MAX_STRINGLIST_TOTAL_STORAGE ];
};
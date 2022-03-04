//============================================================================
// Copyright (C) 2022 Brett R. Jones 
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

#include "Sha1Info.h"
#include "VxFileUtil.h"

//============================================================================
Sha1Info::Sha1Info( VxGUID& assetId, std::string& fileName )
    : m_Sha1Hash()
    , m_AssetId( assetId )
    , m_FileName( fileName )
{
   m_FileLen = VxFileUtil::fileExists( fileName.c_str() );
}

//============================================================================
Sha1Info::Sha1Info( const Sha1Info& rhs )
    : m_Sha1Hash( rhs.m_Sha1Hash )
    , m_AssetId( rhs.m_AssetId )
    , m_FileLen( rhs.m_FileLen )
    , m_FileName( rhs.m_FileName )
{
}

//============================================================================
Sha1Info& Sha1Info::operator = ( const Sha1Info& rhs )
{
    if( this != &rhs )
    {
        m_Sha1Hash = rhs.m_Sha1Hash;
        m_AssetId = rhs.m_AssetId;
        m_FileLen = rhs.m_FileLen;
        m_FileName = rhs.m_FileName;
    }

    return *this;
}

//============================================================================
bool Sha1Info::operator == ( const Sha1Info& rhs ) const
{
    return m_Sha1Hash == rhs.m_Sha1Hash && m_AssetId == rhs.m_AssetId && m_FileName == rhs.m_FileName && m_FileLen == rhs.m_FileLen;
}

//============================================================================
bool Sha1Info::isValid( void )
{
    return m_AssetId.isVxGUIDValid() && m_FileLen && m_Sha1Hash.isHashValid() && !m_FileName.empty();
}

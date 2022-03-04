#pragma once
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

#include <CoreLib/VxSha1Hash.h>
#include <CoreLib/VxGUID.h>

#include <string>

class Sha1Info
{
public:
	Sha1Info() = default;
	Sha1Info( VxGUID& assetId, std::string& fileName );
    Sha1Info( const Sha1Info& rhs );
	~Sha1Info() = default;

    Sha1Info&                   operator = ( const Sha1Info& rhs );
    bool                        operator == ( const Sha1Info& rhs ) const;

	bool						isValid( void );

	void						setSha1Hash( VxSha1Hash& sha1Hash )			{m_Sha1Hash = sha1Hash; }
	VxSha1Hash&					getSha1Hash( void )							{ return m_Sha1Hash; }

	void						setAssetId( VxGUID& assetId )				{ m_AssetId = assetId; }
	VxGUID&						getAssetId( void )							{ return m_AssetId; }

	void						setFileLen( int64_t fileLen )				{ m_FileLen = fileLen; }
	int64_t						getFileLen( void )							{ return m_FileLen; }

	void						setFileName( std::string& fileName )		{ m_FileName = fileName; }
	std::string&				getFileName( void )							{ return m_FileName; }


protected:
	VxSha1Hash					m_Sha1Hash;
	VxGUID						m_AssetId;
	uint64_t					m_FileLen{ 0 };
    std::string					m_FileName{ "" };
};



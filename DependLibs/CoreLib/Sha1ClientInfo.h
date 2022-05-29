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
// http://www.nolimitconnect.org
//============================================================================

#include <CoreLib/Sha1GeneratorCallback.h>
#include <CoreLib/Sha1Info.h>

class Sha1ClientInfo
{
public:
	Sha1ClientInfo() = default;
	Sha1ClientInfo( VxGUID& fileId, std::string& fileName, Sha1GeneratorCallback* client );
	Sha1ClientInfo( const Sha1Info& sha1Info, Sha1GeneratorCallback* client );
    Sha1ClientInfo( const Sha1ClientInfo& rhs );
	~Sha1ClientInfo() = default;

    Sha1ClientInfo&             operator = ( const Sha1ClientInfo& rhs );
    bool                        operator == ( const Sha1ClientInfo& rhs ) const;

	bool						isValid( void );

	void						setSha1Info( Sha1Info& sha1Info )			{ m_Sha1Info = sha1Info; }
	Sha1Info&					getSha1Info( void )							{ return m_Sha1Info; }

	void						setClient( Sha1GeneratorCallback* client )  { m_Client = client; }
	Sha1GeneratorCallback*		getClient( void )							{ return m_Client; }

	std::string&				getFileName( void )							{ return m_Sha1Info.getFileName(); }

protected:
	Sha1Info					m_Sha1Info;
	Sha1GeneratorCallback*		m_Client{ nullptr };
};



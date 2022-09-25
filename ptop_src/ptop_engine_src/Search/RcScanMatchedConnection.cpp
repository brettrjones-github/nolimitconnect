//============================================================================
// Copyright (C) 2016 Brett R. Jones 
// Issued to MIT style license by Brett R. Jones in 2017
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

#include "RcScanMatchedConnection.h"

#include <ptop_src/ptop_engine_src/Search/RcScan.h>


//============================================================================
RcScanMatchedConnection::RcScanMatchedConnection( VxNetIdent* netIdent, VxSktBase* sktBase )
: m_Ident( netIdent )
, m_Skt( sktBase )
, m_ActionStartTimeMs( 0 )
, m_u8JpgData( 0 )
, m_u32JpgDataLen( 0 )
{
}

//============================================================================
RcScanMatchedConnection::RcScanMatchedConnection( const RcScanMatchedConnection& rhs )
{
	*this = rhs;
}

//============================================================================
RcScanMatchedConnection& RcScanMatchedConnection::operator=( const RcScanMatchedConnection& rhs ) 
{	
	if( this != &rhs )
	{
		m_Ident					= rhs.m_Ident;
		m_Skt					= rhs.m_Skt;
		m_ActionStartTimeMs		= rhs.m_ActionStartTimeMs;
		m_u8JpgData				= rhs.m_u8JpgData;
		m_u32JpgDataLen			= rhs.m_u32JpgDataLen;
		m_ActionHadError		= rhs.m_ActionHadError;
		m_ActionComplete		= rhs.m_ActionComplete;
	}

	return *this;
}

//============================================================================
void RcScanMatchedConnection::deleteResources( void )
{
	delete m_u8JpgData;
	m_u8JpgData = 0;
}

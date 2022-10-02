#pragma once
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

#include <CoreLib/VxDefs.h>

class VxSktBase;
class VxNetIdent;

class RcScanMatchedConnection
{
public:
	RcScanMatchedConnection( VxNetIdent* netIdent, VxSktBase* sktBase );
	virtual ~RcScanMatchedConnection() = default;

	RcScanMatchedConnection( const RcScanMatchedConnection& rhs );
	RcScanMatchedConnection& operator=( const RcScanMatchedConnection& rhs );

	void						setActionStartTimeMs( int64_t actionStartTime )		{ m_ActionStartTimeMs = actionStartTime; }
	uint64_t					getActionStartTimeMs( void )						{ return m_ActionStartTimeMs; }

	void						setActionHadError( bool hadError )				{ m_ActionHadError = hadError; if( hadError) m_ActionComplete = true; }
	bool						getActionHadError( void )						{ return m_ActionHadError; }

	void						setIsActionCompleted( bool completed )			{ m_ActionComplete = completed; }
	bool						getIsActionCompleted( void )					{ return m_ActionComplete; }

	void						deleteResources( void );

	//=== vars ===//
	VxNetIdent*				m_Ident;
	VxSktBase *					m_Skt;
	uint64_t					m_ActionStartTimeMs;
	uint8_t *					m_u8JpgData;
	uint32_t					m_u32JpgDataLen;
	bool						m_ActionHadError;
	bool						m_ActionComplete;
};
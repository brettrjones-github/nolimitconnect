#pragma once
//============================================================================
// Copyright (C) 2013 Brett R. Jones
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
// http://www.nolimitconnect.com
//============================================================================

#include "VxConnectInfo.h"
#include "VxOnlineStatusFlags.h"

#pragma pack(push) 
#pragma pack(1)

//! implements minimum to uniquely identify user as individual on the network
//! size
//!   320 bytes VxConnectInfo
// +    1 byte  VxOnlineStatusFlags
// +    1 byte  m_IsOnline
// +    1 byte  m_u8OfferCnt
// +    1 byte  m_u8ReplyCnt
// +    4 bytes m_u32TruthCnt;		
// +    4 bytes m_u32DareCnt;					
// +    4 bytes m_u32RejectedCnt;
// =  352 bytes
class VxNetIdentBase : public VxConnectInfo, public VxOnlineStatusFlags
{
public:
	VxNetIdentBase() = default;
	VxNetIdentBase( const VxNetIdentBase &rhs );
    bool                        addToBlob( PktBlobEntry& blob );
    bool                        extractFromBlob( PktBlobEntry& blob );

	VxNetIdentBase&             operator =( const VxNetIdentBase &rhs );
	bool                        operator ==( const VxNetIdentBase &rhs ) const;
	bool                        operator != ( const VxNetIdentBase &rhs ) const;

	VxConnectInfo&				getConnectInfo( void )					{ return *this; }

    void						setIsOnline( bool isOnline )		    { m_IsOnline = (uint8_t)isOnline; }
    bool						isOnline( void )					    { return (bool)m_IsOnline;  }

    bool                        isMyself( void );
	bool						canDirectConnectToUser( void );

	void						setTruthCount( uint32_t truthCnt );
	uint32_t					getTruthCount( void );
	void						setDareCount( uint32_t dareCnt );
	uint32_t					getDareCount( void );
	void						setRejectedTruthCount( uint16_t rejectCnt );
	uint16_t					getRejectedTruthCount( void );
	void						setRejectedDareCount( uint16_t rejectCnt );
	uint16_t					getRejectedDareCount( void );

	bool						isVxNetIdentMatch( const VxNetIdentBase& oOtherIdent ) const;

protected:
	//=== vars ===//
    uint8_t						m_IsOnline{ 0 };
	uint8_t						m_u8OfferCnt{ 0 };							// offer count ( used as part of rating )
	uint8_t						m_u8ReplyCnt{ 0 };					        // reply count ( used as part of rating )
	uint32_t					m_u32TruthCnt{ 0 };					
	uint32_t					m_u32DareCnt{ 0 };					
	uint16_t					m_u16RejectedTruthsCnt{ 0 };				
	uint16_t					m_u16RejectedDaresCnt{ 0 };	
};

#pragma pack()
#pragma pack(pop)

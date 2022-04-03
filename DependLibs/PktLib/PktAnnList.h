#pragma once
//============================================================================
// Copyright (C) 2003 Brett R. Jones 
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

class VxNetIdent;

#include "VxPktHdr.h"

#pragma pack(push)
#pragma pack(1)
#define MAX_PKT_ANN_LIST_LEN 4080

class PktAnnList : public VxPktHdr
{
public:
	PktAnnList();

	int							emptyLen( void );
	void						calcPktLen( void );
	int						    addAnn( VxNetIdent * pgPktAnn );

private:
	//=== vars ===//
	uint16_t					m_u16ListCnt;
	uint16_t					m_u16Flags;
	uint32_t					m_u16Reason;
	uint32_t					m_u16Res1;
	uint32_t					m_u32Res2;
	uint8_t						m_au8List[ MAX_PKT_ANN_LIST_LEN ];
};

#pragma pack(pop)

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
// http://www.nolimitconnect.org
//============================================================================

class RcScanPic
{
public:
	//=== vars ===//
	VxNetIdent*		m_Ident; 
	VxSktBase *			m_Skt;
	uint8_t *				m_u8JpgData;
	uint32_t					m_u32JpgDataLen;

	RcScanPic( VxNetIdent* netIdent, VxSktBase* sktBase, uint8_t * u8JpgData, uint32_t u32JpgDataLen )
		: m_Ident(netIdent)
		, m_Skt(sktBase)
		, m_u8JpgData(u8JpgData)
		, m_u32JpgDataLen(u32JpgDataLen)
	{
	}
};
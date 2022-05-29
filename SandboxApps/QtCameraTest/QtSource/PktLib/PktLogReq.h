#ifndef PKT_LOG_REQ_H
#define PKT_LOG_REQ_H

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
// http://www.nolimitconnect.org
//============================================================================

#include "PktTypes.h"

#pragma pack(1)

class PktLogReq : public VxPktHdr
{
public:
	PktLogReq();

	void						setEnable( bool enable );
	bool						isEnable( void );

private:
	uint32_t					m_u16Res = 0;								
	uint32_t					m_u32Enable = 0;								
};

#pragma pack()

#endif // PKT_LOG_REQ_H

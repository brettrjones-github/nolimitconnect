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

#define VX_DEFS 1

#include <CoreLib/config_corelib.h>

#pragma pack(push)
#pragma pack(1)

typedef union U128
{
	struct 
	{
		uint64_t  m_u64HiPart;
		uint64_t  m_u64LoPart;
	};
	unsigned char m_u128AsBytes[16];
} U128;

#pragma pack(pop)

// to avoid multiple defines and missing defines
//#define VXMIN(_a,_b)      ((_a)<(_b)?(_a):(_b))
//#define VXMAX(_a,_b)      ((_a)>(_b)?(_a):(_b))
#define VXCLAMP(_a,_b,_c) (VXMAX(_a,VXMIN(_b,_c)))

#define S16_MAXVAL		32767	
#define S16_MINVAL		-32768	
#define U16_MAXVAL		65535	
#define U16_MINVAL		0

#define S32_MAXVAL		2147483647	
#define S32_MINVAL		-2147483648	
#define U32_MAXVAL		4294967295	
#define U32_MINVAL		0	
#define S64_MAXVAL		9223372036854775807
#define S64_MINVAL		-9223372036854775808
#define U64_MAXVAL		18446744073709551615	
#define U64_MINVAL		0	



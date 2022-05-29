#pragma once
//============================================================================
// Copyright (C) 2014 Brett R. Jones 
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

#include "VxDefs.h"

extern unsigned char	aucCommonToChar[];
extern unsigned char	aucCharToCommon[];
extern uint16_t				aucCommonToWChar[];
extern uint16_t				aucWCharToCommon[];

void					VxMapStr( char * pStr );
void					VxUnMapStr( char * pStr );
void					VxMapStr( wchar_t * pStr );
void					VxUnMapStr( wchar_t * pStr );


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

#include <string>

//=== Chop utils for strings ===//
int VxCalcChoppedLen( const char * pStr );
int VxGetUnchoppedStrLen( unsigned char * pu8ChopedStr );
int VxChopStr( const char * pStr, unsigned char * pu8RetChoppedStr );
int VxChopStr( std::string &csStr, unsigned char * pu8RetChoppedStr );
void VxUnchopStr(  unsigned char * pu8ChoppedStr, char * pRetUnchoppedStr );
void VxUnchopStr(  unsigned char * pu8ChoppedStr, std::string &csRetStr );

int VxCalcChoppedLen( const wchar_t * pStr );
int VxGetUnchoppedStrLen( uint16_t * pu8ChopedStr );
int VxChopStr( const wchar_t * pStr, uint16_t * pu8RetChoppedStr );
int VxChopStr( std::wstring &csStr, uint16_t * pu8RetChoppedStr );
void VxUnchopStr(  uint16_t * pu8ChoppedStr, wchar_t * pRetUnchoppedStr );
void VxUnchopStr(  uint16_t * pu8ChoppedStr, std::wstring &csRetStr );

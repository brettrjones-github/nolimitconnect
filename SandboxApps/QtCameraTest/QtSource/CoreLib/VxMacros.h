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

#define VxAbs(x) ((x)<0 ? -(x) : (x)) // get absolute value

#ifndef ROUND_TO_4BYTE_BOUNDRY
#define ROUND_TO_4BYTE_BOUNDRY( a ) (( a + 3 ) & ~3 ) //round upto even 4 byte boundry
#endif //ROUND_TO_4BYTE_BOUNDRY
#ifndef ROUND_TO_8BYTE_BOUNDRY
#define ROUND_TO_8BYTE_BOUNDRY( a ) (( a + 7 ) & ~7 ) //round upto even 8 byte boundry
#endif //ROUND_TO_8BYTE_BOUNDRY
#ifndef ROUND_TO_16BYTE_BOUNDRY
#define ROUND_TO_16BYTE_BOUNDRY( a ) (( a + 15 ) & ~15 ) //round upto even 16 byte boundry
#endif //ROUND_TO_16BYTE_BOUNDRY


#pragma once
//============================================================================
// Copyright (C) 2018 Brett R. Jones 
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
#include "VxGroupDefs.h"

#pragma pack(1)

// do not add any virtual s so can be used in network packets directly
class VxGroupInfo
{
public:
    VxGroupInfo();  // default constructor
    VxGroupInfo( const VxGroupInfo& rhs ); // copy constructor
    VxGroupInfo( VxGroupInfo&& rhs ); // move constructor
    ~VxGroupInfo() = default; 

    VxGroupInfo& operator = ( const VxGroupInfo& rhs ); // copy assignment
    VxGroupInfo& operator = ( VxGroupInfo&& rhs ); // move assignment
};

#pragma pack()


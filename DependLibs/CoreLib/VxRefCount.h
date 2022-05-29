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
#include <CoreLib/config_corelib.h>

class VxRefCount
{
public:
    //=== vars ===//
    int m_iRefCnt;

    //============================================================================
    //=== constructor ===//
    VxRefCount()
        : m_iRefCnt(1)
    {
    }

    //============================================================================
    //=== destructor ===//
    ~VxRefCount()
    {
        m_iRefCnt--;
        //vx_assert( m_iRefCnt == 0 );
    }

    virtual int incRefCnt();

    virtual int decRefCnt();

    virtual int getRefCnt();
};

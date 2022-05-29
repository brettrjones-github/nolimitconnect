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
#include "config_corelib.h"

#include "VxRefCount.h"

//============================================================================
int VxRefCount::incRefCnt()
{
    m_iRefCnt++;
    return m_iRefCnt;
}

//============================================================================
int VxRefCount::decRefCnt()
{
    m_iRefCnt--;
    //vx_assert( m_iRefCnt >= 0 );
    return m_iRefCnt;
}

//============================================================================
int VxRefCount::getRefCnt()
{
    return m_iRefCnt;
}

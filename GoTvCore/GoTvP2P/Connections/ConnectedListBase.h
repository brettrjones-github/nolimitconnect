//============================================================================
// Copyright (C) 2021 Brett R. Jones 
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
#pragma once

#include <CoreLib/VxGUID.h>

#include <map>

class ConnectedInfo;
class VxSktBase;

class ConnectedListBase
{
public:
    ConnectedListBase() = default;
    virtual ~ConnectedListBase() = default;

#ifdef TARGET_OS_ANDROID
    std::map<VxGUID, ConnectedInfo *>&				getConnectedList( void )		{ return m_ConnectList; }
#else
    std::map<VxGUID, ConnectedInfo *, cmp_vxguid>&	getConnectedList( void )		{ return m_ConnectList; }
#endif


protected:
    //=== vars ===//
#ifdef TARGET_OS_ANDROID
    std::map<VxGUID, ConnectedInfo *> m_ConnectList;
    typedef std::map<VxGUID, ConnectedInfo *>::iterator ConnectListIter;
#else
    std::map<VxGUID, ConnectedInfo *, cmp_vxguid> m_ConnectList;
    typedef std::map<VxGUID, ConnectedInfo *, cmp_vxguid>::iterator ConnectListIter;
#endif
};

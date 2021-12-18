#pragma once
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

#include <config_appcorelibs.h>

#include <CoreLib/VxMutex.h>

class VxNetIdent;
class VxPktHdr;
class VxSktBase;

class BaseXferInterface
{
public:
    virtual VxMutex&            getAssetXferMutex( void ) = 0;
    virtual EPluginType         getPluginType( void ) = 0;
    virtual bool                txPacket( VxNetIdent* netIdent, VxSktBase* sktBase, VxPktHdr* pktHdr, bool bDisconnectAfterSend = false ) = 0;

};

class AutoXferLock
{
public:
    AutoXferLock( VxMutex& mutex ) 
        : m_Mutex(mutex)	
    { 
        m_Mutex.lock(); 
    }

    ~AutoXferLock()
    { 
        m_Mutex.unlock(); 
    }

    VxMutex&						m_Mutex;
};
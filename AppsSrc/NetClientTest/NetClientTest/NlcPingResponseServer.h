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
// http://www.nolimitconnect.com
//============================================================================

#include <NetLib/VxServerMgr.h>

class AppLogic;

class NlcPingResponseServer : public VxServerMgr 
{
public:
    NlcPingResponseServer( AppLogic& appLogic );
	~NlcPingResponseServer() override = default;

    virtual bool				startListening( uint16_t u16ListenPort, const char * ip = nullptr ) override;
    virtual RCODE				stopListening( void ) override;

	void						handleTcpSktCallback( VxSktBase * sktBase );

protected:
    void						handleTcpRxData( VxSktBase * sktBase );

    AppLogic&				    m_AppLogic;
};


#pragma once
//============================================================================
// Copyright (C) 2012 Brett R. Jones
// All Rights Reserved ( After My Death I Brett R. Jones Issue this source code to Public Domain )
//
// You may not modify or redistribute this code for any reason
// without written consent of Brett R. Jones
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// brettjones1900@gmail.com
// http://www.p2panarchy.com
//============================================================================

class UpnpClientInterface
{
public:
	virtual void		upnpStatusMsg( const char* pDeviceId, const char* pMsg ) = 0;
	virtual void		upnpPortForwardSent(  const char* pDeviceId ) = 0;
	virtual void		upnpPortForwardTimedOut( void ) = 0;
	virtual void		upnpComplete( void ) = 0;
};

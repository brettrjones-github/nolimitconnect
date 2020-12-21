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
// http://www.nolimitconnect.com
//============================================================================

#include "VxSktUtil.h"
#include <CoreLib/VxTime.h>


#include <CoreLib/VxDebug.h>
#include <CoreLib/VxGlobals.h>

#include <stdlib.h>

//#define VERIFY_PORT_LIST

// list of ports register with icann etc
static uint16_t g_au16AllocatedPorts[] =
{
#include "VxAnyRandomPort.h"
};


//Check known registered ports
static bool IsPortRegistered( uint16_t u16Port )
{
	int iRangeCnt = sizeof( g_au16AllocatedPorts )/4;
	for( int i = 2; i < iRangeCnt; i+=2 )
	{
		if( (u16Port > g_au16AllocatedPorts[i]) &&
			(u16Port < g_au16AllocatedPorts[i]) )
		{
			// it is a port used by some application or agency
			return true;
		}
	}
	return false;
}

#ifdef VERIFY_PORT_LIST
static bool VerifyPortList( void )
{
	if( sizeof( g_au16AllocatedPorts ) & 0x03 )
	{
		vx_assert( false );
	}
	uint16_t u16LastStartPort	= 0;
	uint16_t u16LastEndPort		= 0;
	bool bFirstRound = true;

	int iRangeCnt = sizeof( g_au16AllocatedPorts )/4;
	for( int i = 0; i < iRangeCnt; i+=2 )
	{
		if( g_au16AllocatedPorts[i] >  g_au16AllocatedPorts[i+1] )
		{
			return false;
		}
		if( bFirstRound )
		{
			u16LastStartPort = g_au16AllocatedPorts[i];
			u16LastEndPort = g_au16AllocatedPorts[i+1];
			bFirstRound = false;
		}
		else
		{
			if( (u16LastStartPort > g_au16AllocatedPorts[i]) ||
				(u16LastEndPort > g_au16AllocatedPorts[i]) )
			{
				return false;
			}
		}
	}
	return true;
}
#endif // VERIFY_PORT_LIST

//============================================================================
//! get a random tcp/ip port that is not in use
uint16_t VxGetRandomTcpPort( bool bAbove4000 )
{
	srand( GetGmtTimeMs() );
#ifdef VERIFY_PORT_LIST
	vx_assert( VerifyPortList() );
#endif // VERIFY_PORT_LIST
	// pick a random port
	uint16_t u16RandPort;

	for( int i = 0; i < 30000; i++ )
	{
		u16RandPort = (uint16_t)rand();
		if( 0 == u16RandPort )
		{
			// zero not allowed
			continue;
		}
		if( bAbove4000 )
		{
			if( u16RandPort < 4000 )
			{
				continue;
			}
		}
		// now see if it has been registered 
		if( IsPortRegistered( u16RandPort ) )
		{
			// port was registered with ICAN as use by some app or something
			continue;
		}

		if( VxIsIpPortInUse( u16RandPort, nullptr, true ))
		{
            // sleep a bit or the whole app will look like it is hung
            VxSleep( 100 );
			continue;
		}

		// success
		return u16RandPort;
	}

	// if we got here we have tried 30000 times and failed.. give up
	LogMsg( LOG_ERROR, "VxGetRandomTcpPort: could not find a port\n");
	return 0;
}

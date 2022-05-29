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

#include "PktDebugHelpers.h"
#include "VxPktHdr.h"

#include <CoreLib/VxDebug.h>

void DumpPkt( VxPktHdr * pktHdr )
{
	LogMsg( LOG_INFO, "PktDump len %d type %d ver %d\n",
			pktHdr->getPktLength(),
			pktHdr->getPktType(),
			pktHdr->getPktVersionNum() );

	if( pktHdr->getPktLength() > sizeof( VxPktHdr ) )
	{
		unsigned char * data = (unsigned char *)pktHdr;
		data += sizeof( VxPktHdr );

        HexDump( LOG_INFO, data, pktHdr->getPktLength() - sizeof( VxPktHdr ), 0, (char *)"VxPkt" );
	}
}

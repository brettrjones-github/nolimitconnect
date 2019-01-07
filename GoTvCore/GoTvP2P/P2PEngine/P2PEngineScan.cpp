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
// brett.jones@engineer.com
// http://www.gotvptop.net
//============================================================================
#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvCore/GoTvP2P/Search/RcScan.h>
#include <GoTvCore/GoTvP2P/Plugins/PluginWebServer.h>

//============================================================================
void P2PEngine::fromGuiStartScan( EScanType eScanType, uint8_t searchFlags, uint8_t fileTypeFlags, const char * pSearchPattern )
{
	m_RcScan.fromGuiStartScan( eScanType, searchFlags, fileTypeFlags, pSearchPattern );
}

//============================================================================
void P2PEngine::fromGuiNextScan( EScanType eScanType )
{
	m_RcScan.fromGuiNextScan( eScanType );
}

//============================================================================
void P2PEngine::fromGuiStopScan( EScanType eScanType )
{
	m_RcScan.fromGuiStopScan( eScanType );
}

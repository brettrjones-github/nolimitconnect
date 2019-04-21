//============================================================================
// Copyright (C) 2015 Brett R. Jones 
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
#include <GoTvInterface/IToGui.h>

//============================================================================
void P2PEngine::callbackVideoJpgBig( void * /*userData*/, VxGUID& vidFeedId, uint8_t * jpgData, uint32_t jpgDataLen )
{
	IToGui::getToGui().toGuiPlayVideoFrame( vidFeedId, jpgData, jpgDataLen, 0 );
}

//============================================================================
void P2PEngine::callbackVideoJpgSmall( void * /*userData*/, VxGUID& vidFeedId, uint8_t * jpgData, uint32_t jpgDataLen, int motion0to100000 )
{
	IToGui::getToGui().toGuiPlayVideoFrame( vidFeedId, jpgData, jpgDataLen, motion0to100000 );
}

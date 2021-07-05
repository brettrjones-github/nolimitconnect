#pragma once
//============================================================================
// Copyright (C) 2016 Brett R. Jones
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

#include <CoreLib/VxFileUtil.h>
#include <NetLib/VxXferDefs.h>

class PluginBase;
class PluginMgr;
class IToGui;
class P2PEngine;
class VxSktBase;
class VxPktHdr;
class VxFileXferInfo;
class VxNetIdent;

class FileXferBaseMgr
{
public:
	FileXferBaseMgr( P2PEngine& engine, PluginBase& plugin );
	virtual ~FileXferBaseMgr() = default;

protected:
	EXferError					setupFileDownload( VxFileXferInfo& xferInfo );
	bool						makeIncompleteFileName( std::string& strRemoteFileName, std::string& strRetIncompleteFileName );
	EXferError					sendNextFileChunk( VxFileXferInfo& xxferInfo, VxNetIdent * netIdent, VxSktBase * skt );

	//=== vars ====//
    P2PEngine&					m_Engine;	
	PluginBase&					m_Plugin;				
	PluginMgr&					m_PluginMgr;
};

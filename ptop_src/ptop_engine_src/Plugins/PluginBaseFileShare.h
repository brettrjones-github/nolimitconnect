#pragma once
//============================================================================
// Copyright (C) 2010 Brett R. Jones
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


#include "PluginBaseFiles.h"
#include "PluginSessionMgr.h"
#include "FileInfoSharedFilesMgr.h"

#include <ptop_src/ptop_engine_src/P2PEngine/FileShareSettings.h>

#include <PktLib/VxCommon.h>

class PktFileListReply;
class FileShareSettings;
class FileInfo;
class P2PEngine;
class IToGui;
class VxNetIdent;
class FileTxSession;
class FileRxSession;
class VxFileShredder;

class PluginBaseFileShare : public PluginBaseFiles
{
public:
	PluginBaseFileShare( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent* myIdent, EPluginType pluginType,  std::string fileShareDbName );
	virtual ~PluginBaseFileShare() = default;


	virtual void				onSharedFilesUpdated( uint16_t u16FileTypes );

protected:
	FileInfoSharedFilesMgr		m_FileInfoSharedFilesMgr;
};



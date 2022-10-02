#pragma once
//============================================================================
// Copyright (C) 2022 Brett R. Jones
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

#include "FileInfoDb.h"
#include "FileInfoXferMgr.h"
#include "FileInfoBaseMgr.h"

#include <PktLib/VxCommon.h>

#include <CoreLib/VxThread.h>
#include <CoreLib/VxMutex.h>
#include <CoreLib/Sha1GeneratorCallback.h>

class FileInfo;
class IToGui;
class PluginBase;
class P2PEngine;
class PktFileListReply;
class PktFileInfoSearchReply;
class PktFileInfoMoreReply;
class SearchParams;
class SharedFilesMgr;
class VxSha1Hash;
class VxFileShredder;

class FileInfoSharedFilesMgr : public FileInfoDb, public FileInfoBaseMgr
{
public:
	FileInfoSharedFilesMgr() = delete;
	FileInfoSharedFilesMgr( const FileInfoSharedFilesMgr& rhs ) = delete;
	FileInfoSharedFilesMgr( P2PEngine& engine, PluginBase& plugin, std::string sharedFilesDbName );
	virtual ~FileInfoSharedFilesMgr();

	FileInfoSharedFilesMgr& operator=( const FileInfoSharedFilesMgr& rhs ) = delete;
};


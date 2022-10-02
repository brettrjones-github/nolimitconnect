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

class FileInfoLibraryMgr : public FileInfoDb, public FileInfoBaseMgr
{
public:
	FileInfoLibraryMgr() = delete;
	FileInfoLibraryMgr( const FileInfoLibraryMgr& rhs ) = delete;
	FileInfoLibraryMgr( P2PEngine& engine, PluginBase& plugin, std::string fileDbName );
	virtual ~FileInfoLibraryMgr();

	FileInfoLibraryMgr& operator=( const FileInfoLibraryMgr& rhs ) = delete;

};


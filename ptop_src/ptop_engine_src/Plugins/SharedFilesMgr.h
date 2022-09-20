#pragma once
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

#include "SharedFilesDb.h"

#include <PktLib/VxCommon.h>

#include <CoreLib/VxThread.h>
#include <CoreLib/VxMutex.h>

class SharedFileInfo;
class IToGui;
class PluginBaseFileShare;
class P2PEngine;
class PktFileListReply;
class VxSha1Hash;
class AssetMgr;

class SharedFilesMgr
{
public:
	SharedFilesMgr( P2PEngine& engine, PluginBaseFileShare& plugin, std::string fileShareDbName );
	virtual ~SharedFilesMgr();

	void						lockSharedFiles( void )				{ m_FilesListMutex.lock(); }
	void						unlockSharedFiles( void )			{ m_FilesListMutex.unlock(); }
	std::vector<SharedFileInfo*>& getSharedFilesList( void )		{ return m_SharedList; }

	void						lockFileListPackets( void )			{ m_PacketsMutex.lock(); }
	void						unlockFileListPackets( void )		{ m_PacketsMutex.unlock(); }
	std::vector<PktFileListReply*>& getFileListPackets( void )		{ return m_FileListPackets; }

	bool						isFileShared( std::string& fileName );
	bool						isFileShared( VxSha1Hash& fileHashId );
	bool						getFileFullName( VxSha1Hash& fileHashId, std::string& retFileFullName );
	bool						getFileHashId( std::string& fileFullName, VxSha1Hash& retFileHashId );

	virtual void				fromGuiUserLoggedOn( void );
	virtual bool				fromGuiSetFileIsShared( std::string fileName, bool shareFile, uint8_t * fileHashId = 0 );
	virtual bool				fromGuiGetSharedFiles( uint8_t fileTypeFilter );

	void						updateFileTypes( void );
	void						updateFileListPackets( void );

	void						sharedFilesShutdown( void );

	bool						isAllowedFileOrDir( std::string& strFileName );
	void						updateFilesListFromDb( VxThread * thread = 0 );

	void						addFileToGenHashQue( std::string fileName );
	void						generateHashIds( VxThread * thread = 0 );

	void						clearSharedDirList( void );
	void						clearSharedFileListPackets( void );

private:
	//=== vars ===//
    P2PEngine&					m_Engine;
	PluginBaseFileShare&		m_Plugin;

	int64_t						m_s64TotalByteCnt{ 0 };
	uint16_t					m_u16FileTypes{ 0 };
	VxMutex						m_FilesListMutex;

	std::vector<SharedFileInfo*>m_SharedList;
	SharedFilesDb				m_SharedFilesDb;
	VxThread					m_SharedFilesThread;

	std::vector<std::string>	m_GenHashList;
	VxMutex						m_GenHashMutex;
	VxThread					m_GenHashThread;


	std::vector<PktFileListReply*>m_FileListPackets;
	VxMutex						m_PacketsMutex;

	std::string					m_FileShareDbName;
};


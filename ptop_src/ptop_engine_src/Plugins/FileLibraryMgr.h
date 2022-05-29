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

#include "FileLibraryDb.h"

#include <PktLib/VxCommon.h>

#include <CoreLib/VxThread.h>
#include <CoreLib/VxMutex.h>

class LibraryFileInfo;
class IToGui;
class PluginBase;
class P2PEngine;
class PktFileListReply;
class SharedFilesMgr;
class VxSha1Hash;

class FileLibraryMgr : public VxThread
{
public:
	FileLibraryMgr( P2PEngine& engine, PluginBase& plugin, SharedFilesMgr& sharedFilesMgr, std::string fileLibraryDbName );
	virtual ~FileLibraryMgr();

	void						fileLibraryShutdown( void );

	void						lockFileLibrary( void )				{ m_FilesListMutex.lock(); }
	void						unlockFileLibrary( void )			{ m_FilesListMutex.unlock(); }
	std::vector<LibraryFileInfo*>& getFileLibraryList( void )		{ return m_LibraryFileList; }

	bool						isFileInLibrary( std::string& fileName );
	bool						isFileInLibrary( VxSha1Hash& fileHashId );
	bool						isFileInLibrary( VxGUID& assetId );
	// returns -1 if unknown else percent downloaded
	virtual int					fromGuiGetFileDownloadState( uint8_t * fileHashId );
	virtual bool				fromGuiAddFileToLibrary(	const char *	fileName, 
															bool			addFile, 
															uint8_t *			fileHashId = 0 );
	virtual void				fromGuiGetFileLibraryList( uint8_t fileTypeFilter );

	virtual void				fromGuiUserLoggedOn( void );

	void						addFileToLibrary(	std::string		fileName,
													uint64_t		fileLen, 
													uint8_t			fileType,
													VxSha1Hash&		fileHashId );
	void						removeFromLibrary( std::string& fileName );

	void						updateFileTypes( void );
	void						onFileLibraryUpdated( void );

	bool						isAllowedFileOrDir( std::string strFileName );
	void						updateFilesListFromDb( VxThread * thread = 0 );

	void						addFileToGenHashQue( std::string fileName );
	void						generateHashIds( VxThread * thread = 0 );

	void						clearLibraryFileList( void );

private:
	//=== vars ===//
    P2PEngine&					m_Engine;
	PluginBase&					m_Plugin;

	SharedFilesMgr&				m_SharedFilesMgr;

	int64_t						m_s64TotalByteCnt{ 0 };
	uint16_t					m_u16FileTypes{ 0 };
	VxMutex						m_FilesListMutex;

	std::vector<LibraryFileInfo*>m_LibraryFileList;
	FileLibraryDb				m_FileLibraryDb;
	std::vector<std::string>	m_GenHashList;
	VxMutex						m_GenHashMutex;
	VxThread					m_GenHashThread;
};


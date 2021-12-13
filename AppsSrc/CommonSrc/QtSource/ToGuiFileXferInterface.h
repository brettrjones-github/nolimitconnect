#pragma once
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
// bjones.engineer@gmail.com
// http://www.nolimitconnect.com
//============================================================================

#include <CoreLib/VxDefs.h>
#include <CoreLib/AssetDefs.h>
#include <PktLib/VxCommon.h>

#include <GuiInterface/IDefs.h>

#include <QString>

class VxGUID;
class VxNetIdent;
class GuiFileXferSession;
class FileListReplySession;
class VxMyFileInfo;

class ToGuiFileXferInterface 
{
public:
	virtual void				toGuiFileList( void * userData, VxMyFileInfo& fileInfo ){};
	virtual void				toGuiFileListReply(	void * userData, FileListReplySession * xferSession ){};

	virtual void				toGuiStartUpload( void * userData, GuiFileXferSession * xferSession ){};
	virtual void				toGuiFileUploadComplete( void * userData, VxGUID& lclSession, EXferError xferError ){};

	virtual void				toGuiStartDownload( void * userData, GuiFileXferSession * xferSession ){};
	virtual void				toGuiFileDownloadComplete( void * userData, VxGUID& lclSession, QString newFileName, EXferError xferError ){};

	virtual void				toGuiFileXferState( void * userData, VxGUID& lclSessionId, EXferState eXferState, int param1, int param2 ){};
};

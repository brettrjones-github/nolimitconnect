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

#include <QWidget> // must be declared first or linux Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

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
	virtual void				toGuiFileList( VxMyFileInfo& fileInfo ){};
	virtual void				toGuiFileListReply(	FileListReplySession * xferSession ){};

	virtual void				toGuiStartUpload( GuiFileXferSession * xferSession ){};
	virtual void				toGuiFileUploadComplete( EPluginType pluginType, VxGUID& lclSession, EXferError xferError ){};

	virtual void				toGuiStartDownload( GuiFileXferSession * xferSession ){};
	virtual void				toGuiFileDownloadComplete( EPluginType pluginType, VxGUID& lclSession, QString newFileName, EXferError xferError ){};

	virtual void				toGuiFileXferState( EPluginType pluginType, VxGUID& lclSessionId, EXferState eXferState, int param1, int param2 ){};
};

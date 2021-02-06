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

#include <config_gotvcore.h>

enum EBlobType
{
// the first 8 bits MUST match VxFileInfo 
//#define VXFILE_TYPE_PHOTO					0x01		
//#define VXFILE_TYPE_AUDIO					0x02		
//#define VXFILE_TYPE_VIDEO					0x04		
//#define VXFILE_TYPE_DOC					0x08
//#define VXFILE_TYPE_ARCHIVE_OR_CDIMAGE	0x10
//#define VXFILE_TYPE_EXECUTABLE			0x20	
//#define VXFILE_TYPE_OTHER					0x40
//#define VXFILE_TYPE_DIRECTORY				0x80

	eBlobTypeUnknown				= 0x00,
	eBlobTypePhoto					= 0x01,
	eBlobTypeAudio					= 0x02,
	eBlobTypeVideo					= 0x04,
	eBlobTypeDocument				= 0x08,
	eBlobTypeArchives				= 0x10,
	eBlobTypeExe					= 0x20,
	eBlobTypeOtherFiles			    = 0x40,
	eBlobTypeDirectory				= 0x80,
	// these are specific to my p2p web
	eBlobTypeChatText				= 0x100,
	eBlobTypeChatFace				= 0x200,
	eBlobTypeChatStockAvatar		= 0x400,
	eBlobTypeChatCustomAvatar		= 0x800,
    eBlobTypeThumbnail              = 0x1000,
    eBlobTypeCamRecord              = 0x2000
};

enum EBlobLocation
{
	eBlobLocUnknown				    = 0x00,
	eBlobLocLibrary				    = 0x01,
	eBlobLocShared					= 0x02,
	eBlobLocPersonalRec			    = 0x04,
    eBlobLocThumbDirectory         = 0x08,
    eBlobLocCamRecord              = 0x10
};

enum EBlobAction
{
	eBlobActionUnknown				= 0,
	eBlobActionDeleteFile			= 1,
	eBlobActionShreadFile			= 2,
	eBlobActionAddToBlobMgr		    = 3,
	eBlobActionRemoveFromBlobMgr	= 4,	
	eBlobActionUpdateBlob			= 5,
	eBlobActionAddBlobAndSend		= 6,
	eBlobActionBlobResend			= 7,

	eBlobActionAddToShare			= 8,
	eBlobActionRemoveFromShare		= 9,
	eBlobActionAddToLibrary		    = 10,
	eBlobActionRemoveFromLibrary	= 11,
	eBlobActionAddToHistory		    = 12,
	eBlobActionRemoveFromHistory	= 13,

	eBlobActionRecordBegin			= 14,
	eBlobActionRecordPause			= 15,
	eBlobActionRecordResume		    = 16,
	eBlobActionRecordProgress		= 17,
	eBlobActionRecordEnd			= 18,
	eBlobActionRecordCancel		    = 19,

	eBlobActionPlayBegin			= 20,
	eBlobActionPlayOneFrame		    = 21,
	eBlobActionPlayPause			= 22,
	eBlobActionPlayResume			= 23,
	eBlobActionPlayProgress		    = 24,
	eBlobActionPlayEnd				= 25,
	eBlobActionPlayCancel			= 26,

	eBlobActionTxBegin				= 27,
	eBlobActionTxProgress			= 28,
	eBlobActionTxSuccess			= 29,
	eBlobActionTxError				= 30,
	eBlobActionTxCancel			    = 31,
	eBlobActionTxPermission		    = 32,

	eBlobActionRxBegin				= 33,
	eBlobActionRxProgress			= 34,
	eBlobActionRxSuccess			= 35,
	eBlobActionRxError				= 36,
	eBlobActionRxCancel			    = 37,
	eBlobActionRxPermission		    = 38,

	eBlobActionRxNotifyNewMsg		= 39,
	eBlobActionRxViewingMsg		    = 40,
};

enum EBlobSendState
{
	eBlobSendStateNone     = 0,
	eBlobSendStateTxProgress,
	eBlobSendStateRxProgress,
	eBlobSendStateTxSuccess,
	eBlobSendStateTxFail,
	eBlobSendStateRxSuccess,
	eBlobSendStateRxFail,
	eBlobSendStateTxPermissionErr,
	eBlobSendStateRxPermissionErr,
	
	eMaxBlobSendState	
};


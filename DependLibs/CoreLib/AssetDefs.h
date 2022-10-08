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
// http://www.nolimitconnect.org
//============================================================================

#include <string>

enum EAssetType
{
	// these must exactly match file type definitions in <CoreLib/VxFileTypeMasks.h>
	eAssetTypeUnknown				= 0x00,
	eAssetTypePhoto					= 0x01,
	eAssetTypeAudio					= 0x02,
	eAssetTypeVideo					= 0x04,
	eAssetTypeDocument				= 0x08,
	eAssetTypeArchives				= 0x10,
	eAssetTypeExe					= 0x20,
	eAssetTypeOtherFiles			= 0x40,
	eAssetTypeDirectory				= 0x80,
	// these are specific to ptop web
    eAssetTypeThumbnail             = 0x1000,
    eAssetTypeChatText				= 0x2000,
    eAssetTypeChatFace				= 0x4000,
    eAssetTypeCamRecord             = 0x8000
};

enum EAssetLocation
{
	eAssetLocUnknown				= 0x00,
	eAssetLocLibrary				= 0x01,
	eAssetLocShared					= 0x02,
	eAssetLocPersonalRec			= 0x04,
    eAssetLocThumbDirectory         = 0x08,
    eAssetLocCamRecord              = 0x10
};

enum EAssetAction
{
	eAssetActionUnknown				= 0,
	eAssetActionDeleteFile			= 1,
	eAssetActionShreadFile			= 2,
	eAssetActionAddToAssetMgr		= 3,
	eAssetActionRemoveFromAssetMgr	= 4,	
	eAssetActionUpdateAsset			= 5,
	eAssetActionAddAssetAndSend		= 6,
	eAssetActionAssetResend			= 7,

	eAssetActionAddToShare			= 8,
	eAssetActionRemoveFromShare		= 9,
	eAssetActionAddToLibrary		= 10,
	eAssetActionRemoveFromLibrary	= 11,
	eAssetActionAddToHistory		= 12,
	eAssetActionRemoveFromHistory	= 13,

	eAssetActionRecordBegin			= 14,
	eAssetActionRecordPause			= 15,
	eAssetActionRecordResume		= 16,
	eAssetActionRecordProgress		= 17,
	eAssetActionRecordEnd			= 18,
	eAssetActionRecordCancel		= 19,

	eAssetActionPlayBegin			= 20,
	eAssetActionPlayOneFrame		= 21,
	eAssetActionPlayPause			= 22,
	eAssetActionPlayResume			= 23,
	eAssetActionPlayProgress		= 24,
	eAssetActionPlayEnd				= 25,
	eAssetActionPlayCancel			= 26,

	eAssetActionTxBegin				= 27,
	eAssetActionTxProgress			= 28,
	eAssetActionTxSuccess			= 29,
	eAssetActionTxError				= 30,
	eAssetActionTxCancel			= 31,
	eAssetActionTxPermission		= 32,

	eAssetActionRxBegin				= 33,
	eAssetActionRxProgress			= 34,
	eAssetActionRxSuccess			= 35,
	eAssetActionRxError				= 36,
	eAssetActionRxCancel			= 37,
	eAssetActionRxPermission		= 38,

	eAssetActionRxNotifyNewMsg		= 39,
	eAssetActionRxViewingMsg		= 40,
};

enum EAssetSendState
{
	eAssetSendStateNone     = 0,
	eAssetSendStateTxProgress,
	eAssetSendStateRxProgress,
	eAssetSendStateTxSuccess,
	eAssetSendStateTxFail,
	eAssetSendStateRxSuccess,
	eAssetSendStateRxFail,
	eAssetSendStateTxPermissionErr,
	eAssetSendStateRxPermissionErr,
	
	eMaxAssetSendState	
};

bool VxIsValidAssetType( enum EAssetType assetType );
EAssetType VxFileNameToAssetType( std::string fileName );
EAssetType VxFileTypeToAssetType( uint8_t fileType );

const char* DescribeAssetType( enum EAssetType assetType );

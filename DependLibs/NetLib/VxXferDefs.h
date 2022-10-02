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
// http://www.nolimitconnect.org
//============================================================================

enum EXferDirection
{
    eXferDirectionNone,
    eXferDirectionRx,
    eXferDirectionTx,

    eMaxXferDirection
};

enum EXferError
{
	eXferErrorNone				= 0,
	eXferErrorDisconnected,
	eXferErrorPermission,
	eXferErrorFileNotFound,
	eXferErrorCanceled,
	eXferErrorBadParam,
	eXferErrorAtSrc,
	eXferErrorBusy,
	eXferErrorAlreadyDownloading,
	eXferErrorAlreadyDownloaded,
	eXferErrorAlreadyUploading,
	eXferErrorFileCreateError,
	eXferErrorFileOpenAppendError,
	eXferErrorFileOpenError,
	eXferErrorFileSeekError,
	eXferErrorFileReadError,
	eXferErrorFileWriteError,
	eXferErrorFileMoveError,

	eMaxXferError
};

enum EXferState
{
    eXferStateUnknown,

    eXferStateUploadNotStarted,
    eXferStateWaitingOfferResponse,
    eXferStateInUploadQue,
    eXferStateBeginUpload,
    eXferStateInUploadXfer,
    eXferStateCompletedUpload,
    eXferStateUserCanceledUpload,
    eXferStateUploadOfferRejected,
    eXferStateUploadError,

    eXferStateDownloadNotStarted,
    eXferStateInDownloadQue,
    eXferStateBeginDownload,
    eXferStateInDownloadXfer,
    eXferStateCompletedDownload,
    eXferStateUserCanceledDownload,
    eXferStateDownloadError,

    eMaxXferState
};

const char* DescribeXferDirection( enum EXferDirection xferDir );
const char* DescribeXferError( enum EXferError xferErr );
const char* DescribeXferState( enum EXferState xferState );

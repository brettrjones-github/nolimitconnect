#pragma once
//============================================================================
// Copyright (C) 2003 Brett R. Jones 
// All Rights Reserved ( After My Death I Brett R. Jones Issue this source code to Public Domain )
//
// You may not modify or redistribute this code for any reason 
// without written consent of Brett R. Jones
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// brettjones1900@gmail.com
// http://www.p2panarchy.com
//============================================================================

#include <AppDependLibrariesConfig.h>

#include <CoreLib/VxMutex.h>
#include <CoreLib/VxThread.h>
#include <CoreLib/VxSemaphore.h>

#if defined(TARGET_OS_WINDOWS)
# include <vfw.h>

#include "IVxVidCap.h"

#include <string>
#include <vector>

#define MOTION_SAMPLE_CNT				4
#define VX_CUSTOM_START					WM_APP + 347


//only one format supported right now
#define VX_VIDCAP_FORMAT_RGB24			24

enum ECapImageFormat
{
	eCapImageFormatInvalid	= 0, // invalid image format
	eCapImageFormatJpg		= 1, // jpg image format
	eCapImageFormatBmp		= 2, // bmp image format
	eMaxCapImageFormat	// must be last
};

#define MAX_VFW_DEVICES    10

class VidCapSettings
{
public:
	VidCapSettings();

	void						SetVidCapDefaults( void );

	//=== vars ===//
	bool						m_bEnableCamServer;
	bool						m_bPreviewEnabled;
	ECapImageFormat				m_eImageFormat;
	int							m_iJpegQuality;
	int							m_iRefreshRate;
	bool						m_bInvertImage;
	bool						m_bSwapRgb;
	bool						m_bRealSizePreview;
	bool						m_bUseOverlay;
    uint32_t					m_u32OverlayTransColor;
	int							m_iMotionSnapshotInvervalMiliSec;
	int							m_iPreviewFramesPerSec;
	char						m_as8OverlayBmpFile[ MAX_PATH ];
	char						m_as8CaptionBmpFile[ MAX_PATH ];
};

class VxVidCapImp : public VidCapSettings
{
public:
	VxVidCapImp();
	virtual ~VxVidCapImp();

	VxVidCapImp(const VxVidCapImp &CopyFrom);
	VxVidCapImp &operator =(const VxVidCapImp &CopyFrom);

	RCODE						Initialize( int iVidSrcNum, int iDriverIdx, int iPreferedImageWidth = 320, int iPreferedImageHeight = 200 );
	void						Destroy();

	VIDCAP_HWND					GetCapWindow( void )	{ return m_hWndVideo; };
	BITMAPINFOHEADER			GetBitmapInfoHeader()	{ return m_BmpInfoHeader.bmiHeader; }

	void						LoadSettings( void );
	void						SaveSettings( void );

	//! take snapshot
	//! NOTE: if *ppoBitmap is null then function will allocate but caller must delete the returned bitmap
	bool						CaptureDIB(	PBITMAPINFO *	ppoBitmap,			// if contents null then will allocate
											uint32_t				u32BitmapLength,	// if contents ppoBitmap then length of preallocated bitmap
											uint32_t *			u32RetBitmapLength,	// if not null return total length of bitmap
											uint32_t *			u32RetFormat );
	bool						CaptureAVI(LPCTSTR Filename, FLOAT FramesPerSec, ULONG Duration, UINT Quality);

	//! preview will resize but keep aspect ratio
	bool						EnablePreviewVideo(VIDCAP_HWND Parent, INT x, INT y, INT iWidth, INT iHeight, INT iPreviewRate = 30);
	//! stop preview
	bool						DisablePreviewVideo( void );

	//=== user selection ===//
	//! show dialog for user to pick video source
	bool						DlgVideoSource( void );
	//! show dialog for user to choose video format
	bool						DlgVideoFormat( void );
	//! show dialog for user to adjust contrast, hue etc
	bool						DlgVideoDisplay( void );

	void						CancelCapture( void );

	bool						SetPreviewRate( int iFramesPerSec );
	//! get copy of last snapshot
	bool						GetLastMotionSnapshot(		BITMAPINFO **	ppgRetBitmap,			// return snapshot..NOTE: caller must delete it
															uint32_t &		u32RetBmpLen,			// return length of bitmap
															int64_t &		s64RetTimeOfSnapshot ); // return time snapshot was taken
	//! start thread to detect video motion
	void						StartMotionThread( void );
	//! stop thread that detects video motion
	void						StopMotionThread( void );

	//! stop thread that handles capture messages
	void						StopCaptureThread( void );

	bool						DriverGetCaps(CAPDRIVERCAPS *Caps);


	//=== vars ===//
	VIDCAP_HWND					m_hWndVideo;
	VxMutex						m_ImageProtect;
	VxSemaphore					m_ImageReady;
	VxSemaphore					m_CancelCapture;
	VxSemaphore					m_CaptureEnded;
	VxThread					m_CaptureThread;

	BITMAPINFO					m_BmpInfoHeader;      // Used to store image dimensions.

	PBITMAPINFO					m_TransferBitmapInfo;
	uint32_t							m_TransferBitmapInfoSize;

	INT							m_ErrorID;
	std::string					m_ErrorText;

	int							m_iPreviewFramesPerSec;
	CAPDRIVERCAPS				m_DriverCaps;

	long						tzDiff;
	long						m_s32InitErr;
	PBITMAPINFO					m_BitmapMotionOld;
	uint32_t							m_u32MotionBitmapLen;
	int64_t							m_s64TimeLastSnapshot;

	long						m_s32TimeZoneDiff;
	float						m_f32MotionSamples[ MOTION_SAMPLE_CNT ];
	int							m_iMotionSampleIdx;

	int							m_iVidSrcNum;
	int							m_iDriverIdx;
	char						as8DeviceName[80];
	char						as8DeviceVersion[80];
	uint32_t							m_u32FormatType;
	bool						m_bCapWindowDestroyed;
	bool						m_bCapWindowCreated;
	VxThread					m_MotionThread;

	int							m_iPreferedImageWidth;
	int							m_iPreferedImageHeight; 
	int							m_iCurrentImageWidth;
	int							m_iCurrentImageHeight; 

private:

	//#define SHOW_VID_LOCKS 1
	//! lock image access
	void						lock( int iInst )
	{ 
#ifdef SHOW_VID_LOCKS
		LogMsg( LOG_DEBUG,  "Locking VxVidCap Inst %d\n", iInst );
#endif // SHOW_VID_LOCKS
		m_ImageProtect.lock(); 
	}
	//! unlock image access
	void						unlock( int iInst )
	{ 
#ifdef SHOW_VID_LOCKS
		LogMsg( LOG_DEBUG,  "Unlocking VxVidCap Inst %d\n", iInst );
#endif // SHOW_VID_LOCKS
		m_ImageProtect.unlock(); 
	}

	bool						SetDriver(SHORT DriverIndex);

	bool						SetCaptureFormat( int iFormat, int iWidth, int iHeight );

	void						GetPreviousError(INT *ErrorID, std::string * ErrorString, bool ResetError = FALSE);

	bool						AllocDIBImage(PBITMAPINFO *ppImageData, ULONG *AllocatedSize);
	static ULONG				CalcBitmapSize(const BITMAPINFOHEADER &bmiHeader);
	static ULONG				CalcBitmapInfoSize(const BITMAPINFOHEADER &bmiHeader);
	//! calculate scale needed for capture image size to fit window
	float						CalculateCaptureScale( int iWndWidth, int iWndHeight );


	friend LRESULT CALLBACK		ErrorCallbackProc(HWND, int nErrID, LPSTR lpErrorText);
	friend LRESULT CALLBACK		ControlCallbackProc(HWND, int nState);
	friend LRESULT CALLBACK		StatusCallbackProc(HWND, int nID, LPCSTR lpsz);
	friend LRESULT CALLBACK		FrameCallbackProc(HWND, LPVIDEOHDR lpVHdr);
	friend LRESULT CALLBACK		StreamCallbackProc(HWND, LPVIDEOHDR lpVHdr);

	friend uint32_t					CaptureWindowThreadFunc( void * pvContext );
	//thread to detect motion
	friend uint32_t					CaptureMotionThreadFunc( void * pvContext );
};

#endif // defined(TARGET_OS_WINDOWS)




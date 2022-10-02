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

#ifdef TARGET_OS_WINDOWS
#include "VxVidCapImp.h"
#include "GdvBufferUtil.h"
#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxMacros.h>

#ifdef _MSC_VER
# pragma warning( disable: 4800 ) //forcing value to bool 'true' or 'false' (performance warning)
#endif // _MSC_VER

#define VX_VID_SOURCE      (WM_APP+1)	// message to show dialog for user to pick video source
#define VX_VID_FORMAT      (WM_APP+2)	// message to show dialog for user to choose video format
#define VX_VID_DISPLAY     (WM_APP+3)	// message to show dialog for user to adjust contrast, hue etc

//============================================================================
VidCapSettings::VidCapSettings()
{
	SetVidCapDefaults();
}

//============================================================================
//! set default values for video capture
void VidCapSettings::SetVidCapDefaults( void )
{
	m_bPreviewEnabled		= 0;
	m_bEnableCamServer		= 1;
	m_eImageFormat			= eCapImageFormatJpg;
	m_iJpegQuality			= 75;
	m_iRefreshRate			= 0;
	m_bInvertImage			= 0;
	m_bSwapRgb				= 0;
	m_bRealSizePreview		= 0;
	m_u32OverlayTransColor	= 0x00ff000000;
	m_as8OverlayBmpFile[ 0 ] = 0;
	m_as8CaptionBmpFile[ 0 ] = 0;
	m_iMotionSnapshotInvervalMiliSec = 66; //15 fps
	m_iPreviewFramesPerSec	= 15;
}

//============================================================================
VxVidCapImp::VxVidCapImp()
	: m_TransferBitmapInfo( NULL )
	, m_TransferBitmapInfoSize( 0 )
	, m_hWndVideo( NULL )
	, m_iMotionSampleIdx( 0 )
	, m_BitmapMotionOld( NULL )
	, m_u32MotionBitmapLen( 0 )
	, m_iDriverIdx( -1 )
	, m_iPreferedImageWidth(0)
	, m_iPreferedImageHeight(0)
	, m_iCurrentImageWidth(0)
	, m_iCurrentImageHeight(0)
{
	m_s64TimeLastSnapshot = GetGmtTimeMs();
	memset(&m_BmpInfoHeader,0,sizeof(m_BmpInfoHeader));
	memset( m_f32MotionSamples, 0, sizeof( m_f32MotionSamples ) );
}

//============================================================================
VxVidCapImp::~VxVidCapImp()
{
   Destroy();
}

//============================================================================
RCODE VxVidCapImp::Initialize( int iDriverIdx, int iVidSrcNum, int iPreferedImageWidth, int iPreferedImageHeight )
{
	bool bResult = 0;
	m_iDriverIdx = iDriverIdx;
	m_iVidSrcNum = iVidSrcNum;
	m_iPreferedImageWidth = iPreferedImageWidth;
	m_iPreferedImageHeight = iPreferedImageHeight;

	//Load();
//	Destroy();
	m_s32InitErr = VX_VIDCAP_NO_DEVICES;
	
	// Reset any error conditions.
	GetPreviousError(NULL,NULL,TRUE);
	
	// The image ready semaphore will temporarily be used to determine when the
	// capture thread is read
	
	// Create message pump for window messages.
	m_CaptureThread.startThread( ( VX_THREAD_FUNCTION_T )CaptureWindowThreadFunc, ( void *)this, "CaptureWindowThread" );
	
	// Wait for event to determine when capture thread is ready.
	m_ImageReady.wait();

	
	if( (HWND)m_hWndVideo )
	{
		capSetUserData( (HWND)m_hWndVideo, this );
		capSetCallbackOnError( (HWND)m_hWndVideo, ErrorCallbackProc );
		capSetCallbackOnCapControl( (HWND)m_hWndVideo, ControlCallbackProc );
		capSetCallbackOnStatus( (HWND)m_hWndVideo, StatusCallbackProc );
		capSetCallbackOnFrame( (HWND)m_hWndVideo, FrameCallbackProc );
		capSetCallbackOnVideoStream( (HWND)m_hWndVideo, StreamCallbackProc );
		
		// Construct list of valid video drivers.
		// This creates a contiguous virtual driver table.
		
		// Reset any error conditions.
		GetPreviousError(NULL,NULL,TRUE);
		bResult = SetDriver( m_iDriverIdx );
	}
	
	if( FALSE == bResult || NULL == m_hWndVideo )
	{
		//error setting up
		if( m_ErrorID == 0 )
		{
			m_ErrorID = DV_ERR_NONSPECIFIC;
		}

		Destroy();
		return VX_VIDCAP_DRIVER_ERR;
	}

	m_s32InitErr = 0;
	return 0;
}

//============================================================================
//! start thread to detect video motion
void VxVidCapImp::StartMotionThread( void )
{
	//start Motion thread
	if( false == m_MotionThread.isThreadCreated() )
	{
		m_MotionThread.startThread( (VX_THREAD_FUNCTION_T)CaptureMotionThreadFunc, this, "VidCapMotion" );
	}
	else
	{
		LogMsg( LOG_ERROR, "VxVidCapImp::StartMotionThread: Thread Still Running\n" );
	}
}

//============================================================================
void VxVidCapImp::Destroy()
{
	m_MotionThread.abortThreadRun( true );
	m_CaptureThread.abortThreadRun( true );
	m_ImageReady.signal();

	// Reset any error conditions.
	GetPreviousError(NULL,NULL,TRUE);
	
	if (m_hWndVideo)
	{
		DisablePreviewVideo();
		
		capCaptureAbort((HWND)m_hWndVideo);
		capSetCallbackOnError((HWND)m_hWndVideo,NULL);
		capSetCallbackOnCapControl((HWND)m_hWndVideo,NULL);
		capSetCallbackOnStatus((HWND)m_hWndVideo,NULL);
		capSetCallbackOnFrame((HWND)m_hWndVideo,NULL);
		capSetCallbackOnVideoStream((HWND)m_hWndVideo, NULL);
		
		capSetUserData((HWND)m_hWndVideo,NULL);
		capDriverDisconnect((HWND)m_hWndVideo);
		capCaptureStop((HWND)m_hWndVideo); 
		capCaptureAbort((HWND)m_hWndVideo);
		// send a quit message so thread that handles windows messages will exit
		PostMessage( (HWND)m_hWndVideo, WM_QUIT, 0, 0 );
	}

	StopCaptureThread();
	StopMotionThread();
	
	m_TransferBitmapInfo = NULL;
	m_TransferBitmapInfoSize = 0;
	m_hWndVideo = NULL;
	m_iDriverIdx = -1;
	
	memset(&m_BmpInfoHeader,0,sizeof(m_BmpInfoHeader));
	if( m_BitmapMotionOld )
	{
		delete m_BitmapMotionOld;	
		m_BitmapMotionOld = NULL;	
	}
}

/*******************************************************************************
Function   : CaptureDIB
Arguments  : Bitmap (output) - Pointer to bitmap to receive image.
If *Bitmap = NULL, then allocation will
be performed automatically.
BitmapLength (input) - Size of Bitmap if *Bitmap is not NULL.
RetBitmapLength (output) - Actual size of image.
Return     : TRUE Success, FALSE Failed.
Description: Captures a DIB image from video capture device.
*******************************************************************************/
bool VxVidCapImp::CaptureDIB(	PBITMAPINFO *	Bitmap,
								uint32_t				BitmapLength,
								uint32_t *			RetBitmapLength,
								uint32_t *			u32RetFormat )
{
	if( m_CaptureThread.isAborted() )
	{
		return FALSE;
	}

	bool Ret = FALSE;
	//Lock(737);
	
	DWORD Size = 0;
	
	// Reset any error conditions.
	GetPreviousError(NULL,NULL,TRUE);
	
	if (*Bitmap == NULL)
	{
		AllocDIBImage( Bitmap, &Size );
		BitmapLength = Size;
	}
	else
	{
		AllocDIBImage( NULL, &Size );
	}
	
	if (*Bitmap && Size > 0)
	{
		if (RetBitmapLength)
		{
			*RetBitmapLength = Size;
		}
		
		// Must assign pointer to class member variable so that the
		// callback function can get to it.
		//ImageLockObj.Lock();
		m_TransferBitmapInfo = *Bitmap;
		m_TransferBitmapInfoSize = BitmapLength;
		//ImageLockObj.Unlock();
		
		// Start capturing now.  Callback function will capture and signal us when done.
		//LogMsg( LOG_DEBUG, "Before Capture Start Call\n");
		Ret = capGrabFrame((HWND)m_hWndVideo);
		//LogMsg( LOG_DEBUG, "After Capture Start Call\n");
		if( m_CaptureThread.isAborted() )
		{
			return FALSE;
		}

		m_ImageReady.wait();
		if( m_CaptureThread.isAborted() )
		{
			return FALSE;
		}	

		//ImageLockObj.Lock();
		m_TransferBitmapInfo = NULL;
		m_TransferBitmapInfoSize = 0;
		//ImageLockObj.Unlock();
		if( Ret )
		{
			*u32RetFormat = (*Bitmap)->bmiHeader.biCompression;
		}
		else
		{
			*u32RetFormat = 0;
			if (RetBitmapLength)
			{
				*RetBitmapLength = (ULONG) 0;
			}
		}
	}

	if (!Ret && m_ErrorID == 0)
	{
		m_ErrorID = DV_ERR_NONSPECIFIC;
	}

	//Unlock(737);
	return Ret;
}

/*******************************************************************************
Function   : CaptureAVI
Arguments  : Filename (input) - Name of file to capture AVI
FramesPerSec (input) - Frames Per second of AVI.
Duration (input) - How long to run it in seconds.
Quality (input unused)
Return     : TRUE Success, FALSE Failed.
Description: Captures AVI to file from current video capture device.
*******************************************************************************/
bool VxVidCapImp::CaptureAVI(	LPCTSTR Filename,
								FLOAT	FramesPerSec,
								ULONG	Duration,
								UINT	Quality )
{
	if( m_CaptureThread.isAborted() )
	{
		return FALSE;
	}

	bool Ret = FALSE;
	CAPTUREPARMS OrigCapParms;
	CAPTUREPARMS CapParms;
	// Reset any error conditions.
	GetPreviousError(NULL,NULL,TRUE);
	
	m_CancelCapture.signal();
	
	capCaptureGetSetup((HWND)m_hWndVideo,&OrigCapParms,sizeof(OrigCapParms));
	CapParms = OrigCapParms;
	CapParms.dwRequestMicroSecPerFrame = (DWORD) (1.0e6 / FramesPerSec);
	CapParms.fLimitEnabled = TRUE;
	CapParms.wTimeLimit = Duration;
	CapParms.fYield = FALSE;
	CapParms.wPercentDropForError = 100; //Quality;
	
	capCaptureSetSetup((HWND)m_hWndVideo, &CapParms, sizeof(CapParms));
	
	Ret = capCaptureSequence((HWND)m_hWndVideo);
	if (Ret)
	{
		Ret = capFileSaveAs((HWND)m_hWndVideo, Filename);
	}	

	capCaptureSetSetup((HWND)m_hWndVideo, &OrigCapParms, sizeof (OrigCapParms));
	
	if (!Ret && m_ErrorID == 0)
	{
		m_ErrorID = DV_ERR_NONSPECIFIC;
	}
	
	return Ret;
}

/*******************************************************************************
Function   : SetDriver
Arguments  : DriverIndex (input) - Driver to set
Return     : TRUE Success, FALSE Failed.
Description: Sets current capture driver.
*******************************************************************************/
bool VxVidCapImp::SetDriver(SHORT DriverIndex)
{
	bool Ret = false;
	CAPTUREPARMS CapParms = {0};
	m_iDriverIdx = DriverIndex;
	
	// Reset any error conditions.
	GetPreviousError(NULL,NULL,TRUE);
	char as8Buf[ 10 ];
	
	if( (HWND)m_hWndVideo  )
	{
		if (GetParent((HWND)m_hWndVideo) != NULL)
			capPreview((HWND)m_hWndVideo,FALSE);
		
		DisablePreviewVideo();
		capCaptureAbort((HWND)m_hWndVideo);
		
		Ret = capDriverConnect((HWND)m_hWndVideo, DriverIndex );
		
		if( Ret )
		{
			capGetVideoFormat((HWND)m_hWndVideo,&m_BmpInfoHeader,sizeof(m_BmpInfoHeader));
			m_u32FormatType		= m_BmpInfoHeader.bmiHeader.biCompression;
			int iPlanes			= m_BmpInfoHeader.bmiHeader.biPlanes;
			int iBitCnt			= m_BmpInfoHeader.bmiHeader.biBitCount;
			int iWidth			= m_BmpInfoHeader.bmiHeader.biWidth;
			int iHeight			= m_BmpInfoHeader.bmiHeader.biHeight;
			*((uint32_t *)as8Buf) = m_u32FormatType;
			as8Buf[4] = 0;
			//default to RGB24 mode
			if( m_BmpInfoHeader.bmiHeader.biPlanes != 1 ||
				m_BmpInfoHeader.bmiHeader.biBitCount != 24 ||
				m_BmpInfoHeader.bmiHeader.biCompression != BI_RGB )
			{
				LogMsg( LOG_DEBUG, "Default format type is %s %d planes %d bits format 0x%x\n", 
								as8Buf, 
								m_BmpInfoHeader.bmiHeader.biPlanes,
								m_BmpInfoHeader.bmiHeader.biBitCount,
								m_u32FormatType );
				
				if( (BI_RGB == m_u32FormatType) ||
					(FOURCC_RGB == m_u32FormatType) ||
					(FOURCC_RGBA == m_u32FormatType) ||
					(FOURCC_I420 == m_u32FormatType) ||
					(FOURCC_YUY2 == m_u32FormatType) ||
					(FOURCC_IYUV == m_u32FormatType) ||
					(FOURCC_YV12 == m_u32FormatType) )
				{
					// we should be able to handle these formats
				}
				else
				{
					//change format to RGB24
					m_BmpInfoHeader.bmiHeader.biPlanes = 1;
					m_BmpInfoHeader.bmiHeader.biBitCount = 24;
					m_BmpInfoHeader.bmiHeader.biCompression = BI_RGB;
					if( !capSetVideoFormat( (HWND)m_hWndVideo, 
											&m_BmpInfoHeader,
											sizeof(BITMAPINFO))) 
					{
						LogMsg( LOG_DEBUG, "Error. could not set video format to RGB24 type is %s \n", as8Buf  );
						m_BmpInfoHeader.bmiHeader.biPlanes = iPlanes;
						m_BmpInfoHeader.bmiHeader.biBitCount = iBitCnt;
						m_BmpInfoHeader.bmiHeader.biCompression = m_u32FormatType;
					}
				}

				// attempt to change size to preferred size
				if( ( m_BmpInfoHeader.bmiHeader.biWidth != m_iPreferedImageWidth ) ||
					( m_BmpInfoHeader.bmiHeader.biHeight != m_iPreferedImageHeight ) )
				{
					m_BmpInfoHeader.bmiHeader.biWidth  = m_iPreferedImageWidth;
					m_BmpInfoHeader.bmiHeader.biHeight = m_iPreferedImageHeight;
					if( !capSetVideoFormat( (HWND)m_hWndVideo, 
						&m_BmpInfoHeader,
						sizeof(BITMAPINFO))) 
					{
						LogMsg( LOG_DEBUG, "Error. could not set video size to %d %d \n", m_iPreferedImageWidth, m_iPreferedImageHeight );
						m_BmpInfoHeader.bmiHeader.biWidth  = iWidth;
						m_BmpInfoHeader.bmiHeader.biHeight = iHeight;
					}
				}

				// record the real size
				m_iCurrentImageWidth  = m_BmpInfoHeader.bmiHeader.biWidth;
				m_iCurrentImageHeight = m_BmpInfoHeader.bmiHeader.biHeight;
			}

			capCaptureGetSetup( (HWND)m_hWndVideo, &CapParms, sizeof(CapParms) );
			CapParms.fAbortLeftMouse = FALSE;
			CapParms.fAbortRightMouse = FALSE;
			CapParms.fYield = TRUE;
			CapParms.fCaptureAudio = FALSE;
			CapParms.wPercentDropForError = 100;
			capCaptureSetSetup( (HWND)m_hWndVideo, &CapParms, sizeof(CapParms) );
			m_iDriverIdx = DriverIndex;
			
			if( NULL != GetParent( (HWND)m_hWndVideo ) )
			{
				capPreview( (HWND)m_hWndVideo, TRUE );
			}

			//get caps
			Ret = DriverGetCaps( & m_DriverCaps );
		}
	}
	
	if (!Ret && m_ErrorID == 0)
	{
		m_ErrorID = DV_ERR_NONSPECIFIC;
	}

	return Ret;
}

/*******************************************************************************
Function   : GetPreviousError
Arguments  : ErrorID (output) - ID of Error
ErrorString (output) - Description of error.
ResetError (input) - TRUE Reset error condition.
Return     : none
Description: Gets the last Error ID and Error Description.
*******************************************************************************/
void VxVidCapImp::GetPreviousError(INT *ErrorID, std::string * ErrorString, bool ResetError)
{
	if( 0 == this )
	{
		// this should not happen
		return;
	}

	if (ErrorID)
		*ErrorID = m_ErrorID;
	
	if (ErrorString)
		*ErrorString = m_ErrorText;
	
	if (ResetError)
	{
		m_ErrorID = 0;
		m_ErrorText.clear();
	}
}

/*******************************************************************************
Function   : EnablePreviewVideo
Arguments  : Parent (input) - Parent window that will display video.
x (input) - X Location in parent where video will be shown.
y (input) - Y location in parent where video will be shown.
Width (input) - Width of preview window.
Height (input) - Height of preview window.
PreviewRate (input) - Rate of preview in FPS.
Return     : TRUE Success, FALSE Failed.
Description: Enables preview video mode.
*******************************************************************************/
bool VxVidCapImp::EnablePreviewVideo(VIDCAP_HWND Parent, INT x, INT y, INT iWidth, INT iHeight, INT PreviewRate)
{
	// Reset any error conditions.
	GetPreviousError(NULL,NULL,TRUE);
	
	SetParent((HWND)m_hWndVideo,(HWND)Parent);
	SetWindowLong((HWND)m_hWndVideo,GWL_STYLE,WS_CHILD);
	
	SetWindowPos(	(HWND)m_hWndVideo, 
					NULL,
					x, 
					y, 
					iWidth, 
					iHeight, 
					SWP_NOZORDER); 
	ShowWindow((HWND)m_hWndVideo,SW_SHOW);
	capPreviewRate((HWND)m_hWndVideo, PreviewRate);

	float f32Scale = CalculateCaptureScale( iWidth, iHeight );

	capPreviewScale((HWND)m_hWndVideo, f32Scale);
	
	return capPreview((HWND)m_hWndVideo,TRUE);
}

/*******************************************************************************
Function   : DisablePreviewVideo
Arguments  : none
Return     : TRUE Success, FALSE Failed.
Description: Disables preview video.
*******************************************************************************/
bool VxVidCapImp::DisablePreviewVideo()
{
	// Reset any error conditions.
	GetPreviousError(NULL,NULL,TRUE);
	
	bool Ret = capPreview((HWND)m_hWndVideo,FALSE);
	
	SetWindowPos((HWND)m_hWndVideo,NULL,0,0,0,0,SWP_NOZORDER);
	SetParent((HWND)m_hWndVideo,NULL);
	SetWindowLong((HWND)m_hWndVideo,GWL_STYLE,WS_POPUP);
	
	return Ret;
}

/*******************************************************************************
Function   : DriverGetCaps
Arguments  : Caps (output)
Return     : See capDriverGetCaps()
Description: Wrapper function for capDriverGetCaps().
*******************************************************************************/
bool VxVidCapImp::DriverGetCaps(CAPDRIVERCAPS *Caps)
{
	// Reset any error conditions.
	GetPreviousError(NULL,NULL,TRUE);
	
	return capDriverGetCaps((HWND)m_hWndVideo,Caps,sizeof(*Caps));
}

/*******************************************************************************
Function   : DlgVideoSource
Arguments  : none
Return     : See capDlgVideoSource()
Description: Wrapper function for capDlgVideoSource().
*******************************************************************************/
bool VxVidCapImp::DlgVideoSource( void )
{
	// Reset any error conditions.
	GetPreviousError(NULL,NULL,TRUE);

	//return PostThreadMessage(m_CaptureThread->m_nThreadID,VX_VID_SOURCE,0,0);
	return FALSE;
}

/*******************************************************************************
Function   : DlgVideoFormat
Arguments  : none
Return     : See capDlgVideoFormat()
Description: Wrapper function for capDlgVideoFormat()
*******************************************************************************/
bool VxVidCapImp::DlgVideoFormat( void )
{
	// Reset any error conditions.
	GetPreviousError(NULL,NULL,TRUE);
	
//	return PostThreadMessage(m_CaptureThread->m_nThreadID,VX_VID_FORMAT,0,0);

	//we want to wait for user to make changes
	//so we can resize the window so dont use thread for this dlg
	
	if( capDlgVideoFormat((HWND)m_hWndVideo) )
	{
		// format has probably changed
		lock( 989 );
		SetDriver( (SHORT)m_iDriverIdx );
		unlock( 989 );
		return true;
	}

	return false;
}


/*******************************************************************************
Function   : DlgVideoDisplay
Arguments  : none
Return     : See capDlgVideoDisplay()
Description: Wrapper function for capDlgVideoDisplay()
*******************************************************************************/
bool VxVidCapImp::DlgVideoDisplay()
{
	// Reset any error conditions.
	GetPreviousError(NULL,NULL,TRUE);
	
	//return PostThreadMessage(m_CaptureThread->m_nThreadID,VX_VID_DISPLAY,0,0);
	return true;
}

/*******************************************************************************
Function   : CancelCapture
Arguments  : none
Return     : none
Description: Cancels current AVI capture.
*******************************************************************************/
void VxVidCapImp::CancelCapture()
{
	capCaptureAbort((HWND)(HWND)m_hWndVideo);
	m_CancelCapture.signal();
}

/*******************************************************************************
Function   : AllocDIBImage
Arguments  : ppImageData (output)     - Return pointer to allocated
memory.  If passed as NULL,
not used.
AllocatedSize (output)   - Size of allocated block.
If passed as NULL, not used.
Return     : none
Description: Allocates image buffer for DIB capture.
*******************************************************************************/
bool VxVidCapImp::AllocDIBImage(PBITMAPINFO *	ppImageData,
								ULONG *			AllocatedSize )
{
	bool Ret = TRUE;
	DWORD Size = 0;
	
	// Reset any error conditions.
	GetPreviousError(NULL,NULL,TRUE);
	
	Size = CalcBitmapInfoSize( m_BmpInfoHeader.bmiHeader ) + CalcBitmapSize( m_BmpInfoHeader.bmiHeader );
	
	if (Size > 0)
	{
		if (ppImageData)
		{
			*ppImageData = (BITMAPINFO *) new BYTE[Size];
			(**ppImageData)=m_BmpInfoHeader;
		}
	}
	else
	{
		Ret = FALSE;
	}
	
	if (AllocatedSize)
	{
		*AllocatedSize = Size;
	}
	
	return Ret;
}

/*******************************************************************************
Function   : CalcBitmapSize()
Arguments  : bmiHeader (input) - BITMAPINFOHEADER from which to calculate
bitmap size.
Return     : Size of Bitmap.
Description: Calculates the size of a bitmap based upon the contents of
the BITMAPINFOHEADER passed in.
*******************************************************************************/
ULONG VxVidCapImp::CalcBitmapSize(const BITMAPINFOHEADER &bmiHeader)
{
	ULONG Size = 0;
	if (bmiHeader.biSizeImage == 0)
	{
		Size =	bmiHeader.biWidth *
				bmiHeader.biHeight *
				bmiHeader.biBitCount / 8;
	}
	else
	{
		Size = bmiHeader.biSizeImage;
	}
	
	return Size;
}

/*******************************************************************************
Function   : CalcBitmapInfoSize()
Arguments  : bmiHeader (input) - BITMAPINFOHEADER from which to calculate
bitmap size.
Return     : Size of Bitmap Info Header.
Description: Calculates the size of a bitmap info header based upon the
contents of the BITMAPINFOHEADER passed in.  This function
can be used to determine the offset from the BITMAPINFOHEADER
to the actual bitmap data.
*******************************************************************************/
ULONG VxVidCapImp::CalcBitmapInfoSize(const BITMAPINFOHEADER &bmiHeader)
{
	UINT bmiSize = (bmiHeader.biSize != 0) ? bmiHeader.biSize : sizeof(BITMAPINFOHEADER);
	return bmiSize + bmiHeader.biClrUsed * sizeof (RGBQUAD);
}

//============================================================================
//! calculate scale needed for capture image size to fit window
float VxVidCapImp::CalculateCaptureScale( int iWndWidth, int iWndHeight )
{
	// get size of capture
	// record the real size
	m_iCurrentImageWidth  = m_BmpInfoHeader.bmiHeader.biWidth;
	m_iCurrentImageHeight = m_BmpInfoHeader.bmiHeader.biHeight;

	float f32ScaleWidth = (float)iWndWidth / (float)m_iCurrentImageWidth;
	float f32ScaleHeight = (float)iWndHeight / (float)m_iCurrentImageHeight;
	
	return (f32ScaleWidth < f32ScaleHeight) ? f32ScaleWidth : f32ScaleHeight;
}


//============================================================================
// Internal callback functions.
//============================================================================
//==============================================================
// ErrorCallbackProc
//==============================================================
static LRESULT CALLBACK ErrorCallbackProc(HWND hWnd, int nErrID, LPSTR lpErrorText)
{
	VxVidCapImp *poVidCap = (VxVidCapImp *) capGetUserData(hWnd);
	
	if (poVidCap)
	{
		poVidCap->m_ErrorID = nErrID;
		poVidCap->m_ErrorText = lpErrorText;
	}

	return (LRESULT) TRUE;
}

//==============================================================
// ControlCallbackProc
//==============================================================
static LRESULT CALLBACK ControlCallbackProc(HWND hWnd, int nState)
{
	VxVidCapImp *poVidCap = (VxVidCapImp *) capGetUserData(hWnd);
	LRESULT Ret = TRUE;
	
	switch(nState)
	{
	case CONTROLCALLBACK_PREROLL:
		if (poVidCap)
		{
           // poVidCap->m_CancelCapture.ResetEvent();
		}
		
		Ret = TRUE;
		break;
		
	case CONTROLCALLBACK_CAPTURING:
		// if m_CancelCapture is posted, then we cancel AVI capture by returning FALSE.
		if (poVidCap)
		{
            //Ret = (::WaitForSingleObject(poVidCap->m_CancelCapture,0) != WAIT_OBJECT_0);
			
            if (!Ret)
				LogMsg( LOG_DEBUG, "Callback Canceled Capture\n");
		}
		
		break;
	}	

	return Ret;
}

//==============================================================
// StatusCallbackProc
//==============================================================
static LRESULT CALLBACK StatusCallbackProc(HWND hWnd, int nID, LPCSTR lpsz)
{
    //VxVidCapImp *poVidCap = (VxVidCapImp *) capGetUserData(hWnd);
	
	switch(nID)
	{
	case IDS_CAP_BEGIN:
		break;
		
	case IDS_CAP_END:
		break;
	}

	return (LRESULT) TRUE;
}

//==============================================================
// FrameCallbackProc
//==============================================================
static LRESULT CALLBACK FrameCallbackProc(HWND hWnd, LPVIDEOHDR lpVHdr)
{
	VxVidCapImp *poVidCap = (VxVidCapImp *) capGetUserData(hWnd);
	LRESULT Ret = TRUE;
	
	if (poVidCap)
	{
		if (! (HWND)poVidCap->m_hWndVideo )
		{
			Ret = FALSE;
			poVidCap->m_ImageReady.signal();
		}
		else
		{
			//CSingleLock ImageLockObj( &poVidCap->m_ImageProtect, TRUE );		
			
			if (poVidCap->m_TransferBitmapInfo)
			{
				ULONG Size;
				
				memcpy( poVidCap->m_TransferBitmapInfo, 
						&poVidCap->m_BmpInfoHeader.bmiHeader, sizeof( BITMAPINFOHEADER ) );
				
				Size =  MIN(poVidCap->m_TransferBitmapInfoSize - poVidCap->CalcBitmapInfoSize( poVidCap->m_TransferBitmapInfo->bmiHeader ),
					lpVHdr->dwBytesUsed);
				memcpy(((CHAR *) poVidCap->m_TransferBitmapInfo) + poVidCap->CalcBitmapInfoSize( poVidCap->m_TransferBitmapInfo->bmiHeader ),
					lpVHdr->lpData,
					Size);
			}

			//ImageLockObj.Unlock();
			Ret = TRUE;			
			poVidCap->m_ImageReady.signal();
		}
	}
	else
	{
		Ret = FALSE;
		poVidCap->m_ImageReady.signal();
	}

	return Ret;
}

//==============================================================
// StreamCallbackProc
//==============================================================
static LRESULT CALLBACK StreamCallbackProc(HWND hWnd, LPVIDEOHDR lpVHdr)
{
    //VxVidCapImp *poVidCap = (VxVidCapImp *) capGetUserData(hWnd);
	LRESULT Ret = TRUE;
	
	return Ret;
}

//==============================================================
// CaptureWindowThreadFunc
//==============================================================
static uint32_t CaptureWindowThreadFunc( void * pvContext )
{
	VxThread* poThread = (VxThread*)pvContext;
	poThread->setIsThreadRunning( true );
	VxVidCapImp *pImageProc = (VxVidCapImp *)poThread->getThreadUserParam();
	if( poThread->isAborted() )
	{
		pImageProc->m_hWndVideo = NULL;
		pImageProc->m_ImageReady.signal();
		//! Thread calls this just before exit
		poThread->threadAboutToExit();
		return 0;
	}

	UINT Ret = 0;
	MSG Msg;
	int i;
	
	bool bQuit = FALSE;
	pImageProc->m_bCapWindowDestroyed = FALSE;
	pImageProc->m_bCapWindowCreated = FALSE;

	if( NULL == (HWND)pImageProc->m_hWndVideo )
	{
        pImageProc->m_hWndVideo = (VIDCAP_HWND)capCreateCaptureWindowA((LPSTR) "My Capture Window", WS_VISIBLE | WS_POPUP,
			0,0,
			10,10,0,0x0);
	}

	if( (NULL == (HWND)pImageProc->m_hWndVideo) || 
		(INVALID_HANDLE_VALUE == (HWND)pImageProc->m_hWndVideo) )
	{
		//failed.. what to do
		for( i = 0; i < 3; i++ )
		{
			LogMsg( LOG_DEBUG, "CaptureWindowThreadFunc: unable to create capture window\n" );
		}
		pImageProc->m_hWndVideo = NULL;
		pImageProc->m_ImageReady.signal();
		poThread->threadAboutToExit();
		return 0;
	}

	// start capturing
	ShowWindow( (HWND)pImageProc->m_hWndVideo, FALSE );

	pImageProc->m_bCapWindowCreated = TRUE;
	pImageProc->m_ImageReady.signal(); // tell main thread capture window is created and ready
	
	// Process window messages for VFW window until a WM_QUIT is posted.
	while( GetMessage( &Msg, (HWND)pImageProc->m_hWndVideo, 0, 0 ) && 
		(FALSE == bQuit) )
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
		
		switch(Msg.message)
		{
//			case VX_CUSTOM_START:
//				pImageProc->m_ImageReady.SetEvent();
//				break;

		case WM_DEVICECHANGE:
			LogMsg( LOG_DEBUG, "VidCap DeviceChange\n" );
			break;

		case VX_VID_SOURCE:
			capDlgVideoSource((HWND)pImageProc->m_hWndVideo);
			break;
			
		case VX_VID_FORMAT:
			capDlgVideoFormat((HWND)pImageProc->m_hWndVideo);
			break;
			
		case VX_VID_DISPLAY:
			capDlgVideoDisplay((HWND)pImageProc->m_hWndVideo);
			break;

		case WM_CAP_DRIVER_CONNECT:
			LogMsg( LOG_DEBUG, "VidCap DriverConnect\n" );
			break;
		case WM_CAP_DRIVER_DISCONNECT:
			LogMsg( LOG_DEBUG, "VidCap DriverDisconnect\n" );
			break;
		case WM_CLOSE:
		case WM_DESTROY:
		case WM_QUIT:
			bQuit = TRUE;
			break;
		}
	}

	Ret = Msg.wParam;	
	pImageProc->m_bCapWindowDestroyed = TRUE;
	poThread->threadAboutToExit();
	
	return Ret;
}

//============================================================================
bool VxVidCapImp::SetPreviewRate( int iFramesPerSec )
{
	if( iFramesPerSec < 0 )
	{
		return FALSE;
	}

	if( capPreviewRate( (HWND)GetCapWindow(), iFramesPerSec ) )
	{
		m_iPreviewFramesPerSec = iFramesPerSec ; 
		return TRUE;
	}

	return FALSE;
}

//============================================================================
// Function name   : VxVidCapImp::Load
// Description     : 
// Return type     : void 
void VxVidCapImp::LoadSettings( void )
{
	//vx_assert( g_poMainDlg );

	//VidCapSettings * poSaveSettings = &g_poMainDlg->m_VidCamSettings;
	//memcpy( (VidCapSettings *)this, poSaveSettings, sizeof( VidCapSettings ) );

	/*
	CMyRegKey gReg;
	DWORD value, dwDispNew;
	
	if( ERROR_SUCCESS  != gReg.Create(HKEY_CURRENT_USER, "Software\\Tronacom\\Vidcap",
		&dwDispNew ) )
	{
		LogMsg( LOG_DEBUG, "VxVidCapImp::Load: unable to open settings key" );
		return;
	}
	
	
	if(gReg.QueryValue(value, "ServerEnabled") != ERROR_SUCCESS)
		serverEnabled = FALSE;
	else
		serverEnabled = value;
	
	if(gReg.QueryValue(value, "ImageType") != ERROR_SUCCESS)
	{
		imageFormat = IMG_TYPE_JPEG;
	}
	else
	{
		imageFormat = IMG_TYPE_JPEG;
//		imageFormat = value;
	}
	
	if(gReg.QueryValue(value, "PreviewEnabled") != ERROR_SUCCESS)
		previewEnabled = TRUE;
	else
		previewEnabled = value;
	value = 0;
	//FIXME 
	if(gReg.QueryValue(value, "CaptureDevice") != ERROR_SUCCESS)
	{
		//accept default so do nothing
	}
	else
	{
		//set the capture device..
		//TODO: check if device available
	}
//	if(gReg.QueryValue(value, "m_iDestImageHeight") != ERROR_SUCCESS)
//		m_iDestImageHeight = 120;
//	else
//		m_iDestImageHeight = value;
	
//	if(gReg.QueryValue(value, "m_iDestImageWidth") != ERROR_SUCCESS)
//		m_iDestImageWidth = 160;
//	else
//		m_iDestImageWidth = value;
	
	if(gReg.QueryValue(value, "JpegQuality") != ERROR_SUCCESS)
		jpegQuality = 75;
	else
		jpegQuality = value;
	
	if(gReg.QueryValue(value, "m_bInvertImage") != ERROR_SUCCESS)
		m_bInvertImage = FALSE;
	else
		m_bInvertImage = value;
	
	if(gReg.QueryValue(value, "UseOverlay") != ERROR_SUCCESS)
		useOverlay = FALSE;
	else
		useOverlay = value;
	
	if(gReg.QueryValue(value, "OverlayTransColor") != ERROR_SUCCESS)
		overlayTransColor.Set( 0, 0, 0 );
	else
		overlayTransColor = value;
	
	for(int i=0; i < 6; i++)
		captions[i].LoadFromRegKey( gReg, i );
	
//	if(gReg.QueryValue(value, "LogAccess") != ERROR_SUCCESS)
//		logAccess = TRUE;
//	else
//		logAccess = value;
	
	if(gReg.QueryValue(value, "RefreshRate") != ERROR_SUCCESS)
		refreshRate = 0;
	else
		refreshRate = value;
	
	gReg.Close();
	
	if(dwDispNew==REG_CREATED_NEW_KEY)
		Save();
	getProfileString( TC_PLUGIN_LCL_NAME, "VxVidCapImp", "CapOffBmp", "Plugins/cam_cast_server_res/offline.png", m_csCaptionBmpFile );
	getProfileString( TC_PLUGIN_LCL_NAME, "VxVidCapImp", "OverlayBmp","Plugins/cam_cast_server_res/overlay.png",  m_csOverlayBmpFile );
	if( 0 == m_csCaptionBmpFile.size() )
	{
		m_csCaptionBmpFile = "Plugins/cam_cast_server_res/offline.png";
	}
	if( 0 == m_csOverlayBmpFile.size() )
	{
		m_csOverlayBmpFile ="Plugins/cam_cast_server_res/overlay.png";
	}
	*/
}

//============================================================================

void VxVidCapImp::SaveSettings( void )
{
	//vx_assert( g_poMainDlg );

	//VidCapSettings * poSaveSettings = &g_poMainDlg->m_VidCamSettings;
	//memcpy( poSaveSettings, (VidCapSettings *)this, sizeof( VidCapSettings ) );
	//g_poMainDlg->SaveCamCastServerSettings();

	/*
	CMyRegKey gReg;
	
	gReg.Create(HKEY_CURRENT_USER, "Software\\Tronacom\\Vidcap");
	gReg.SetValue(refreshRate, "RefreshRate");
//	gReg.SetValue(logAccess, "LogAccess");
	gReg.SetValue(serverEnabled, "ServerEnabled");
	gReg.SetValue(imageFormat, "ImageType");
//	gReg.SetValue(previewEnabled, "PreviewEnabled");
//	gReg.SetValue(captureDevice, "CaptureDevice");
	gReg.SetValue(jpegQuality, "JpegQuality");
	gReg.SetValue(m_bInvertImage, "m_bInvertImage");
	gReg.SetValue(useOverlay, "UseOverlay");
	gReg.SetValue( VxColourToU32( overlayTransColor ), "OverlayTransColor");
	
	for(int i=0; i < 6; i++)
	{
		captions[i].SaveToRegKey( gReg, i );
	}
	gReg.Close();
	setProfileString( TC_PLUGIN_LCL_NAME, "VxVidCapImp", "CapOffBmp",	m_csCaptionBmpFile.c_str() );
	setProfileString( TC_PLUGIN_LCL_NAME, "VxVidCapImp", "OverlayBmp",	m_csOverlayBmpFile.c_str() );
	*/
}

//============================================================================
bool VxVidCapImp::SetCaptureFormat( int iFormat, int iWidth, int iHeight )
{
	//for right now we only support RGB24
	if( VX_VIDCAP_FORMAT_RGB24 != iFormat )
	{
		return FALSE;
	}

	BITMAPINFO oCapFormat;
	oCapFormat.bmiHeader.biSize				= sizeof(BITMAPINFOHEADER);
	oCapFormat.bmiHeader.biWidth			= iWidth;
	oCapFormat.bmiHeader.biHeight			= iHeight;
	oCapFormat.bmiHeader.biPlanes			= 1;
	oCapFormat.bmiHeader.biBitCount			= 24;
	oCapFormat.bmiHeader.biCompression		= BI_RGB;
	oCapFormat.bmiHeader.biSizeImage		= ROUND_TO_4BYTE_BOUNDRY( iWidth * 3 ) * iHeight;
	oCapFormat.bmiHeader.biXPelsPerMeter	= 0;
	oCapFormat.bmiHeader.biYPelsPerMeter	= 0;
	oCapFormat.bmiHeader.biClrUsed			= 0;
	oCapFormat.bmiHeader.biClrImportant		= 0;
	if( !capSetVideoFormat( (HWND)m_hWndVideo, 
							&oCapFormat,
							sizeof(BITMAPINFO))) 
	{
		return FALSE;
	}
	/*
	CAPTUREPARMS   gCapParms;
	gCapParms.dwRequestMicroSecPerFrame=40000; //25 Hz //(20000 = 50 Hz Sampling)
	gCapParms.fMakeUserHitOKToCapture=FALSE;   // dialog box to start?
	gCapParms.wPercentDropForError=70;         // stop recording if 70% of frames are lost
	gCapParms.fYield=TRUE;                    // use extra thread for capturing?
	gCapParms.dwIndexSize=0;                   // (only needed when writing to files)
	gCapParms.wChunkGranularity=0;               // (only needed when writing to files)
	gCapParms.fUsingDOSMemory=0;                 //unused in win32.
	gCapParms.wNumVideoRequested=50; //200          //maximum number of video buffers to be allocated
	gCapParms.fCaptureAudio=FALSE;             //capture audio as well?
	gCapParms.wNumAudioRequested=10;           //maximum number of audio buffers to be allocated
	gCapParms.vKeyAbort=VK_ESCAPE | 0x8000;    //key to end capturing (STRG-ESCAPE) - causes crash when used!
	gCapParms.fAbortLeftMouse=FALSE;           //stop capturing when left mouse button pressed?
	gCapParms.fAbortRightMouse=FALSE;          //stop capturing when right mouse button pressed?
	gCapParms.fLimitEnabled=FALSE;             //stop after time limit?
	gCapParms.wTimeLimit=0;                      //according time limit
	gCapParms.fMCIControl=FALSE;               //is this an MCI Device?
	gCapParms.fStepMCIDevice=FALSE;  //ignored
	gCapParms.dwMCIStartTime=0;    //ignored
	gCapParms.dwMCIStopTime=0;     //ignored
	gCapParms.fStepCaptureAt2x=FALSE; 
	gCapParms.wStepCaptureAverageFrames=5;
	gCapParms.dwAudioBufferSize=0; 
	gCapParms.fDisableWriteCache=FALSE; //unused in win32 programs
	gCapParms.AVStreamMaster=AVSTREAMMASTER_NONE; 
	if (!capCaptureSetSetup(ghWndCap, &gCapParms, sizeof(CAPTUREPARMS))) return(0);
	if (!capCaptureSequenceNoFile(ghWndCap)) return(0);
	*/
	return TRUE;
}

//============================================================================
//! stop thread that detects video motion
void VxVidCapImp::StopMotionThread( void )
{
	m_MotionThread.abortThreadRun( true );
	m_MotionThread.killThread();
}

//============================================================================
//! stop thread that handles capture messages
void VxVidCapImp::StopCaptureThread( void )
{
	m_CaptureThread.abortThreadRun( true );
	m_ImageReady.signal();
	m_CaptureThread.killThread();
}

//============================================================================
static uint32_t CaptureMotionThreadFunc( void * pvContext )
{
	VxThread* poThread = (VxThread*)pvContext;
	poThread->setIsThreadRunning( true );
	VxVidCapImp *poVidCap = (VxVidCapImp *)poThread->getThreadUserParam();
	if( poThread->isAborted() )
	{
		goto thread_done;
	}

	BITMAPINFO * poBitmap = NULL;
	uint32_t		u32BitmapLen = 0;

	int64_t		s64CurTime;
	int64_t		s64ElapsedTime;
	uint32_t		u32ElapsedMilSec;
	int		iCaptureWidth;
	int		iCaptureHeight;

	while( TRUE )
	{
		if( poThread->isAborted() )
		{
			goto thread_done;
		}

		s64CurTime = GetGmtTimeMs();
		s64ElapsedTime = s64CurTime - poVidCap->m_s64TimeLastSnapshot;

		//convert to millisec
		u32ElapsedMilSec = (uint32_t)( s64ElapsedTime / 10000);

		if( u32ElapsedMilSec < (uint32_t)poVidCap->m_iMotionSnapshotInvervalMiliSec )
		{
			//not time to snapshot yet
			if( poVidCap->m_iMotionSnapshotInvervalMiliSec < 2000 )
			{
				VxSleep( poVidCap->m_iMotionSnapshotInvervalMiliSec/2 );
				continue;
			}
			else
			{
				VxSleep( 1000 );
				continue;
			}
		}

		//capture bitmap
		//other wise grab a frame to a bitmap
		poBitmap = NULL;

		poVidCap->lock( 121 );
		uint32_t u32Format;
		poVidCap->CaptureDIB( &poBitmap, 0, &u32BitmapLen, &u32Format );
		if( poThread->isAborted() )
		{
			goto thread_done;
		}

		if( poBitmap && u32BitmapLen )
		{
			// we got at frame

			iCaptureWidth = ((BITMAPINFOHEADER *)poBitmap)->biWidth;
			iCaptureHeight = ((BITMAPINFOHEADER *)poBitmap)->biHeight;

			//determine what percent of picture has changed for motion detection
			if( ( NULL != poVidCap->m_BitmapMotionOld ) &&
				( poVidCap->m_BitmapMotionOld->bmiHeader.biWidth == iCaptureWidth ) &&
				( poVidCap->m_BitmapMotionOld->bmiHeader.biHeight == iCaptureHeight ) )
			{
				poVidCap->m_u32MotionBitmapLen = u32BitmapLen;
				if( poVidCap->m_BitmapMotionOld )
				{
					delete poVidCap->m_BitmapMotionOld;
				}

				poVidCap->m_BitmapMotionOld  = poBitmap;
				poVidCap->m_s64TimeLastSnapshot = GetGmtTimeMs();
				//store the change
				poVidCap->m_f32MotionSamples[ poVidCap->m_iMotionSampleIdx ] = 100;
				//set idx for next sample
				poVidCap->m_iMotionSampleIdx++;
				if( MOTION_SAMPLE_CNT <= poVidCap->m_iMotionSampleIdx   )
				{
					poVidCap->m_iMotionSampleIdx = 0;
				}
			}
			else
			{
				poVidCap->m_u32MotionBitmapLen = u32BitmapLen;
				//we need to allocate new save bitmap
				if( poVidCap->m_BitmapMotionOld )
				{
					delete poVidCap->m_BitmapMotionOld;
				}

				poVidCap->m_BitmapMotionOld = poBitmap;
				poVidCap->m_s64TimeLastSnapshot = GetGmtTimeMs();
			}
		}

		poVidCap->unlock( 121 );
		if( poThread->isAborted() )
		{
			goto thread_done;
		}
	}

thread_done:
	//! Thread calls this just before exit
	poThread->threadAboutToExit();
	return 0;
}

//============================================================================
//! get snapshot as bitmap
//! NOTE: caller must delete the allocated bitmap
bool VxVidCapImp::GetLastMotionSnapshot(	BITMAPINFO **	ppgRetBitmap,			// return snapshot..NOTE: caller must delete it
											uint32_t &			u32RetBmpLen,			// return length of bitmap
											int64_t &			s64RetTimeOfSnapshot )	// return time snapshot was taken
{
	BITMAPINFO * poBitmap;
	if( 0 == m_u32MotionBitmapLen )
	{
		//no valid bmp
		* ppgRetBitmap = NULL;
		u32RetBmpLen = 0;
		return FALSE;
	}

//	Lock( 124 );
	if( m_BitmapMotionOld )
	{
		u32RetBmpLen = m_u32MotionBitmapLen;
		char * pBmp = new char[ m_u32MotionBitmapLen ];
		memcpy( pBmp, m_BitmapMotionOld, m_u32MotionBitmapLen );
		* ppgRetBitmap = (BITMAPINFO *)pBmp;
		s64RetTimeOfSnapshot = m_s64TimeLastSnapshot;
//		Unlock( 124 );
		return TRUE;
	}
	else
	{
		//no snapshot so take one
		uint32_t u32BitmapLen = 0;
		poBitmap = NULL;
		uint32_t u32Format;
		if( CaptureDIB( &poBitmap, 0, &u32BitmapLen, &u32Format ) &&
			u32BitmapLen )
		{
			* ppgRetBitmap = poBitmap;
			u32RetBmpLen = u32BitmapLen;
			s64RetTimeOfSnapshot = GetGmtTimeMs();
//			Unlock( 124 );
			return TRUE;
		}

		* ppgRetBitmap = NULL;
		u32RetBmpLen = 0;
//		Unlock( 124 );
		return FALSE;
	}
}

#endif // TARGET_OS_WINDOWS

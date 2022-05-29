#pragma once
//============================================================================
// Copyright (C) 2020 Brett R. Jones 
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

#include <stdint.h>

//#include <CoreLib/VxKeyDefs.h>
#include <CoreLib/VxFileTypeMasks.h>
//#include <CoreLib/AssetDefs.h>
#include <CoreLib/MediaCallbackInterface.h>

class InetAddress;
class FileShareSettings;
class AssetInfo;

//! \public Plugin server state
enum EPluginServerState
{
    ePluginServerStateDisabled,		//< server is disabled
    ePluginServerStateStarted,		//< server is running
    ePluginServerStateStopped,		//< server is enabled but not running

    eMaxPluginServerState
};

//! \public Video recording state
enum EVideoRecordState
{
    eVideoRecordStateDisabled,
    eVideoRecordStateStopRecording,
    eVideoRecordStateStartRecording,
    eVideoRecordStateStartRecordingInPausedState,
    eVideoRecordStatePauseRecording,
    eVideoRecordStateResumeRecording,
    eVideoRecordStateCancelRecording,
    eVideoRecordStateError,

    eMaxVideoRecordState
};

//! \public Audio recording state
enum ESndRecordState
{
    eSndRecordStateDisabled,
    eSndRecordStateStopRecording,
    eSndRecordStateStartRecording,
    eSndRecordStateStartRecordingInPausedState,
    eSndRecordStatePauseRecording,
    eSndRecordStateResumeRecording,
    eSndRecordStateCancelRecording,
    eSndRecordStateError,

    eMaxSndRecordState
};

//! \public Media request for callback when processed MediaProcessor types
enum EMediaInputType
{
    eMediaInputNone,
    eMediaInputAudioPkts,
    eMediaInputAudioOpus,
    eMediaInputAudioPcm,

    eMediaInputVideoPkts,
    eMediaInputVideoJpgSmall,
    eMediaInputVideoJpgBig,

    eMediaInputMixer,

    eMaxMediaInputType
};


/// IFromGui is an abstract interface for from GUI to P2PEngine calls
class IFromGui
{
public:
    /// Called when raw camera capture video data is available ( u32FourCc is the video data format )
    virtual void				fromGuiVideoData( uint32_t u32FourCc, uint8_t * pu8VidDataIn, int iWidth, int iHeight, uint32_t u32VidDataLen, int iRotation ) = 0;
};
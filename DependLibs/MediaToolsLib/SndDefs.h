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

#include <CoreLib/VxDefs.h>

#define MY_OPUS_PKT_FRAMES_PER_PACKET			2		// number of frames to enc/dec at a time ( for MyP2PWeb.. not Opus )
#define MY_OPUS_PKT_UNCOMPRESSED_DATA_LEN		1280	// uncompressed length of  MyP2PWeb audio packet )
#define MY_PCM_SAMPLE_CNT						640		// expected PCM samples to encode / decode at a time ( 2 opus frames worth )
#define MY_OPUS_PKT_MS_PER_PKT					80		// milliseconds per pkt ( 2 opus frames )

#define MY_OPUS_FRAME_SAMPLE_CNT				320		// feed into opus frame size in samples
#define MY_OPUS_FRAME_BYTE_LEN					640		// opus frame size in bytes

#define MY_OPUS_TARGET_BIT_RATE					16000	// seem good quality vs size trade off
#define MY_OPUS_INPUT_SAMPLE_RATE				8000	// frequency of input samples


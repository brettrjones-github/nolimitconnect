#pragma once
//============================================================================
// Copyright (C) 2022 Brett R. Jones
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

#define MONO_8000HZ_SAMPLES_PER_10MS	80 //  (8000 HZ / 1000) * 10ms

#define MIXER_CHUNK_LEN_BYTES			1280
#define MIXER_CHUNK_LEN_SAMPLES			640
#define MIXER_CHUNK_LEN_MS				80 // PCM data len = 1280 at 8000 HZ sampling = 640 samples = 80ms of sound

#define SPEAKER_CHUNK_LEN_BYTES			(MIXER_CHUNK_LEN_SAMPLES * 6 * 2 * 2) // upsampled to 48000 2 channels length * 2 bytes per frame
#define SPEAKER_CHUNK_LEN_SAMPLES		(MIXER_CHUNK_LEN_SAMPLES * 6 * 2) // upsampled to 48000 2 channelssamples
#define MIXER_TO_SPEAKER_MULTIPLIER		12 // 48000 * 2 channels / 8000

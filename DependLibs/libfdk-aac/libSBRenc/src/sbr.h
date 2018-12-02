
/* -----------------------------------------------------------------------------------------------------------
Software License for The Fraunhofer FDK AAC Codec Library for Android

© Copyright  1995 - 2013 Fraunhofer-Gesellschaft zur Förderung der angewandten Forschung e.V.
  All rights reserved.

 1.    INTRODUCTION
The Fraunhofer FDK AAC Codec Library for Android ("FDK AAC Codec") is software that implements
the MPEG Advanced Audio Coding ("AAC") encoding and decoding scheme for digital audio.
This FDK AAC Codec software is intended to be used on a wide variety of Android devices.

AAC's HE-AAC and HE-AAC v2 versions are regarded as today's most efficient general perceptual
audio codecs. AAC-ELD is considered the best-performing full-bandwidth communications codec by
independent studies and is widely deployed. AAC has been standardized by ISO and IEC as part
of the MPEG specifications.

Patent licenses for necessary patent claims for the FDK AAC Codec (including those of Fraunhofer)
may be obtained through Via Licensing (www.vialicensing.com) or through the respective patent owners
individually for the purpose of encoding or decoding bit streams in products that are compliant with
the ISO/IEC MPEG audio standards. Please note that most manufacturers of Android devices already license
these patent claims through Via Licensing or directly from the patent owners, and therefore FDK AAC Codec
software may already be covered under those patent licenses when it is used for those licensed purposes only.

Commercially-licensed AAC software libraries, including floating-point versions with enhanced sound quality,
are also available from Fraunhofer. Users are encouraged to check the Fraunhofer website for additional
applications information and documentation.

2.    COPYRIGHT LICENSE

Redistribution and use in source and binary forms, with or without modification, are permitted without
payment of copyright license fees provided that you satisfy the following conditions:

You must retain the complete text of this software license in redistributions of the FDK AAC Codec or
your modifications thereto in source code form.

You must retain the complete text of this software license in the documentation and/or other materials
provided with redistributions of the FDK AAC Codec or your modifications thereto in binary form.
You must make available free of charge copies of the complete source code of the FDK AAC Codec and your
modifications thereto to recipients of copies in binary form.

The name of Fraunhofer may not be used to endorse or promote products derived from this library without
prior written permission.

You may not charge copyright license fees for anyone to use, copy or distribute the FDK AAC Codec
software or your modifications thereto.

Your modified versions of the FDK AAC Codec must carry prominent notices stating that you changed the software
and the date of any change. For modified versions of the FDK AAC Codec, the term
"Fraunhofer FDK AAC Codec Library for Android" must be replaced by the term
"Third-Party Modified Version of the Fraunhofer FDK AAC Codec Library for Android."

3.    NO PATENT LICENSE

NO EXPRESS OR IMPLIED LICENSES TO ANY PATENT CLAIMS, including without limitation the patents of Fraunhofer,
ARE GRANTED BY THIS SOFTWARE LICENSE. Fraunhofer provides no warranty of patent non-infringement with
respect to this software.

You may use this FDK AAC Codec software or modifications thereto only for purposes that are authorized
by appropriate patent licenses.

4.    DISCLAIMER

This FDK AAC Codec software is provided by Fraunhofer on behalf of the copyright holders and contributors
"AS IS" and WITHOUT ANY EXPRESS OR IMPLIED WARRANTIES, including but not limited to the implied warranties
of merchantability and fitness for a particular purpose. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
CONTRIBUTORS BE LIABLE for any direct, indirect, incidental, special, exemplary, or consequential damages,
including but not limited to procurement of substitute goods or services; loss of use, data, or profits,
or business interruption, however caused and on any theory of liability, whether in contract, strict
liability, or tort (including negligence), arising in any way out of the use of this software, even if
advised of the possibility of such damage.

5.    CONTACT INFORMATION

Fraunhofer Institute for Integrated Circuits IIS
Attention: Audio and Multimedia Departments - FDK AAC LL
Am Wolfsmantel 33
91058 Erlangen, Germany

www.iis.fraunhofer.de/amm
amm-info@iis.fraunhofer.de
----------------------------------------------------------------------------------------------------------- */

/*!
  \file
  \brief  Main SBR structs definitions  
*/

#ifndef __SBR_H
#define __SBR_H

#include "fram_gen.h"
#include "bit_sbr.h"
#include "tran_det.h"
#include "code_env.h"
#include "env_est.h"
#include "cmondata.h"

#include "qmf.h"
#include "resampler.h"

#include "ton_corr.h"


/* SBR bitstream delay */
  #define DELAY_FRAMES 2


typedef struct SBR_CHANNEL {
    struct ENV_CHANNEL        hEnvChannel;
    //INT_PCM                  *pDSOutBuffer;            /**< Pointer to downsampled audio output of SBR encoder */
    DOWNSAMPLER               downSampler;

} SBR_CHANNEL;
typedef SBR_CHANNEL* HANDLE_SBR_CHANNEL;

typedef struct SBR_ELEMENT {
    HANDLE_SBR_CHANNEL        sbrChannel[2];
    QMF_FILTER_BANK          *hQmfAnalysis[2];
    SBR_CONFIG_DATA           sbrConfigData;
    SBR_HEADER_DATA           sbrHeaderData;
    SBR_BITSTREAM_DATA        sbrBitstreamData;
    COMMON_DATA               CmonData;
    int32_t                       dynXOverFreqDelay[5];    /**< to delay a frame (I don't like it that much that way - hrc) */
    SBR_ELEMENT_INFO          elInfo;

    uint8_t                     payloadDelayLine[1+DELAY_FRAMES][MAX_PAYLOAD_SIZE];
    uint32_t                      payloadDelayLineSize[1+DELAY_FRAMES];                 /* Sizes in bits */

} SBR_ELEMENT, *HANDLE_SBR_ELEMENT;

typedef struct SBR_ENCODER
{
  HANDLE_SBR_ELEMENT   sbrElement[(8)];
  HANDLE_SBR_CHANNEL   pSbrChannel[(8)];
  QMF_FILTER_BANK      QmfAnalysis[(8)];
  DOWNSAMPLER          lfeDownSampler;
  int                  lfeChIdx;                 /* -1 default for no lfe, else assign channel index */
  int                  noElements;               /* Number of elements */
  int                  nChannels;                /* Total channel count across all elements. */
  int                  frameSize;                /* SBR framelength. */
  int                  bufferOffset;             /* Offset for SBR parameter extraction in time domain input buffer. */
  int                  downsampledOffset;        /* Offset of downsampled/mixed output for core encoder. */
  int                  downmixSize;              /* Size in samples of downsampled/mixed output for core encoder. */
  int32_t                  downSampleFactor;         /* Sampling rate relation between the SBR and the core encoder. */
  int                  fTimeDomainDownsampling;  /* Flag signalling time domain downsampling instead of QMF downsampling. */
  int                  nBitstrDelay;             /* Amount of SBR frames to be delayed in bitstream domain. */
  int32_t                  estimateBitrate;          /* estimate bitrate of SBR encoder */
  int32_t                  inputDataDelay;           /* delay caused by downsampler, in/out buffer at sbrEncoder_EncodeFrame */

  uint8_t* dynamicRam;
  uint8_t* pSBRdynamic_RAM;

  HANDLE_PARAMETRIC_STEREO  hParametricStereo;
  QMF_FILTER_BANK           qmfSynthesisPS;

  /* parameters describing allocation volume of present instance */
  int32_t                  maxElements;
  int32_t                  maxChannels;
  int32_t                  supportPS;


} SBR_ENCODER;


#endif /* __SBR_H */

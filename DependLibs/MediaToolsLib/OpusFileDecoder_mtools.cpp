/* Copyright (c) 2002-2007 Jean-Marc Valin
Copyright (c) 2008 CSIRO
Copyright (c) 2007-2013 Xiph.Org Foundation
File: opusdec.c

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

- Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.

- Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
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

#include "OpusFileDecoder.h"
#include "OpusAudioDecoder.h"
#include "OggStream.h"

#include "opus.h"
//#include "debug.h"
#include "opus_types.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <GuiInterface/IToGui.h>

#include <CoreLib/IsBigEndianCpu.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxFileUtil.h>


#include <math.h>
#include <stdlib.h>
#include <memory.h>

#ifdef HAVE_LRINTF
# define float2int(x) lrintf(x)
#else
# define float2int(flt) ((int)(floor(.5+flt)))
#endif

// 120ms at 48000
#define MAX_FRAME_SIZE (960*6)

#define readint(buf, base) (((buf[base+3]<<24)&0xff000000)| \
	((buf[base+2]<<16)&0xff0000)| \
	((buf[base+1]<<8)&0xff00)| \
	(buf[base]&0xff))
static unsigned int rngseed = 22222;
static GOTV_INLINE unsigned int fast_rand(void) {
	rngseed = (rngseed * 96314165) + 907633515;
	return rngseed;
}

#ifndef HAVE_FMINF
# define fminf(_x,_y) ((_x)<(_y)?(_x):(_y))
#endif

#ifndef HAVE_FMAXF
# define fmaxf(_x,_y) ((_x)>(_y)?(_x):(_y))
#endif

#if !defined(__LITTLE_ENDIAN__) && ( defined(GOTV_ARCH_BIGENDIAN) || defined(__BIG_ENDIAN__) )
#define le_short(s) ((short) ((unsigned short) (s) << 8) | ((unsigned short) (s) >> 8))
#define be_short(s) ((short) (s))
#else
#define le_short(s) ((short) (s))
#define be_short(s) ((short) ((unsigned short) (s) << 8) | ((unsigned short) (s) >> 8))
#endif 



namespace
{
	const int		    OPUS_FILE_HEADER_LEN			= 0x349; // (decimal 841)
	const int		    OPUS_MAX_PKT_LEN				= 390*6;

	//============================================================================
	int OpusHeaderParse( const unsigned char *packet, int len, MyOpusHeader * h )
	{
	   int i;
	   char str[9];
	   OggBuffer oggBuffer;
	   unsigned char ch;
	   ogg_uint16_t shortval;

	   oggBuffer.setDataBuf( (uint8_t *)packet );
	   oggBuffer.setMaxLen( len );
	   oggBuffer.setPos( 0 );
	   str[8] = 0;
	   if( len < 19 )
		   return 0;
	   oggBuffer.readChars( (unsigned char*)str, 8);
	   if( 0 != memcmp(str, "OpusHead", 8) )
		  return 0;

	   if( !oggBuffer.readChars( &ch, 1 ) )
		  return 0;
	   h->m_Version = ch;
	   if( (h->m_Version&240) != 0) /* Only major version 0 supported. */
		  return 0;

	   if( !oggBuffer.readChars( &ch, 1 ) )
		  return 0;
	   h->m_Channels = ch;
	   if (h->m_Channels == 0)
		  return 0;

	   if( !oggBuffer.readU16( &shortval ) )
		  return 0;
	   h->m_Preskip = shortval;

	   if( !oggBuffer.readU32( &h->m_InputSampleRate ) )
		  return 0;

	   if( !oggBuffer.readU16( &shortval))
		  return 0;
	   h->m_Gain = (short)shortval;

	   if( !oggBuffer.readChars( &ch, 1))
		  return 0;
	   h->m_ChannelMapping = ch;

	   if (h->m_ChannelMapping != 0)
	   {
		  if( !oggBuffer.readChars( &ch, 1))
			 return 0;

		  if (ch<1)
			 return 0;
		  h->m_StreamCnt = ch;

		  if( !oggBuffer.readChars( &ch, 1))
			 return 0;

		  if (ch>h->m_StreamCnt || (ch+h->m_StreamCnt)>255)
			 return 0;
		  h->m_CoupledCnt = ch;

		  /* Multi-stream support */
		  for (i=0;i<h->m_Channels;i++)
		  {
			 if( !oggBuffer.readChars( &h->m_StreamMap[i], 1))
				return 0;
			 if (h->m_StreamMap[i]>(h->m_StreamCnt+h->m_CoupledCnt) && h->m_StreamMap[i]!=255)
				return 0;
		  }
	   } else {
		  if(h->m_Channels>2)
			 return 0;
		  h->m_StreamCnt = 1;
		  h->m_CoupledCnt = h->m_Channels>1;
		  h->m_StreamMap[0]=0;
		  h->m_StreamMap[1]=1;
	   }
	   /*For version 0/1 we know there won't be any more data
		 so reject any that have data past the end.*/
	   if( (h->m_Version==0 || h->m_Version==1) && oggBuffer.getPos() != len)
		  return 0;
	   return 1;
	}
}

//============================================================================
OpusFileDecoder::OpusFileDecoder( P2PEngine& engine, MediaProcessor& mediaProcessor )
: m_Engine( engine )
, m_MediaProcessor( mediaProcessor )
, m_FileLen( 0 )
, m_FilePos( 0 )
, m_FileHandle( 0 )
, m_TotalSndFramesInFile( 0 )
, m_ConsumedSndFrames( 0 )
, m_DecoderInitialized( false )
, m_InputInitialized( false )
, m_StreamInit( false )
, m_HasOpusStream( 0 )
, m_HasTagsPacket( 0 )
, m_OpusSerialNum( 0 )
, m_Eos( 0 )
, m_PacketCount( 0 )
, m_LinkOut( 0 )
, m_ManualGain( 0 )
, m_Channels( -1 )
, m_Rate( 0 )
, m_Preskip( 0 )
, m_GranOffset( 0 )
, m_Resampler( 0 )
, m_OpusOutput( 0 )
, m_SpaceAvailCallbackEnabled( false )
{
}

//============================================================================
OpusFileDecoder::~OpusFileDecoder()
{
	delete m_OpusOutput;
	clearDecodedFrames();
}

//============================================================================
bool OpusFileDecoder::beginFileDecode( const char * fileName, VxGUID& assetId, int pos0to100000 )
{
	m_FileName	= fileName;
	m_AssetId	= assetId;
	//m_FileName	= "F://audio_test/outBigVoip.opus";
	m_FileLen	= VxFileUtil::getFileLen( m_FileName.c_str() );
	m_FilePos	= 0;
	if( 0 == m_FileLen )
	{
		LogMsg( LOG_ERROR, "OpusFileDecoder::file does not exist %s\n", fileName );
		return false;
	}

	if( MIN_OPUS_FILE_LEN > m_FileLen )
	{
		LogMsg( LOG_ERROR, "OpusFileDecoder::file too short %s\n", fileName );
		return false;
	}

	m_FileHandle = fopen( m_FileName.c_str(), "rb" );
	if( 0 == m_FileHandle )
	{
		LogMsg( LOG_ERROR, "OpusFileDecoder::beginWrite could not open file to read %s\n", fileName );
		return false;
	}

	m_StreamInit			= false;
	m_PacketCount			= 0;
	m_FirstDecodedFrame		= true;
	m_HeaderHasBeenRead		= false;
	m_DecoderInitialized = seekOpusFile( m_FileHandle, pos0to100000 );
	
	if( !m_DecoderInitialized )
	{
		fclose( m_FileHandle );
		m_FileHandle = 0;
		return false;
	}

	m_InputInitialized = true;
	enableSpaceAvailCallback( true, true );

	return m_DecoderInitialized;
}

//============================================================================
void OpusFileDecoder::enableSpaceAvailCallback( bool enableCallback, bool lockResources )
{
	if( lockResources )
	{
		m_ResourceMutex.lock();
	}

	if( enableCallback != m_SpaceAvailCallbackEnabled )
	{
		m_SpaceAvailCallbackEnabled = enableCallback;
		m_MediaProcessor.wantMediaInput( eMediaInputMixer, this, this, m_SpaceAvailCallbackEnabled );
	}

	if( lockResources )
	{
		m_ResourceMutex.unlock();
	}
}

//============================================================================
void OpusFileDecoder::callbackAudioOutSpaceAvail( int freeSpaceLen )
{
	//LogMsg( LOG_INFO, "OpusFileDecoder::callbackAudioOutSpaceAvail start\n" );
	if( m_InputInitialized )
	{
		int decodedLen = 0;
		m_ResourceMutex.lock();
		if( freeSpaceLen >= MY_OPUS_PKT_UNCOMPRESSED_DATA_LEN )
		{
			uint8_t buf[ MY_OPUS_PKT_UNCOMPRESSED_DATA_LEN ];
			decodedLen = decodedNextFrame( buf, MY_OPUS_PKT_UNCOMPRESSED_DATA_LEN );
			if( decodedLen < MY_OPUS_PKT_UNCOMPRESSED_DATA_LEN )
			{
				memset( &buf[decodedLen], 0, MY_OPUS_PKT_UNCOMPRESSED_DATA_LEN-decodedLen );
			}
			else if( m_TotalSndFramesInFile )
			{
				m_Engine.getToGui().toGuiAssetAction( eAssetActionPlayProgress, m_AssetId, (int)((m_ConsumedSndFrames * 100000UL) / m_TotalSndFramesInFile) );
			}

			m_MediaProcessor.playAudio( (int16_t *)buf, MY_OPUS_PKT_UNCOMPRESSED_DATA_LEN );
		}

		if( 0 == decodedLen )
		{
			// all done
			m_InputInitialized = false;
			enableSpaceAvailCallback( false, false );
			m_Engine.getToGui().toGuiAssetAction( eAssetActionPlayEnd, m_AssetId, 0 );
		}

		m_ResourceMutex.unlock();
	}

	//LogMsg( LOG_INFO, "OpusFileDecoder::callbackAudioOutSpaceAvail done\n" );
}

//============================================================================
void OpusFileDecoder::moveOpusFramesToOutput( uint8_t * outBuffer )
{
	for( int i = 0; i < 2; i++ )
	{
		if( m_DecodedFrames.size() )
		{
			char * frame1 =  m_DecodedFrames[0];
			// in many android devices seem low volume. do a boost in volume
			m_Engine.getMediaProcesser().increasePcmSampleVolume( (int16_t *)frame1, 640, 20 );
			memcpy( outBuffer, frame1, 640 );
			m_DecodedFrames.erase( m_DecodedFrames.begin() );
			m_ConsumedSndFrames++;
			delete frame1;
		}
		else
		{
			memset( outBuffer, 0, 640 );
		}

		outBuffer += 640;
	}
}

//============================================================================
int OpusFileDecoder::decodedNextFrame( uint8_t * frameBuffer, int frameBufferLen )
{
	if( 2 <= m_DecodedFrames.size() )
	{
		// already have 2 opus frames which are 640 * 2 = 1280 or one pcm audio frame length
		moveOpusFramesToOutput( frameBuffer );
		return MY_OPUS_PKT_UNCOMPRESSED_DATA_LEN;
	}

	if( false == m_DecoderInitialized )
	{
		return 0;
	}

	char *			data;
	int				nb_read;
	ogg_int64_t 	pageGranule;
	bool			endOfFile = false;
	while( m_DecodedFrames.size() < 2 )
	{
		/*Get the ogg buffer for writing*/
		data = ogg_sync_buffer(&m_OggSyncState, 1800);
		/*Read bitstream from input file*/
		if( 0 != m_FileHandle )
		{
			nb_read = (int)fread( data, sizeof(char), 1800, m_FileHandle );
			m_FilePos += nb_read;
			if( 0 == m_TotalSndFramesInFile )
			{
				// if not total frames then use file offset to set progress bar
				m_Engine.getToGui().toGuiAssetAction( eAssetActionPlayProgress, m_AssetId, calculateFileProgress() );
			}

			ogg_sync_wrote(&m_OggSyncState, nb_read);
			if( 1800 != nb_read )
			{
				endOfFile = true;
				fclose( m_FileHandle );
				m_FileHandle = 0;
			}
		}

		/*Loop for all complete pages we got (most likely only one)*/
		while( 1 == ogg_sync_pageout( &m_OggSyncState, &m_OggPage ) )
		{
			if( 0 == m_StreamInit ) 
			{
				ogg_stream_init(&m_StreamState, ogg_page_serialno(&m_OggPage));
				m_StreamInit = true;
			}

			if (ogg_page_serialno(&m_OggPage) != m_StreamState.serialno) 
			{
				/* so all streams are read. */
				ogg_stream_reset_serialno(&m_StreamState, ogg_page_serialno(&m_OggPage));
			}

			/*Add page to the bitstream*/
			ogg_stream_pagein( &m_StreamState, &m_OggPage );
			pageGranule = (int)ogg_page_granulepos(&m_OggPage);
			/*Extract all available packets*/
			while( 1 == ogg_stream_packetout(&m_StreamState, &m_OggPkt ) )
			{
				/*OggOpus streams are identified by a magic string in the initial
				stream header.*/
				if( m_OggPkt.b_o_s && m_OggPkt.bytes >= 8 && !memcmp(m_OggPkt.packet, "OpusHead", 8 ) ) 
				{
					if( m_HasOpusStream && m_HasTagsPacket )
					{
						/*If we're seeing another BOS OpusHead now it means
						the stream is chained without an EOS.*/
						m_HasOpusStream=0;
						//if(m_AudioDecoder)
						//	opus_multistream_decoder_destroy(st);
						//st = NULL;
						LogMsg( LOG_ERROR, "\nWarning: stream %lld ended without EOS and a new stream began.\n",(long long)m_StreamState.serialno);
					}

					if( !m_HasOpusStream )
					{
						if( m_PacketCount > 0 && m_OpusSerialNum == m_StreamState.serialno )
						{
							LogMsg( LOG_ERROR, "\nError: Apparent chaining without changing serial number (%lld==%lld).\n",
								(long long)m_OpusSerialNum,(long long)m_StreamState.serialno );
							finishFileDecode();
							return 0;
						}

						m_OpusSerialNum = m_StreamState.serialno;
						m_HasOpusStream = 1;
						m_HasTagsPacket = 0;
						m_LinkOut = 0;
						m_PacketCount = 0;
						m_Eos = 0;
					} 
					else 
					{
						LogMsg( LOG_ERROR, "\nWarning: ignoring opus stream %lld\n",(long long)m_StreamState.serialno);
					}
				}

				if( !m_HasOpusStream || m_StreamState.serialno != m_OpusSerialNum )
					break;
				/*If first packet in a logical stream, process the Opus header*/
				if( 0 == m_PacketCount )
				{
					if( ! processOggFileHeader( m_MyOpusHeader, &m_OggPkt, m_ManualGain ) )
					{
						finishFileDecode();
						return 0;
					}

					if(ogg_stream_packetout(&m_StreamState, &m_OggPkt)!=0 || m_OggPage.header[m_OggPage.header_len-1]==255)
					{
						/*The format specifies that the initial header and tags packets are on their
						own pages. To aid implementors in discovering that their files are wrong
						we reject them explicitly here. In some player designs files like this would
						fail even without an explicit test.*/
						LogMsg( LOG_ERROR,  "Extra packets on initial header page. Invalid stream.\n");
						finishFileDecode();
						return 0;
					}

					/*Remember how many samples at the front we were told to skip
					so that we can adjust the timestamp counting.*/
					m_GranOffset	= m_MyOpusHeader.m_Preskip;
					m_Preskip		= m_MyOpusHeader.m_Preskip;
					m_Channels		= m_MyOpusHeader.m_Channels;
					m_Rate			= m_MyOpusHeader.m_InputSampleRate;

					if( !m_OpusOutput )
					{
						m_OpusOutput = (float *)malloc(sizeof(float)*MAX_FRAME_SIZE*m_Channels);
					}

					/*Normal players should just play at 48000 or their maximum m_Rate,
					as described in the OggOpus spec.  But for commandline tools
					like opusdec it can be desirable to exactly preserve the original
					sampling m_Rate and duration, so we have a resampler here.*/
					if( m_Rate != 48000 && m_Resampler == NULL )
					{
						int err;
						m_Resampler = speex_resampler_init(m_Channels, 48000, m_Rate, 5, &err);
						if (err!=0)
							LogMsg( LOG_ERROR,  "m_Resampler error: %s\n", speex_resampler_strerror(err));
						speex_resampler_skip_zeros(m_Resampler);
					}

					//if(!fout)fout=out_file_open(outFile, &wav_format, m_Rate, mapping_family, &m_Channels, fp);
				} 
				else if( 1 == m_PacketCount )
				{
					m_HasTagsPacket=1;
					if(ogg_stream_packetout(&m_StreamState, &m_OggPkt)!=0 || m_OggPage.header[m_OggPage.header_len-1]==255)
					{
						LogMsg( LOG_ERROR, "Extra packets on initial tags page. Invalid stream.\n");
						finishFileDecode();
						return 0;
					}
				} 
				else 
				{
					opus_int64 maxout;
					opus_int64 outsamp;

					// End of stream condition
					if (m_OggPkt.e_o_s && m_StreamState.serialno == m_OpusSerialNum)
						m_Eos=1; /* don't care for anything except opus m_Eos */

					int ret = opus_decode_float( m_OpusDecoder, (unsigned char*)m_OggPkt.packet, m_OggPkt.bytes, m_OpusOutput, MAX_FRAME_SIZE, 0);

					// If the decoder returned less than zero, we have an error.
					if( ret < 0 )
					{
						LogMsg( LOG_ERROR,  "Decoding error: %s\n", opus_strerror(ret));
						finishFileDecode();
						return 0;
					}

					m_DecodedSampleCnt = ret;
					/*This handles making sure that our output duration respects
					the final end-trim by not letting the output sample count
					get ahead of the granpos indicated value.*/
					maxout = ((( pageGranule - m_GranOffset ) * m_Rate ) / 48000 ) - m_LinkOut;
					outsamp = opusFloatOutputToPcm( m_OpusOutput, m_Channels, m_DecodedSampleCnt, m_Resampler, &m_Preskip, 0 > maxout ? 0 : maxout );
					m_LinkOut += outsamp;
					//audio_size += (fp?4:2)*outsamp*m_Channels;
					if( outsamp <= 0 )
					{
						LogMsg( LOG_INFO,  "WARNING opusFloatOutputToPcm returned %d\n", outsamp );
					}
				}

				m_PacketCount++;
			}

			// We're done, drain the resampler if we were using it.
			if( m_Eos && m_Resampler )
			{
				float *zeros;
				int drain;

				zeros=(float *)calloc(100*m_Channels,sizeof(float));
				drain = speex_resampler_get_input_latency(m_Resampler);
				do 
				{
					opus_int64 outsamp;
					int tmp = drain;
					if (tmp > 100)
						tmp = 100;
					outsamp = opusFloatOutputToPcm( zeros, m_Channels, tmp, m_Resampler, NULL, ((pageGranule-m_GranOffset)*m_Rate/48000)-m_LinkOut );
					m_LinkOut += outsamp;
					drain -= tmp;
				} while (drain>0);
				free(zeros);
				speex_resampler_destroy(m_Resampler);
				m_Resampler=NULL;
			}

			if( m_Eos )
			{
				m_HasOpusStream=0;
				break;
			}
		}

		if( endOfFile || ( 0 == m_HasOpusStream ) ) 
		{
			break;
		}
	}

	if( endOfFile || ( 0 == m_HasOpusStream ) ) 
	{
		finishFileDecode();
	}

	moveOpusFramesToOutput( frameBuffer );
	return MY_OPUS_PKT_UNCOMPRESSED_DATA_LEN;
}

//============================================================================
/*Process an Opus header and setup the opus decoder based on it.
It takes several pointers for header values which are needed
elsewhere in the code.*/
bool OpusFileDecoder::processOggFileHeader( MyOpusHeader& header, ogg_packet *op, float manualGain )
{
	int err;
	if( 0 == OpusHeaderParse( op->packet, op->bytes, &header )  )
	{
		LogMsg( LOG_ERROR, "Cannot parse header\n");
		return false;
	}

	m_Channels	= header.m_Channels;
	m_Rate		= header.m_InputSampleRate;
	m_Preskip	= header.m_Preskip;
	m_OpusDecoder = opus_decoder_create( 48000, header.m_Channels, &err );
	if(err != OPUS_OK)
	{
		LogMsg( LOG_ERROR, "Cannot create decoder: %s\n", opus_strerror( err ) );
		return false;
	}

	if( !m_OpusDecoder )
	{
		LogMsg( LOG_ERROR, "Decoder initialization failed: %s\n", opus_strerror( err ) );
		return false;
	}

	if(header.m_Gain!=0 || m_ManualGain!=0)
	{
		/*Gain API added in a newer libopus version, if we don't have it
		we apply the gain ourselves. We also add in a user provided
		manual gain at the same time.*/
		int gainadj = (int)(m_ManualGain*256.)+header.m_Gain;
		err=opus_decoder_ctl( m_OpusDecoder, OPUS_SET_GAIN(gainadj) );
		if(err==OPUS_UNIMPLEMENTED)
		{
			//*gain = (float)pow(10., gainadj/5120.);
			LogMsg( LOG_ERROR, "Old Opus Library does not implement gain\n" );
		} 
		else if (err!=OPUS_OK)
		{
			LogMsg( LOG_ERROR, "Error setting gain: %s\n", opus_strerror(err));
			return false;
		}
	}

	return true;
}

//============================================================================
void OpusFileDecoder::clearDecodedFrames( void )
{
	std::vector<char *>::iterator iter;
	for( iter = m_DecodedFrames.begin(); iter != m_DecodedFrames.end(); ++iter )
	{
		delete *iter;
	}

	m_DecodedFrames.clear();
}

//============================================================================
void OpusFileDecoder::finishFileDecode( bool abortedByUser )
{
	m_DecoderInitialized = false;

	if( abortedByUser )
	{
		m_ResourceMutex.lock();
		enableSpaceAvailCallback( false, false );
		clearDecodedFrames();
		m_InputInitialized = false; // normally this is cleared when all the decoded audio is used up
	}

	if( m_FileHandle )
	{
		fclose( m_FileHandle );
		m_FileHandle = 0;
	}

	ogg_stream_clear( &m_StreamState );
	ogg_sync_clear( &m_OggSyncState );

	if( m_OpusDecoder )
	{
		opus_decoder_destroy( m_OpusDecoder );
		m_OpusDecoder = 0;
	}

	if( abortedByUser )
	{
		m_ResourceMutex.unlock();
	}
}

//============================================================================
int OpusFileDecoder::opusFloatOutputToPcm(	float *			opusOutput, 
											int				channels, 
											int				frame_size, 
											SpeexResamplerState *resampler,
											int *			skip, 
											opus_int64		maxout )
{
	int sampout = 0;
	int i,tmp_skip;

	uint32_t out_len;
	short *out;
	float *buf;
	float *output;
	out = (short *)alloca(sizeof(short)*MAX_FRAME_SIZE*m_Channels);
	buf = (float *)alloca(sizeof(float)*MAX_FRAME_SIZE*m_Channels);
	do {
		if (skip)
		{
			tmp_skip = (*skip>frame_size) ? (int)frame_size : *skip;
			*skip -= tmp_skip;
		} 
		else 
		{
			tmp_skip = 0;
		}

		if (resampler)
		{
			uint32_t in_len;
			output=buf;
			in_len = frame_size-tmp_skip;
			out_len = (unsigned int)(1024<maxout?1024:maxout);
			speex_resampler_process_interleaved_float( resampler, opusOutput+m_Channels*tmp_skip, &in_len, buf, &out_len);
			opusOutput += m_Channels*(in_len+tmp_skip);
			frame_size -= in_len+tmp_skip;
		} 
		else 
		{
			output=opusOutput+m_Channels*tmp_skip;
			out_len=frame_size-tmp_skip;
			frame_size=0;
		}

		for (i=0;i<(int)out_len*m_Channels;i++)
			out[i]=(short)float2int(fmaxf(-32768,fminf(output[i]*32768.f,32767)));

		if( !IsBigEndianCpu() )
		{
			for (i=0;i<(int)out_len*m_Channels;i++)
				out[i]=le_short(out[i]);
		}

		if( maxout > 0 )
		{
			int totalLen = (int)(out_len < maxout ? out_len : maxout);
			int amountUsed = 0;
			char * tempOut = (char *)out;
			if( m_FirstDecodedFrame && ( totalLen < MY_OPUS_FRAME_SAMPLE_CNT ) )
			{
				// first frame not enough samples
				m_FirstDecodedFrame = false;
				char * outPcmOpusFrame = new char[ MY_OPUS_FRAME_BYTE_LEN ];
				int lenToCopy = totalLen * sizeof( int16_t );
				memset( outPcmOpusFrame, 0, MY_OPUS_FRAME_BYTE_LEN - lenToCopy );
				memcpy( &outPcmOpusFrame[ MY_OPUS_FRAME_BYTE_LEN - lenToCopy ], tempOut, lenToCopy );
				m_DecodedFrames.push_back( outPcmOpusFrame );
				amountUsed = totalLen;
			}
			else
			{
				while( totalLen >= MY_OPUS_FRAME_SAMPLE_CNT )
				{
					char * outPcmOpusFrame = new char[ MY_OPUS_FRAME_BYTE_LEN ];
					memcpy( outPcmOpusFrame, tempOut, MY_OPUS_FRAME_BYTE_LEN );
					m_DecodedFrames.push_back( outPcmOpusFrame );
					amountUsed += MY_OPUS_FRAME_SAMPLE_CNT;
					totalLen -= MY_OPUS_FRAME_SAMPLE_CNT;
					tempOut += MY_OPUS_FRAME_BYTE_LEN;
				}

				if( 0 != totalLen )
				{
					// some left over.. may be tail end
					int lenToCopy = totalLen * sizeof( int16_t );
					char * outPcmOpusFrame = new char[ MY_OPUS_FRAME_BYTE_LEN ];
					memcpy( outPcmOpusFrame, tempOut, lenToCopy );
					memset( &outPcmOpusFrame[ lenToCopy ], 0, MY_OPUS_FRAME_BYTE_LEN - lenToCopy );
					m_DecodedFrames.push_back( outPcmOpusFrame );
					amountUsed += totalLen;
					totalLen = 0;
				}
			}

			sampout+=amountUsed;
			maxout-=amountUsed;
		}
	} while (frame_size>0 && maxout>0);

	return sampout;
}

//============================================================================
bool OpusFileDecoder::seekOpusFile( FILE * fileHandle, int pos0to100000 )
{
	ogg_int64_t 	pageGranule;
	if( !m_HeaderHasBeenRead )
	{
		readTotalSndFrames( fileHandle );
		ogg_sync_init( &m_OggSyncState );
		if( ! seekFile( fileHandle, 0 ) )
			return false;
		while( !m_HeaderHasBeenRead )
		{
			/*Get the ogg buffer for writing*/
			char * data = ogg_sync_buffer(&m_OggSyncState, 1800);
			/*Read bitstream from input file*/
			if( 0 != m_FileHandle )
			{
				int nb_read = (int)fread( data, sizeof(char), 1800, m_FileHandle );
				m_FilePos += nb_read;
				ogg_sync_wrote(&m_OggSyncState, nb_read);
				if( 1800 != nb_read )
				{
					LogMsg( LOG_ERROR, "ERROR OpusFileDecoder:: could not read header\n", m_FileName.c_str() );
					return false;
				}
			}

			/*Loop for all complete pages we got (most likely only one)*/
			while( 1 == ogg_sync_pageout( &m_OggSyncState, &m_OggPage ) )
			{
				if( 0 == m_StreamInit ) 
				{
					ogg_stream_init(&m_StreamState, ogg_page_serialno(&m_OggPage));
					m_StreamInit = true;
				}

				if (ogg_page_serialno(&m_OggPage) != m_StreamState.serialno) 
				{
					/* so all streams are read. */
					ogg_stream_reset_serialno(&m_StreamState, ogg_page_serialno(&m_OggPage));
				}

				/*Add page to the bitstream*/
				ogg_stream_pagein( &m_StreamState, &m_OggPage );
				pageGranule = (int)ogg_page_granulepos(&m_OggPage);
				/*Extract all available packets*/
				if( 1 == ogg_stream_packetout(&m_StreamState, &m_OggPkt ) )
				{
					if( ! processOggFileHeader( m_MyOpusHeader, &m_OggPkt, m_ManualGain ) )
					{
						LogMsg( LOG_ERROR, "Cannot process ogg header: %s\n", m_FileName.c_str() );
						return false;
					}
					if(ogg_stream_packetout(&m_StreamState, &m_OggPkt)!=0 || m_OggPage.header[m_OggPage.header_len-1]==255)
					{
						/*The format specifies that the initial header and tags packets are on their
						own pages. To aid implementors in discovering that their files are wrong
						we reject them explicitly here. In some player designs files like this would
						fail even without an explicit test.*/
						LogMsg( LOG_ERROR,  "Extra packets on initial header page. Invalid stream.\n");
						return false;
					}

					/*Remember how many samples at the front we were told to skip
					so that we can adjust the timestamp counting.*/
					m_GranOffset	= m_MyOpusHeader.m_Preskip;
					m_Preskip		= m_MyOpusHeader.m_Preskip;
					m_Channels		= m_MyOpusHeader.m_Channels;
					m_Rate			= m_MyOpusHeader.m_InputSampleRate;

					if( !m_OpusOutput )
					{
						m_OpusOutput = (float *)malloc( sizeof(float) * MAX_FRAME_SIZE * m_Channels );
					}

					/*Normal players should just play at 48000 or their maximum m_Rate,
					as described in the OggOpus spec.  But for commandline tools
					like opusdec it can be desirable to exactly preserve the original
					sampling m_Rate and duration, so we have a resampler here.*/
					if( m_Rate != 48000 && m_Resampler == NULL )
					{
						int err;
						m_Resampler = speex_resampler_init( m_Channels, 48000, m_Rate, 5, &err );
						if (err!=0)
							LogMsg( LOG_ERROR,  "m_Resampler error: %s\n", speex_resampler_strerror(err));
						speex_resampler_skip_zeros(m_Resampler);
					}

					m_HeaderHasBeenRead = true;

					m_OpusSerialNum = m_StreamState.serialno;
					m_HasOpusStream = 1;
					m_HasTagsPacket = 0;
					m_LinkOut = 0;
					m_PacketCount = 2;
					m_Eos = 0;

					break;
				}
			}
		}
	}

	ogg_sync_reset( &m_OggSyncState );
	ogg_stream_init( &m_StreamState, m_OpusSerialNum );
	ogg_packet_clear( &m_OggPkt );
	m_OggPage.body_len 		= 0;
	m_OggPage.header_len 	= 0;

	// calculate position in file
	uint64_t filePos = 0;
	if( pos0to100000 )
	{
		filePos =  (uint64_t) (((double)pos0to100000 / 100000.0) * m_FileLen);
		if( filePos > ( m_FileLen - OPUS_MAX_PKT_LEN * 2 ) )
		{
			filePos = ( m_FileLen - OPUS_MAX_PKT_LEN * 2 );
		}
	}

	if( filePos < OPUS_FILE_HEADER_LEN )
	{
		filePos = OPUS_FILE_HEADER_LEN;
	}

	if( ! seekFile( fileHandle, filePos ) )
		return false;

	// find OggS packet header signature and start reading from there
	const char * oggHdrSig = "OggS";
	// 4 byte OggS then 1 byte struct version ( 0 ) then flags and stuff 
	char dataBuf[2048];
	int sigPos = -1;
	int amtRead = (int)fread( dataBuf, 1, 2048, fileHandle );
	if( amtRead > 5 )
	{
		for( int i = 0; i < amtRead; i++ )
		{
			if( 'O' == dataBuf[i] )
			{
				if( 0 == memcmp( &dataBuf[i], oggHdrSig, 5 ) )
				{
					// found packet header signature
					sigPos = i;
					break;
				}
			}
		}
	}

	if( -1 != sigPos )
	{
		if( 0 != pos0to100000 )
		{
			double percentPos =  (double)pos0to100000 / 100000.0;
			if( m_TotalSndFramesInFile )
			{
				m_ConsumedSndFrames =  (int)( (double)m_TotalSndFramesInFile * percentPos );
			}
		}

		return seekFile( fileHandle, filePos + sigPos );
	}

	return false;
}

//============================================================================
bool OpusFileDecoder::readTotalSndFrames( FILE * fileHandle )
{
	// at 0x9c ( should be signature nolimitconnect.com v0000000000000000-XXv where the zeros are hex ascii of total snd frames and XX is version number
	m_TotalSndFramesInFile = 0;
	m_ConsumedSndFrames = 0;
	char readBuf[ 512 ];
    if( 0 == fseek( fileHandle, NO_LIMIT_OPUS_SIGNITURE_OFFS, SEEK_SET ) )
    {
        if( sizeof( readBuf ) == fread( readBuf, 1, sizeof( readBuf ), fileHandle ) )
	    {
            uint64_t totalFrames = 0;
            for( int i = 0; i < 10; i++ )
            {
                if( 0 == strncmp( NO_LIMIT_OPUS_SIGNITURE, &readBuf[i], NO_LIMIT_OPUS_SIGNITURE_LEN ) )
                {
                    if( VxFileUtil::hexAsciiToU64( &readBuf[ NO_LIMIT_OPUS_SIGNITURE_LEN + i ], totalFrames ) )
                    {
                        if( 0 != totalFrames )
                        {
                            m_TotalSndFramesInFile = htonU64( totalFrames );
                            return true;
                        }
                    }
                }
            }
		}
	}

	return false;
}

//============================================================================
bool OpusFileDecoder::seekFile( FILE * fileHandle, uint64_t filePosition )
{
	RCODE rc = VxFileUtil::fileSeek( fileHandle, filePosition );
	if( rc )
	{
		LogMsg( LOG_ERROR, "ERROR OpusFileDecoder::seekFile err %d %s\n", rc, m_FileName.c_str() );
		return false;
	}

	m_FilePos = filePosition;
	return true;
}

//============================================================================
int	 OpusFileDecoder::calculateFileProgress( void )
{
	if( m_FileLen >= 2641 )
	{
		// the opus header + first chunk read is 2641 which is the position when first actually decoding
		if( m_FilePos < 2641 )
		{
			return 0;
		}

		uint64_t progress = (( m_FilePos - 2641 ) * 100000UL) / ( m_FileLen - 2641 );
		LogMsg( LOG_DEBUG, "OpusFileDecoder file pos %d len %d progress %d\n", (uint32_t)m_FilePos, (uint32_t)m_FileLen, (uint32_t)progress );

		return (int)progress;
	}

	return 0;
}

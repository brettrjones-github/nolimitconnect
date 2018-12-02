/*
 *  Copyright (c) 2015 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */
// Code Modified for MyP2PWeb by Brett R. Jones

package com.p2pgotv.gotvstation;

import java.lang.System;
import java.nio.ByteBuffer;
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;

import android.content.Context;
import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder.AudioSource;
import android.os.Process;
import android.util.Log;

public class WebRtcAudioRecord
{
	private static final boolean DEBUG = false;

	private static final String LOG_TAG = "WebRtcAudioRecord";

	// Default audio data format is PCM 16 bit per sample.
	// Guaranteed to be supported by all devices.
	private static final int BITS_PER_SAMPLE = 16;

	// Requested size of each recorded buffer provided to the client.
	private static final int CALLBACK_BUFFER_SIZE_MS = 10;

	// Average number of callbacks per second.
	private static final int BUFFERS_PER_SECOND = 1000 / CALLBACK_BUFFER_SIZE_MS;

	// We ask for a native buffer size of BUFFER_SIZE_FACTOR * (minimum required
	// buffer size). The extra space is allocated to guard against glitches
	// under
	// high load.
	private static final int 	BUFFER_SIZE_FACTOR 		= 2;

	private final long 			nativeAudioRecord;
	private final Context 		context;

	private WebRtcAudioEffects effects 					= null;

	private ByteBuffer 			byteBuffer;

	private AudioRecord 		audioRecord 			= null;
	private AudioRecordThread 	audioThread 			= null;

	private static volatile boolean microphoneMute 		= false;
	private byte[] 				emptyBytes;
	private static boolean 		m_RecordStarted 		= false;
	private static boolean 		m_WantMicrophoneInput 	= false;
	final Semaphore 			m_MicrophoneSemaphore 	= new Semaphore(1); 

	/**
	 * Audio thread which keeps calling ByteBuffer.read() waiting for audio to
	 * be recorded. Feeds recorded data to the native counterpart as a periodic
	 * sequence of callbacks using DataIsRecorded(). This thread uses a
	 * Process.THREAD_PRIORITY_URGENT_AUDIO priority.
	 */
	private class AudioRecordThread extends Thread implements ToGuiHardwareControlInterface
	{
		private volatile boolean m_KeepAlive = true;

		//========================================================================
		public AudioRecordThread( String name )
		{
			super( name );
		}

		//========================================================================
		@Override
		public void run()
		{
		    final String orgName = Thread.currentThread().getName();
		    Thread.currentThread().setName( orgName + " AudioRecordThread" );		

			Process.setThreadPriority( Process.THREAD_PRIORITY_URGENT_AUDIO );
			Log.d( LOG_TAG,
					"AudioRecordThread" + WebRtcAudioUtils.getThreadInfo() );
			assertTrue( audioRecord.getRecordingState() == AudioRecord.RECORDSTATE_RECORDING );

			long lastTime = System.nanoTime();
			try 
			{
				NativeRxFrom.wantToGuiHardwareControl( this, true );
				while( m_KeepAlive )
				{
					if( false == m_WantMicrophoneInput )
					{
						//  basically do nothing
						m_MicrophoneSemaphore.acquire();						
						if( false == m_KeepAlive )
						{
							break;
						}

						if( false == m_WantMicrophoneInput )
						{
							sleep( 100 );
							continue;
						}
					}
					
					int bytesRead = audioRecord.read( byteBuffer,
							byteBuffer.capacity() );
					if( bytesRead == byteBuffer.capacity() )
					{
						if( microphoneMute )
						{
							byteBuffer.clear();
							byteBuffer.put( emptyBytes );
						}
						
						NativeTxTo.fromGuiDataIsRecorded( bytesRead, nativeAudioRecord );
					}
					else
					{
						Log.e( LOG_TAG, "AudioRecord.read failed: " + bytesRead );
						if( bytesRead == AudioRecord.ERROR_INVALID_OPERATION )
						{
							m_KeepAlive = false;
						}
					}
					
					if( DEBUG )
					{
						long nowTime = System.nanoTime();
						long durationInMs = TimeUnit.NANOSECONDS
								.toMillis( ( nowTime - lastTime ) );
						lastTime = nowTime;
						Log.d( LOG_TAG, "bytesRead[" + durationInMs + "] "
								+ bytesRead );
					}
				}
			} 
			catch (InterruptedException e) 
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			} 
			
			NativeRxFrom.wantToGuiHardwareControl( this, false );
			try
			{
				audioRecord.stop();
			}
			catch( IllegalStateException e )
			{
				Log.e( LOG_TAG, "AudioRecord.stop failed: " + e.getMessage() );
			}
		}

		//========================================================================
		public void joinThread()
		{
			m_KeepAlive = false;
			m_MicrophoneSemaphore.release();
			while( isAlive() )
			{
				try
				{
					join();
				}
				catch( InterruptedException e )
				{
					// Ignore.
				}
			}
		}

		//========================================================================
		@Override
		public void doGuiShutdownHardware()
		{
			m_KeepAlive = false;
			m_MicrophoneSemaphore.release();
		}

		//========================================================================
		@Override
		public void doGuiWantMicrophoneRecording( boolean wantMicInput )
		{
			m_WantMicrophoneInput = wantMicInput;
			if( m_WantMicrophoneInput )
			{
				m_MicrophoneSemaphore.release();
			}			
		}

		//========================================================================
		@Override
		public void doGuiWantSpeakerOutput( boolean wantSpeakerOutput )
		{
		}

		//========================================================================
		@Override
		public void doGuiWantVideoCapture( boolean wantVideoCapture )
		{
		}
	}

	//========================================================================
	WebRtcAudioRecord( Context context, long nativeAudioRecord )
	{
		Log.d( LOG_TAG, "ctor" + WebRtcAudioUtils.getThreadInfo() );
		this.context = context;
		this.nativeAudioRecord = nativeAudioRecord;
		//effects = WebRtcAudioEffects.create();
		effects = null;
	}

	//========================================================================
	private boolean enableBuiltInAEC( boolean enable )
	{
		Log.d( LOG_TAG, "enableBuiltInAEC(" + enable + ')' );
		if( effects == null )
		{
			Log.e( LOG_TAG, "Built-in AEC is not supported on this platform" );
			return false;
		}
		
		return effects.setAEC( enable );
	}

	//========================================================================
	private boolean enableBuiltInAGC( boolean enable )
	{
		Log.d( LOG_TAG, "enableBuiltInAGC(" + enable + ')' );
		if( effects == null )
		{
			Log.e( LOG_TAG, "Built-in AGC is not supported on this platform" );
			return false;
		}
		
		return effects.setAGC( enable );
	}

	//========================================================================
	private boolean enableBuiltInNS( boolean enable )
	{
		Log.d( LOG_TAG, "enableBuiltInNS(" + enable + ')' );
		if( effects == null )
		{
			Log.e( LOG_TAG, "Built-in NS is not supported on this platform" );
			return false;
		}
		
		return effects.setNS( enable );
	}

	//========================================================================
	private int initRecording( int sampleRate, int channels )
	{
		Log.d( LOG_TAG, "initRecording(sampleRate=" + sampleRate
				+ ", channels=" + channels + ")" );
		if( !WebRtcAudioUtils.hasPermission( context,
				android.Manifest.permission.RECORD_AUDIO ) )
		{
			Log.e( LOG_TAG, "RECORD_AUDIO permission is missing" );
			return -1;
		}
		if( audioRecord != null )
		{
			Log.e( LOG_TAG,
					"InitRecording() called twice without StopRecording()" );
			return -1;
		}
		final int bytesPerFrame = channels * ( BITS_PER_SAMPLE / 8 );
		final int framesPerBuffer = sampleRate / BUFFERS_PER_SECOND;
		byteBuffer = ByteBuffer
				.allocateDirect( bytesPerFrame * framesPerBuffer );
		Log.d( LOG_TAG, "byteBuffer.capacity: " + byteBuffer.capacity() );
		emptyBytes = new byte[byteBuffer.capacity()];
		// Rather than passing the ByteBuffer with every callback (requiring
		// the potentially expensive GetDirectBufferAddress) we simply have the
		// the native class cache the address to the memory once.
		NativeTxTo.fromGuiCacheRecordDirectBufferAddress( byteBuffer, nativeAudioRecord );

		// Get the minimum buffer size required for the successful creation of
		// an AudioRecord object, in byte units.
		// Note that this size doesn't guarantee a smooth recording under load.
		int minBufferSize = AudioRecord.getMinBufferSize( sampleRate,
				AudioFormat.CHANNEL_IN_MONO, AudioFormat.ENCODING_PCM_16BIT );
		if( minBufferSize == AudioRecord.ERROR
				|| minBufferSize == AudioRecord.ERROR_BAD_VALUE )
		{
			Log.e( LOG_TAG, "AudioRecord.getMinBufferSize failed: "
					+ minBufferSize );
			return -1;
		}
		Log.d( LOG_TAG, "AudioRecord.getMinBufferSize: " + minBufferSize );

		// Use a larger buffer size than the minimum required when creating the
		// AudioRecord instance to ensure smooth recording under load. It has
		// been
		// verified that it does not increase the actual recording latency.
		int bufferSizeInBytes = Math.max( BUFFER_SIZE_FACTOR * minBufferSize,
				byteBuffer.capacity() );
		Log.d( LOG_TAG, "bufferSizeInBytes: " + bufferSizeInBytes );
		try
		{
			audioRecord = new AudioRecord( AudioSource.VOICE_COMMUNICATION,
					sampleRate, AudioFormat.CHANNEL_IN_MONO,
					AudioFormat.ENCODING_PCM_16BIT, bufferSizeInBytes );
		}
		catch( IllegalArgumentException e )
		{
			Log.e( LOG_TAG, e.getMessage() );
			return -1;
		}
		if( audioRecord == null
				|| audioRecord.getState() != AudioRecord.STATE_INITIALIZED )
		{
			Log.e( LOG_TAG, "Failed to create a new AudioRecord instance" );
			return -1;
		}
		/*
		Log.d( LOG_TAG,
				"AudioRecord " + "session ID: "
						+ audioRecord.getAudioSessionId() + ", "
						+ "audio format: " + audioRecord.getAudioFormat()
						+ ", " + "channels: " + audioRecord.getChannelCount()
						+ ", " + "sample rate: " + audioRecord.getSampleRate() );
		if( effects != null )
		{
			effects.enable( audioRecord.getAudioSessionId() );
		}*/
		
		// TODO(phoglund): put back audioRecord.getBufferSizeInFrames when
		// all known downstream users supports M.
		// if (WebRtcAudioUtils.runningOnMOrHigher()) {
		// Returns the frame count of the native AudioRecord buffer. This is
		// greater than or equal to the bufferSizeInBytes converted to frame
		// units. The native frame count may be enlarged to accommodate the
		// requirements of the source on creation or if the AudioRecord is
		// subsequently rerouted.

		// Log.d( LOG_TAG, "bufferSizeInFrames: "
		// + audioRecord.getBufferSizeInFrames());
		// }
		return framesPerBuffer;
	}

	//========================================================================
	private boolean startRecording()
	{
		if( false == m_RecordStarted )
		{
			m_RecordStarted = true;
			Log.d( LOG_TAG, "startRecording" );
			assertTrue( audioRecord != null );
			assertTrue( audioThread == null );
			try
			{
				audioRecord.startRecording();
			}
			catch( IllegalStateException e )
			{
				Log.e( LOG_TAG,
						"AudioRecord.startRecording failed: " + e.getMessage() );
				return false;
			}
			if( audioRecord.getRecordingState() != AudioRecord.RECORDSTATE_RECORDING )
			{
				Log.e( LOG_TAG, "AudioRecord.startRecording failed" );
				return false;
			}
			
			audioThread = new AudioRecordThread( "AudioRecordJavaThread" );
			audioThread.start();
		}
		
		return true;
	}

	//========================================================================
	private boolean stopRecording()
	{
		Log.d( LOG_TAG, "stopRecording" );
		if( m_RecordStarted )
		{
			m_RecordStarted = false;
			if( null != audioThread )
			{
				audioThread.joinThread();
				audioThread = null;
			}
			
			//if( effects != null )
			//{
			//	effects.release();
			//}
			
			if( audioRecord != null )
			{
				audioRecord.stop();
				audioRecord.release();
				audioRecord = null;
			}			
		}
		
		Log.d( LOG_TAG, "stopRecording done" );
		return true;
	}

	//========================================================================
	// Helper method which throws an exception when an assertion has failed.
	private static void assertTrue( boolean condition )
	{
		if( !condition )
		{
			throw new AssertionError( "Expected condition to be true" );
		}
	}

	//========================================================================
	// TODO(glaznev): remove this API once SW mic mute can use
	// AudioTrack.setEnabled().
	public static void setMicrophoneMute( boolean mute )
	{
		Log.w( LOG_TAG, "setMicrophoneMute API will be deprecated soon." );
		microphoneMute = mute;
	}
}

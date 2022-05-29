#pragma once
//============================================================================
// Copyright (C) 2003 Brett R. Jones 
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

#include <CoreLib/VxTimer.h>

#define VxSKT_INFO_SAMPLE_CNT 10
class VxSktSampleInfo
{
public:
	double			m_dTime;
	int32_t			m_u32Bytes;
};

class VxSktThrottle
{
public:
	
	VxSktThrottle();

    
    virtual ~VxSktThrottle();

	//=== public methods ===//
	//! return true if throttle is enabled
	bool	                    IsThrottleEnabled( void )				{ return m_bEnableThrottle;}
	//! set throttle enable state
	void	                    EnableThrottle( bool bEnable = true )	{ m_bEnableThrottle = bEnable;}

	//! set receive speed limit in kilobytes per second
	void	                    SetRxBandwidthLimitKbs( float dKbs )		{ m_f32RxSpeedLimit = dKbs; }
	//! set transmit speed limit in kilobytes per second
	void	                    SetTxBandwidthLimitKbs( float dKbs )		{ m_f32TxSpeedLimit = dKbs; }
	//! if transmit speed limit reached return true
	bool	                    TxBandwidthLimitReached()				{ return( m_f32TxSpeedLimit && ( GetTxSpeedKbs() > m_f32TxSpeedLimit ))?1:0; }
	//! if receive speed limit reached return true
	bool	                    RxBandwidthLimitReached()				{ return( m_f32RxSpeedLimit && ( GetRxSpeedKbs() > m_f32RxSpeedLimit ))?1:0; }

	//! get bytes sent	
	virtual int64_t		        GetBytesSent( void )			{ return m_s64BytesTxed; }
	//! get packets sent	
	virtual int64_t		        GetPacketsSent( void )			{ return m_s64TxPktCnt; }
	//! get bytes received	
	virtual int64_t		        GetBytesReceived( void )		{ return m_s64BytesRxed; }
	//! get packets sent	
	virtual int64_t		        GetPacketsReceived( void )		{ return m_s64RxPktCnt; }

	//! reset all statistics
	virtual void	            ResetStatistics( void );
	//! reset just receive statistics
	virtual void	            ResetRxStatistics( void );
	//! reset just send statistics
	virtual void	            ResetTxStatistics( void );
	//! get transmit speed in bits per second
	virtual float		        GetTxSpeed( void )				{ return m_f32TxCurrentSpeed;}
	//! get transmit speed in kilobits per second
	virtual float		        GetTxSpeedKbs( void )			{ return m_f32TxCurrentSpeed/1000.0f;}
	//! get receive speed in bits per second
	virtual float		        GetRxSpeed( void )				{ return m_f32RxCurrentSpeed;}
	//! get receive speed in kilobits per second
	virtual float		        GetRxSpeedKbs( void )			{ return m_f32RxCurrentSpeed/1000.0f;}

	//! packet was sent
	virtual void	            TxedPkt( int iPktLen );
	//! packet was received
	virtual void	            RxedPkt( int iPktLen );
	//! packet could not be sent
	virtual void	            DroppedTxedPkt( int iPktLen );
	//! packet could not be received
	virtual void	            DroppedRxedPkt( int iPktLen );
	//! update current transmit speed
	virtual void	            UpdateTxSpeed( void )			{m_f32TxCurrentSpeed = (float)((double)m_iTxSamplesBytes/m_TxTimer.elapsedSec());}
	//! update current transmit speed
	virtual void	            UpdateRxSpeed( void )			{m_f32RxCurrentSpeed = (float)((double)m_iRxSamplesBytes/m_RxTimer.elapsedSec());}

protected:
	//=== vars == //
	bool			            m_bEnableThrottle;
	VxTimer			            m_TxTimer;
	VxTimer			            m_RxTimer;

	//statistics
	float				        m_f32TxCurrentSpeed;		// current tx speed in bytes per second
	float				        m_f32TxSpeedLimit; 
	float				        m_f32RxCurrentSpeed;		// current tx speed in bytes per second
	float				        m_f32RxSpeedLimit;			
	int				            m_iTxSamplesCnt;			// number of sent packets in samples
	int				            m_iRxSamplesCnt;			// number of received packets in samples
	int				            m_iTxSamplesBytes;			// total bytes in tx sample buffers
	int				            m_iRxSamplesBytes;			// total bytes in rx sample buffers

	int64_t				        m_s64BytesTxed;
	int64_t				        m_s64BytesRxed;

	int64_t				        m_s64TxPktsDropped;
	int64_t				        m_s64RxPktsDropped;
	int64_t				        m_s64TxPktCnt;
	int64_t				        m_s64RxPktCnt;
};


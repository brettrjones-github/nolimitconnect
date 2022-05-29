#pragma once
//============================================================================
// Copyright (C) 2013 Brett R. Jones
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
// also you must link to  ws2_32.lib

#define VXSKT_INFO_SAMPLE_CNT 10
class VxSktSampleInfo
{
public:
	double m_dTime;
	unsigned long m_u32Bytes;
};

class VxSktInfo
{
public:
	VxSktInfo();

	double						ComputeRxSpeedKbs( void );
	double						ComputeTxSpeedKbs( void );
	void						SetRxBandwidthLimitKbs( double dKbs ){ m_dRxSpeedLimit = dKbs; }
	void						SetTxBandwidthLimitKbs( double dKbs ){ m_dTxSpeedLimit = dKbs; }
	bool						TxBandwidthLimitReached() { return( m_dTxSpeedLimit && ( ComputeTxSpeedKbs() >= m_dTxSpeedLimit ))?1:0; }
	bool						RxBandwidthLimitReached() { return( m_dRxSpeedLimit && ( ComputeRxSpeedKbs() >= m_dRxSpeedLimit ))?1:0; }
	double						TxRemainingToLimit();
	double						RxRemainingToLimit();
	void						LogTxBytes( long s32ByteCnt );
	void						LogRxBytes( long s32ByteCnt );
	void						ResetTxInfo( void );
	void						ResetRxInfo( void );

public:

	//statistics
	double	m_dTxSpeedLimit; 
	double	m_dRxSpeedLimit; 
	VxSktSampleInfo m_agTxSamples[VXSKT_INFO_SAMPLE_CNT];
	VxSktSampleInfo m_agRxSamples[VXSKT_INFO_SAMPLE_CNT];
	unsigned long m_u32TxSpeedIdx;
	unsigned long m_u32RxSpeedIdx;

	int64_t		m_i64BytesTxed;
	int64_t		m_i64BytesRxed;

	unsigned long m_u32txPacketsDropped;
	unsigned long m_u32RxPacketsDropped;
	unsigned long m_u32txPacketCnt;
	unsigned long m_u32RxPacketCnt;

	VxTimer m_Timer;
};


#pragma once
//============================================================================
// Copyright (C) 2010 Brett R. Jones 
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

#include "PktTypes.h"
#include "VxCommon.h"

#define PIC_TYPE_JPEG				0x01

enum ETodGameVarId
{
	eTodGameVarIdDareChallengeCnt,
	eTodGameVarIdDareAcceptedCnt,
	eTodGameVarIdDareRejectedCnt,
	eTodGameVarIdTruthChallengeCnt,
	eTodGameVarIdTruthAcceptedCnt,
	eTodGameVarIdTruthRejectedCnt,
	eMaxTodGameStatId,
	eMaxTodGameVarId
};

enum ETodGameAction
{
	eTodGameActionSendStats,
	eTodGameActionChallengeDare,
	eTodGameActionDareAccepted,
	eTodGameActionDareRejected,
	eTodGameActionChallengeTruth,
	eTodGameActionTruthAccepted,
	eTodGameActionTruthRejected,
	eMaxTodGameAction // must be last
};

#pragma pack(push) 
#pragma pack(1)

class PktTodGameStats : public VxPktHdr
{
public:
	PktTodGameStats();

	void						setVar( ETodGameVarId eVarId, int32_t s32Value );
	int32_t							getVar( ETodGameVarId eVarId );

private:
	//=== vars ===//
	int32_t							m_s32DareChallengeCnt;
	int32_t							m_s32DareAcceptedCnt;
	int32_t							m_s32DareRejectedCnt;
	int32_t							m_s32TruthChallengeCnt;
	int32_t							m_s32TruthAcceptedCnt;
	int32_t							m_s32TruthRejectedCnt;
	uint32_t							m_u32Res1;
	uint32_t							m_u32Res2;
	uint32_t							m_u32Res3;
	uint32_t							m_u32Res4;

};

class PktTodGameAction : public VxPktHdr
{
public:
	PktTodGameAction();

	void						setAction( ETodGameAction eAction, int32_t s32Val );
	ETodGameAction				getActionVarId( void );
	int32_t							getActionVarValue( void );

private:
	//=== vars ===//
	int32_t							m_s32ActionId;
	int32_t							m_s32ActionVal;
	uint32_t							m_u32Res1;
	uint32_t							m_u32Res2;
	uint32_t							m_u32Res3;
	uint32_t							m_u32Res4;

};

class PktTodGameValue : public VxPktHdr
{
public:
	PktTodGameValue();

	void						setValue( ETodGameVarId eValueId, int32_t s32Val );

	ETodGameVarId				getValueVarId( void );

	int32_t							getValueVar( void );

private:
	//=== vars ===//
	int32_t							m_s32GameValueId;
	int32_t							m_s32GameValueVar;
	uint32_t							m_u32Res1;
	uint32_t							m_u32Res2;
	uint32_t							m_u32Res3;
	uint32_t							m_u32Res4;

};

#pragma pack()
#pragma pack(pop)

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

#include "TodGameWidget.h"
#include <PktLib/PktsTodGame.h>
#include <PktLib/VxCommon.h>

enum EGameStatus
{
	eWaitingForChallengeRx,
	eRxedDareChallenge,
	eRxedDareAccepted,
	eRxedDareRejected,
	eRxedTruthChallenge,
	eRxedTruthAccepted,
	eRxedTruthRejected,

	eWaitingForChallengeTx,
	eTxedDareChallenge,
	eTxedDareAccepted,
	eTxedDareRejected,
	eTxedTruthChallenge,
	eTxedTruthAccepted,
	eTxedTruthRejected,

	eTxedOffer,
	eOfferRejected,

	eMaxGameStatus // must be last
};

enum EGameButton
{
	eGameButtonDare,
	eGameButtonTruth,
	eMaxGameButton
};

class TodPlayerStats
{
public:
	TodPlayerStats();

	int32_t						getVar( ETodGameVarId eVarId );
	void					setVar( ETodGameVarId eVarId, int32_t s32Value );

	//=== vars ===//
	int32_t						m_s32DareChallengeCnt;
	int32_t						m_s32DareAcceptedCnt;
	int32_t						m_s32DareRejectedCnt;
	int32_t						m_s32TruthChallengeCnt;
	int32_t						m_s32TruthAcceptedCnt;
	int32_t						m_s32TruthRejectedCnt;
};

class TodGameStats
{
public:
	//=== vars ===//
	TodPlayerStats			m_MyPlayerStats;
	TodPlayerStats			m_FriendPlayerStats;
};

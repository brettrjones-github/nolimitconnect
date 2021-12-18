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

#include <AppDependLibrariesConfig.h>

#include "PktsTodGame.h"

#include <CoreLib/VxDebug.h>

//============================================================================
PktTodGameStats::PktTodGameStats()
: m_s32DareChallengeCnt(0)
, m_s32DareAcceptedCnt(0)
, m_s32DareRejectedCnt(0)
, m_s32TruthChallengeCnt(0)
, m_s32TruthAcceptedCnt(0)
, m_s32TruthRejectedCnt(0)
, m_u32Res1(0)
, m_u32Res2(0)
, m_u32Res3(0)
, m_u32Res4(0)
{
	setPktLength( sizeof( PktTodGameStats ) ); 
	setPktType( PKT_TYPE_TOD_GAME_STATS ); 
}

//============================================================================
int32_t PktTodGameStats::getVar( ETodGameVarId eVarId )
{
	switch(eVarId)
	{
	case eTodGameVarIdDareChallengeCnt:
		return ntohl( m_s32DareChallengeCnt );
	case eTodGameVarIdDareAcceptedCnt:
		return ntohl( m_s32DareAcceptedCnt );
	case eTodGameVarIdDareRejectedCnt:
		return ntohl( m_s32DareRejectedCnt );
	case eTodGameVarIdTruthChallengeCnt:
		return ntohl( m_s32TruthChallengeCnt );
	case eTodGameVarIdTruthAcceptedCnt:
		return ntohl( m_s32TruthAcceptedCnt );
	case eTodGameVarIdTruthRejectedCnt:
		return ntohl( m_s32TruthRejectedCnt );
	default:
		return 0;
	}
}

//============================================================================
void PktTodGameStats::setVar( ETodGameVarId eVarId, int32_t s32Value )
{
	switch(eVarId)
	{
	case eTodGameVarIdDareChallengeCnt:
		m_s32DareChallengeCnt = htonl( s32Value );
		break;
	case eTodGameVarIdDareAcceptedCnt:
		m_s32DareAcceptedCnt = htonl( s32Value );
		break;
	case eTodGameVarIdDareRejectedCnt:
		m_s32DareRejectedCnt = htonl( s32Value );
		break;
	case eTodGameVarIdTruthChallengeCnt:
		m_s32TruthChallengeCnt = htonl( s32Value );
		break;
	case eTodGameVarIdTruthAcceptedCnt:
		m_s32TruthAcceptedCnt = htonl( s32Value );
		break;
	case eTodGameVarIdTruthRejectedCnt:
		m_s32TruthRejectedCnt = htonl( s32Value );
		break;
	default:
		break;
	}
}

//============================================================================
PktTodGameAction::PktTodGameAction()
	: m_s32ActionId(0)
	, m_s32ActionVal(0)
	, m_u32Res1(0)
	, m_u32Res2(0)
	, m_u32Res3(0)
	, m_u32Res4(0)
{
	setPktLength( sizeof( PktTodGameAction ) ); 
	setPktType( PKT_TYPE_TOD_GAME_ACTION ); 
}

//============================================================================
void PktTodGameAction:: setAction( ETodGameAction eAction, int32_t s32Val )
{
	m_s32ActionId	= htonl( eAction );
	m_s32ActionVal	= htonl( s32Val );
}

//============================================================================
ETodGameAction PktTodGameAction::getActionVarId( void )
{
	return (ETodGameAction) ntohl( m_s32ActionId );
}

//============================================================================
int32_t PktTodGameAction::getActionVarValue( void )
{
	return ntohl( m_s32ActionVal );
}

//============================================================================
PktTodGameValue::PktTodGameValue()
: m_s32GameValueId(0)
, m_s32GameValueVar(0)
, m_u32Res1(0)
, m_u32Res2(0)
, m_u32Res3(0)
, m_u32Res4(0)
{
	setPktLength( sizeof( PktTodGameValue ) ); 
	setPktType( PKT_TYPE_TOD_GAME_VALUE ); 
}

//============================================================================
void PktTodGameValue::setValue( ETodGameVarId eValueId, int32_t s32Val )
{
	m_s32GameValueId	= htonl( eValueId );
	m_s32GameValueVar	= htonl( s32Val );
}

//============================================================================
ETodGameVarId PktTodGameValue::getValueVarId( void )
{
	return (ETodGameVarId) ntohl( m_s32GameValueId );
}

//============================================================================
int32_t PktTodGameValue::getValueVar( void )
{
	return ntohl( m_s32GameValueVar );
}


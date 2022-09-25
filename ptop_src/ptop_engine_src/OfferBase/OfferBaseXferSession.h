#pragma once
//============================================================================
// Copyright (C) 2021 Brett R. Jones
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

#include <config_appcorelibs.h>

#include "OfferBaseInfo.h"

#include <NetLib/VxFileXferInfo.h>

#include <vector>

class VxSktBase;
class VxNetIdent;
class P2PEngine;
class OfferBaseMgr;

class OfferBaseXferSession
{
public:
	OfferBaseXferSession( P2PEngine& engine, OfferBaseMgr& offerMgr );
	OfferBaseXferSession( P2PEngine& engine, OfferBaseMgr& offerMgr, VxSktBase* sktBase, VxNetIdent* netIdent );
	OfferBaseXferSession( P2PEngine& engine, OfferBaseMgr& offerMgr, VxGUID& lclSessionId, VxSktBase* sktBase, VxNetIdent* netIdent );
	virtual ~OfferBaseXferSession() = default;

	void						setIdent( VxNetIdent * ident )				{ m_Ident = ident; }
	VxNetIdent *				getIdent( void )							{ return m_Ident; }

	void						setOfferInfo( OfferBaseInfo& assetInfo )	{ m_OfferBaseInfo = assetInfo; }
	OfferBaseInfo&				getOfferInfo( void )						{ return m_OfferBaseInfo; }

	void						setSkt( VxSktBase * skt )					{ m_Skt = skt; }
	VxSktBase *					getSkt( void )								{ return m_Skt; }

	void						setLclSessionId( VxGUID& lclId )			{ m_FileXferInfo.setLclSessionId( lclId ); }
	VxGUID&						getLclSessionId( void )						{ return m_FileXferInfo.getLclSessionId(); }
	void						setRmtSessionId( VxGUID& rmtId )			{ m_FileXferInfo.setRmtSessionId( rmtId ); }
	VxGUID&						getRmtSessionId( void )						{ return m_FileXferInfo.getRmtSessionId(); }

	void						setFileHashId( uint8_t * fileHashId )		{ m_FileXferInfo.setFileHashId( fileHashId ); }
	void						setFileHashId( VxSha1Hash& fileHashId )		{ m_FileXferInfo.setFileHashId( fileHashId ); }
	VxSha1Hash&					getFileHashId( void )						{ return m_FileXferInfo.getFileHashId(); }

	VxFileXferInfo&				getXferInfo( void )							{ return m_FileXferInfo; }
	void						setXferDirection( EXferDirection dir )		{ m_FileXferInfo.setXferDirection( dir ); }
	EXferDirection				getXferDirection( void )					{ return m_FileXferInfo.getXferDirection(); }

	void						reset( void );
	bool						isXferingFile( void );

	void						setErrorCode( RCODE error )						{ if( error ) m_Error = error; } 
	RCODE						getErrorCode( void )							{ return m_Error; }
	void						clearErrorCode( void )							{ m_Error = 0; }

	void						setOfferBaseStateSendBegin( void );
	void						setOfferBaseStateSendProgress( int progress );
	void						setOfferBaseStateSendFail( void );
	void						setOfferBaseStateSendCanceled( void );
	void						setOfferBaseStateSendSuccess( void );

	//=== vars ===//
protected:
	P2PEngine&					m_Engine; 
    OfferBaseMgr&               m_OfferMgr;
	VxFileXferInfo				m_FileXferInfo;		// file being transmitted
    int							m_iPercentComplete{ 0 };
	VxSktBase *					m_Skt{ nullptr };
	VxNetIdent *				m_Ident{ nullptr };
	uint32_t					m_Error{ 0 };
	OfferBaseInfo				m_OfferBaseInfo;

private:
	void						initLclSessionId( void );
};

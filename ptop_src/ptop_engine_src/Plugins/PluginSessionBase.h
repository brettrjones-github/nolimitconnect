#pragma once
//============================================================================
// Copyright (C) 2014 Brett R. Jones
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

#include "AudioJitterBuffer.h"

#include <PktLib/VxCommon.h>

#include <CoreLib/VxGUID.h>
#include <CoreLib/VxSha1Hash.h>
#include <CoreLib/VxSemaphore.h>

enum EPluginSessionType
{
	ePluginSessionTypeUnknown,
	ePluginSessionTypeP2P,
	ePluginSessionTypeTx,
	ePluginSessionTypeRx,
	ePluginSessionTypeRelayServer,
	ePluginSessionTypeRelayClient,

	eMaxPluginSessionType
};

class VxSktBase;
class VxPktHdr;
class OpusAudioDecoder;

class PluginSessionBase
{
public:
	PluginSessionBase();
	PluginSessionBase( VxSktBase* sktBase, VxNetIdent* netIdent, EPluginType pluginType );
	PluginSessionBase( VxGUID& lclSessionId, VxSktBase* sktBase, VxNetIdent* netIdent, EPluginType pluginType );
	virtual ~PluginSessionBase();

	virtual void				setPluginType( EPluginType ePluginType );
	virtual EPluginType			getPluginType( void );
	virtual void				setIdent( VxNetIdent* ident );
	virtual VxNetIdent*		getIdent( void );
	virtual const char*		getOnlineName( void );
	virtual VxGUID&				getOnlineId( void );
	virtual void				setSkt( VxSktBase* sktBase );
	virtual VxSktBase *			getSkt( void );
	virtual void				setSessionType( EPluginSessionType sessionType );
	virtual EPluginSessionType	getSessionType( void );
	virtual OpusAudioDecoder *	getAudioDecoder( void ); // will create decoder if doesn't already exist
	virtual AudioJitterBuffer&  getJitterBuffer( void )			{ return m_JitterBuffer; }

	virtual bool				isP2PSession( void );
	virtual bool				isTxSession( void );
	virtual bool				isRxSession( void );

	virtual void				setIsSessionStarted( bool isStarted );
	virtual bool				getIsSessionStarted( void );

	virtual void				setIsRmtInitiated( bool isRmtInitiated );
	virtual bool				isRmtInitiated( void );


	void						setIsInTest( bool bTest )					{ m_bTest = bTest; }
	bool						isInTest( void )							{ return m_bTest; }

	void						setLclSessionId( VxGUID& lclId )			{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )						{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )			{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )						{ return m_RmtSessionId; }

	void						setAssetId( VxGUID& rmtId )					{ m_AssetId = rmtId; }
	VxGUID&						getAssetId( void )							{ return m_AssetId; }

	void						setFileHashId( VxSha1Hash& id )				{ m_FileHashId = id; }
	VxSha1Hash&					getFileHashId( void )						{ return m_FileHashId; }

	void						setOfferResponse( EOfferResponse eResponse ){ m_eOfferResponse = eResponse; }
	EOfferResponse				getOfferResponse( void )					{ return m_eOfferResponse; }

	void						setOfferMsg( const char* msg );
	std::string&				getOfferMsg( void );

	void						setOfferFile( const char* fileName );
	std::string&				getOfferFile( void );

	bool						waitForTestSemaphore( int iMilliseconds )			{ return m_TestSemaphore.wait(iMilliseconds); }
	void						signalTestSemaphore( void )							{ if(m_bTest) m_TestSemaphore.signal(); }

	std::string					m_strOfferMsg;
	std::string					m_strOfferFile;

protected:
	//=== vars ===//
	EPluginType					m_ePluginType;
	VxNetIdent*				m_Ident;
	VxSktBase *					m_Skt;
	EPluginSessionType			m_ePluginSessionType;
	bool						m_bSessionStarted;
	bool						m_bRmtInitiatedSession;
	bool						m_bTest;
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	VxGUID						m_AssetId;
	VxSha1Hash					m_FileHashId;
	EOfferResponse				m_eOfferResponse;
	OpusAudioDecoder *			m_AudioDecoder;
	AudioJitterBuffer			m_JitterBuffer;

	VxSemaphore					m_TestSemaphore; // semaphore used for synchronous tests
};

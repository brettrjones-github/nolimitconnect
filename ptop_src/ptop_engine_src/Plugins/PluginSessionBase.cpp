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

#include "PluginSessionBase.h"

#include <MediaToolsLib/OpusAudioDecoder.h>

//============================================================================
PluginSessionBase::PluginSessionBase()
: m_ePluginType( ePluginTypeInvalid )
, m_Ident(0)
, m_Skt(0)
, m_ePluginSessionType( ePluginSessionTypeUnknown )
, m_bSessionStarted( false )
, m_bRmtInitiatedSession( false )
, m_LclSessionId()
, m_RmtSessionId()
, m_AudioDecoder( 0 )
, m_JitterBuffer( 15 )
{
}

//============================================================================
PluginSessionBase::PluginSessionBase( VxSktBase* sktBase, VxNetIdent* netIdent, EPluginType pluginType )
: m_ePluginType( pluginType )
, m_Ident( netIdent )
, m_Skt( sktBase )
, m_bSessionStarted( false )
, m_bRmtInitiatedSession( false )
, m_bTest( false )
, m_LclSessionId()
, m_RmtSessionId()
, m_AssetId()
, m_eOfferResponse( eOfferResponseNotSet )
, m_AudioDecoder( 0 )
, m_JitterBuffer( 15 )
{
	m_LclSessionId.initializeWithNewVxGUID();
}

//============================================================================
PluginSessionBase::PluginSessionBase( VxGUID& lclSessionId, VxSktBase* sktBase, VxNetIdent* netIdent, EPluginType pluginType )
: m_ePluginType( pluginType )
, m_Ident( netIdent )
, m_Skt( sktBase )
, m_bSessionStarted( false )
, m_bRmtInitiatedSession( false )
, m_bTest( false )
, m_LclSessionId( lclSessionId )
, m_RmtSessionId()
, m_AssetId()
, m_eOfferResponse( eOfferResponseNotSet )
, m_AudioDecoder( 0 )
, m_JitterBuffer( 15 )
{
}

//============================================================================
PluginSessionBase::~PluginSessionBase()
{
	delete m_AudioDecoder;
}

//============================================================================
OpusAudioDecoder *	PluginSessionBase::getAudioDecoder( void )		
{ 
	if( 0 == m_AudioDecoder )
	{
		m_AudioDecoder = new OpusAudioDecoder();
	}

	return m_AudioDecoder;
}

//============================================================================
void PluginSessionBase::setPluginType( EPluginType ePluginType  )		
{ 
	m_ePluginType = ePluginType; 
}

//============================================================================
EPluginType PluginSessionBase::getPluginType( void )
{ 
	return m_ePluginType; 
}

//============================================================================
void PluginSessionBase::setIdent( VxNetIdent * ident )	
{ 
	m_Ident = ident; 
}

//============================================================================
VxNetIdent * PluginSessionBase::getIdent( void )
{ 
	return m_Ident; 
}

//============================================================================
const char * PluginSessionBase::getOnlineName( void )
{ 
	return m_Ident->getOnlineName(); 
}

//============================================================================
VxGUID& PluginSessionBase::getOnlineId( void )
{ 
	return m_Ident->getMyOnlineId(); 
}

//============================================================================
void PluginSessionBase::setSkt( VxSktBase* sktBase )		
{ 
	m_Skt = sktBase; 
}

//============================================================================
VxSktBase * PluginSessionBase::getSkt( void )
{ 
	return m_Skt; 
}

//============================================================================
void PluginSessionBase::setSessionType( EPluginSessionType sessionType )
{
	m_ePluginSessionType = sessionType;
}

//============================================================================
EPluginSessionType PluginSessionBase::getSessionType( void )
{
	return m_ePluginSessionType;
}

//============================================================================
bool  PluginSessionBase::isP2PSession( void )
{
	return m_ePluginSessionType == ePluginSessionTypeP2P ? true : false;
}

//============================================================================
bool  PluginSessionBase::isTxSession( void )
{
	return m_ePluginSessionType == ePluginSessionTypeTx ? true : false;
}

//============================================================================
bool  PluginSessionBase::isRxSession( void )
{
	return m_ePluginSessionType == ePluginSessionTypeRx ? true : false;
}

//============================================================================
void PluginSessionBase::setIsSessionStarted( bool isStarted )	
{ 
	m_bSessionStarted = isStarted; 
}

//============================================================================
bool PluginSessionBase::getIsSessionStarted( void )
{ 
	return m_bSessionStarted; 
}

//============================================================================
void  PluginSessionBase::setIsRmtInitiated( bool isRmtInitiated )
{ 
	m_bRmtInitiatedSession = isRmtInitiated; 
}

//============================================================================
bool  PluginSessionBase::isRmtInitiated( void )					
{ 
	return m_bRmtInitiatedSession; 
}

//============================================================================
void  PluginSessionBase::setOfferMsg( const char * msg )
{
	if( msg )
	{
		m_strOfferMsg = msg;
	}
	else
	{
		m_strOfferMsg = "";
	}
}

//============================================================================
std::string&  PluginSessionBase::getOfferMsg( void )
{
	return m_strOfferMsg;
}

//============================================================================
void  PluginSessionBase::setOfferFile( const char * fileName )
{
	if( fileName )
	{
		m_strOfferFile = fileName;
	}
	else
	{
		m_strOfferFile = "";
	}
}

//============================================================================
std::string&  PluginSessionBase::getOfferFile( void )
{
	return m_strOfferFile;
}


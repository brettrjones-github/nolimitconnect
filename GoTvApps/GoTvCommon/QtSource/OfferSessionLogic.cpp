//============================================================================
// Copyright (C) 2016 Brett R. Jones
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

#include <app_precompiled_hdr.h>
#include "AppCommon.h"
#include "AppletPeerBase.h"
#include "OfferSessionLogic.h"
#include "GuiOfferSession.h"
#include "OfferSessionCallbackInterface.h"
#include "OffersMgr.h"
#include "OfferSessionState.h"
#include "GuiHelpers.h"
#include "GuiParams.h"

#include <CoreLib/VxGlobals.h>

#include <stdio.h>

//======================================================================== 
OfferSessionLogic::OfferSessionLogic( AppletBase* appletBase, OfferSessionCallbackInterface * callbackInterface, AppCommon& myApp )
: m_MyApp( myApp )
, m_OffersMgr( myApp.getOffersMgr() )
, m_AppletBase( appletBase )
, m_OfferCallback( callbackInterface )
, m_MyIdent( myApp.getMyIdentity() )
, m_LclSessionId()
, m_RmtSessionId()
, m_OfferSessionId()
, m_strOfferText( "" )
, m_strOfferFileName(  "" )
{
    /*
	m_IsMyself 					= (m_MyIdent->getMyOnlineId() == m_HisIdent->getMyOnlineId() );
	if( 0 != m_GuiOfferSession )
	{
		m_IsOffer			= true;
		m_LclSessionId		= m_GuiOfferSession->assuredValidLclSessionId();
		m_OfferSessionId	= m_GuiOfferSession->getOfferSessionId();
	}
	else
	{
		m_IsOffer = false;
		m_GuiOfferSession = new GuiOfferSession();

		m_GuiOfferSession->setPluginType( ePluginType );
		m_GuiOfferSession->setHisIdent( hisIdent );
		m_GuiOfferSession->setIsRemoteInitiated( false );
		m_LclSessionId		= m_GuiOfferSession->assuredValidLclSessionId();
		m_OfferSessionId	= m_GuiOfferSession->getOfferSessionId();
	}

	m_IsPluginSingleSession = GuiHelpers::isPluginSingleSession( m_ePluginType );
	m_UserData = m_GuiOfferSession->getUserData();
	m_FileHashId.setHashData( m_GuiOfferSession->getFileHashId().getHashData() );
	m_strOfferText = GuiParams::describePlugin( m_ePluginType, false );
    */
}

//======================================================================== 
bool OfferSessionLogic::isOurSessionType( GuiOfferSession * offerSession )
{
	if( ( offerSession->getPluginType() == m_ePluginType )
		&& ( 0 != m_HisIdent ) )
	{
		return true;
	}

	return false;
}

//======================================================================== 
bool OfferSessionLogic::isOurSessionInstance( GuiOfferSession * offerSession )
{
	if( isOurSessionType( offerSession ) )
	{
		if( GuiHelpers::isPluginSingleSession( offerSession->getPluginType() ) )
		{
			if(  offerSession->getHisIdent()->getMyOnlineId() ==  m_HisIdent->getMyOnlineId() )
			{
				return true;
			}				
		}
		else if(  offerSession->getOfferSessionId() == m_OfferSessionId ) 
		{
			return true;
		}
	}

	return false;
}

//======================================================================== 
void OfferSessionLogic::onInSession( bool isInSession )
{
	std::string sessionMsg = isInSession ? "In " : "Ended ";
	sessionMsg += GuiParams::describePlugin( m_ePluginType, m_IsOffer );
	sessionMsg += " Session";
	m_MyApp.toGuiStatusMessage( sessionMsg.c_str() );

	m_OfferCallback->onInSession( isInSession );
	m_OffersMgr.onIsInSession( m_ePluginType, m_OfferSessionId, m_HisIdent, isInSession );
}

//======================================================================== 
void OfferSessionLogic::doToGuiRxedPluginOffer( GuiOfferSession * offerSession ) 
{
	if( isOurSessionType( offerSession ) )
	{			
		if( GuiHelpers::isPluginSingleSession( m_ePluginType ) )
		{
			// special case for multisession and other single session use online id instead of session id and auto accept
			if( offerSession->getHisIdent()->getMyOnlineId() == m_HisIdent->getMyOnlineId() )
			{
				if( offerSession->getOfferSessionId().isVxGUIDValid() )
				{
					m_OfferSessionId = offerSession->getOfferSessionId();
				}

				m_OffersMgr.removePluginSessionOffer( m_ePluginType, m_HisIdent );
				// already in session with this user
				m_MyApp.getEngine().fromGuiToPluginOfferReply( 	m_ePluginType, 
																m_HisIdent->getMyOnlineId(), 
																offerSession->getUserData(), 
																eOfferResponseAccept,
																m_OfferSessionId );
				if( !m_IsInSession )
				{
					m_IsInSession = true;
					m_MyApp.getEngine().fromGuiStartPluginSession( 	m_ePluginType, 
						m_HisIdent->getMyOnlineId(), 
						offerSession->getUserData(),
						m_OfferSessionId );
					onInSession( true );
				}
			}
			else
			{
				// already in session with someone else
				if( false == offerSession->getOfferSessionId().isVxGUIDValid() )
				{
					m_MyApp.getEngine().fromGuiToPluginOfferReply(	m_ePluginType, 
						offerSession->getHisIdent()->getMyOnlineId(), 
						offerSession->getUserData(), 
						eOfferResponseBusy,
						offerSession->getHisIdent()->getMyOnlineId() );	

					m_OffersMgr.sentOfferReply( m_ePluginType, offerSession->getHisIdent()->getMyOnlineId(), offerSession->getHisIdent(), eOfferResponseBusy );
				}
				else
				{
					m_MyApp.getEngine().fromGuiToPluginOfferReply(	m_ePluginType, 
						offerSession->getHisIdent()->getMyOnlineId(), 
						offerSession->getUserData(), 
						eOfferResponseBusy,
						offerSession->getOfferSessionId() );									
					m_OffersMgr.sentOfferReply( m_ePluginType, offerSession->getOfferSessionId(), offerSession->getHisIdent(), eOfferResponseBusy );					
				}
			}
		}
		else
		{
			if( offerSession->getOfferSessionId() == m_OfferSessionId )
			{
				// already in session with this user
				m_MyApp.getEngine().fromGuiToPluginOfferReply( 	m_ePluginType, 
																m_HisIdent->getMyOnlineId(), 
																offerSession->getUserData(), 
																eOfferResponseAccept,
																m_OfferSessionId );
				if( !m_IsInSession )
				{
					m_IsInSession = true;
					m_MyApp.getEngine().fromGuiStartPluginSession( 	m_ePluginType, 
																	m_HisIdent->getMyOnlineId(), 
																	offerSession->getUserData(),
																	m_OfferSessionId );
					onInSession( true );
				}
			}
			else
			{
				// already in session with someone else
				if( false == offerSession->getOfferSessionId().isVxGUIDValid() )
				{
					m_MyApp.getEngine().fromGuiToPluginOfferReply(	m_ePluginType, 
						offerSession->getHisIdent()->getMyOnlineId(), 
						offerSession->getUserData(), 
						eOfferResponseBusy,
						offerSession->getHisIdent()->getMyOnlineId() );	

					m_OffersMgr.sentOfferReply( m_ePluginType, offerSession->getHisIdent()->getMyOnlineId(), offerSession->getHisIdent(), eOfferResponseBusy );
				}
				else
				{
					m_MyApp.getEngine().fromGuiToPluginOfferReply(	m_ePluginType, 
						offerSession->getHisIdent()->getMyOnlineId(), 
						offerSession->getUserData(), 
						eOfferResponseBusy,
						offerSession->getOfferSessionId() );									
					m_OffersMgr.sentOfferReply( m_ePluginType, offerSession->getOfferSessionId(), offerSession->getHisIdent(), eOfferResponseBusy );					
				}
			}
		}
	}
}

//======================================================================== 
void OfferSessionLogic::doToGuiRxedOfferReply( GuiOfferSession * offerSession ) 
{
	if( isOurSessionType( offerSession ) )
	{			
		if( isOurSessionInstance( offerSession ) )
		{
			EOfferResponse offerResponse		= offerSession->getOfferResponse();			
            //VxGUID lclSessionId					= offerSession->getLclSessionId();
			VxGUID offerSessionId				= offerSession->getOfferSessionId();			
			if( eOfferResponseAccept == offerResponse )
			{
				m_MyApp.playSound( eSndDefOfferAccepted );
				postStatusMsg( (getHisOnlineName() + " Accepted Offer " + describePlugin()).c_str() );

				if( !m_IsInSession )
				{
					m_IsInSession = true;
					m_MyApp.getEngine().fromGuiStartPluginSession( m_ePluginType, m_HisIdent->getMyOnlineId(),
						offerSession->getUserData(),
						offerSessionId );
					m_OffersMgr.startedSessionInReply( m_ePluginType, offerSessionId, m_HisIdent );
					onInSession( true );
				}
			}
			else 
			{
				handleSessionEnded( offerResponse );
			}

			if( GuiHelpers::isPluginSingleSession( m_ePluginType ) )
			{
				m_OffersMgr.removePluginSessionOffer( m_ePluginType, m_HisIdent );
			}
		}
	}
}

//======================================================================== 
void OfferSessionLogic::toGuiPluginSessionEnded( GuiOfferSession * offerSession ) 
{
	/*
	if( isOurSessionInstance( offerSession ) )
	{
		EOfferResponse offerResponse = offerSession->getOfferResponse();	

		m_OffersMgr.recievedSessionEnd( m_ePluginType, m_OfferSessionId, m_HisIdent, offerResponse );
		handleSessionEnded( offerResponse );
	}
	*/
}

//======================================================================== 
void OfferSessionLogic::toGuiContactOffline( VxNetIdent * friendIdent ) 
{
	if( ( 0 != m_HisIdent )
		&& ( friendIdent->getMyOnlineId() == m_HisIdent->getMyOnlineId() ) )
	{
		//m_OffersMgr.contactWentOffline( m_HisIdent );
		handleSessionEnded( eOfferResponseUserOffline );
	}
}

//========================================================================
bool OfferSessionLogic::sendOfferOrResponse()
{
	if( getIsServerSession() )
	{
		return true;
	}

	if( m_ePluginType < eMaxUserPluginType )
	{
		bool sentOk = true;
		if( m_IsOffer )
		{
			if (false == m_MyApp.getEngine().fromGuiToPluginOfferReply(	m_ePluginType, 
																		m_HisIdent->getMyOnlineId(),
																		getUserData(), 
																		eOfferResponseAccept,
																		m_OfferSessionId ) )
			{
				if( ePluginTypeMessenger != m_ePluginType )
				{
					showOfflineMsg();
				}
				else
				{
					postStatusMsg( "%s is offline-chat disabled", m_HisIdent->getOnlineName() );
				}

				sentOk = false;
			}
			else
			{
				startPluginSessionIfIsSessionOffer();
			}
		}
		else
		{
			if (false == m_MyApp.getEngine().fromGuiMakePluginOffer(	m_ePluginType, 
																		m_HisIdent->getMyOnlineId(), 
																		getUserData(), 
																		getOfferText().c_str(), 
																		getOfferFileName().c_str(),
																		m_FileHashId.getHashData(),
																		m_OfferSessionId ) )
			{
				if( ePluginTypeMessenger != m_ePluginType )
				{
					showOfflineMsg();
				}
				else
				{
					postStatusMsg( "%s is offline-chat disabled", m_HisIdent->getOnlineName() );
				}

				sentOk = false;
			}
			else
			{
				m_OffersMgr.sentOffer( m_ePluginType, m_OfferSessionId, m_HisIdent );
				postStatusMsg( "Waiting Reply Offer %s From %s", describePlugin().c_str(),  getHisOnlineName().c_str() );
			}
		}

		return sentOk;
	}

	return true;
}

//============================================================================
bool OfferSessionLogic::sendOfferReply( EOfferResponse offerResponse )
{
	if( m_ePluginType < eMaxUserPluginType )
	{	
		m_GuiOfferSession->setOfferResponse( offerResponse );
		bool bSentMsg = m_MyApp.getEngine().fromGuiToPluginOfferReply(	getPluginType(), 
																		getHisIdent()->getMyOnlineId(), 
																		getUserData(),
																		offerResponse,
																		getOfferSessionId() );
		if( false == bSentMsg )
		{
			handleUserWentOffline();
		}
		else
		{
			m_AppletBase->setStatusText( QObject::tr( "Sent Offer Reply" ) );
		}

		return bSentMsg;
	}
	
	return true;
}

//============================================================================  
void OfferSessionLogic::onStop()
{
	if( false == m_IsOnStopCalled && m_HisIdent )
	{
		m_IsOnStopCalled = true;
		m_OffersMgr.onSessionExit( m_ePluginType, m_OfferSessionId, m_HisIdent );
		if( false == getIsServerSession() )
		{
			setCallState( eCallStateHangedUp );
			if( false == m_SessionEndIsHandled )
			{
				m_SessionEndIsHandled = true;
				setIsInSession( false );
				if( 0 != m_HisIdent )
				{
					if( m_ePluginType < eMaxUserPluginType )
					{
						m_MyApp.getEngine().fromGuiToPluginOfferReply( 	m_ePluginType, 
							m_HisIdent->getMyOnlineId(), 
							0, 
							eOfferResponseEndSession, 
							m_OfferSessionId );
						m_OffersMgr.sentOfferReply( m_ePluginType, m_OfferSessionId, m_HisIdent, eOfferResponseEndSession );
					}
				}
			}

			if( 0 != m_HisIdent )
			{
				if( m_ePluginType < eMaxUserPluginType )
				{
					m_MyApp.getEngine().fromGuiStopPluginSession( 	m_ePluginType, 
																	m_HisIdent->getMyOnlineId(),
																	getUserData(),
																	m_OfferSessionId
																	);		   
				}
			}
		}

		postStatusMsg( (describePlugin() + " with " + getHisOnlineName() + " Has Ended ").c_str() );
	}
	else
	{
		LogMsg( LOG_INFO, "OfferSessionLogic::onStop was already called \n" );
	}

}

//============================================================================  
bool OfferSessionLogic::startPluginSessionIfIsSessionOffer()
{
	if( isSessionOffer() )
	{
		setIsInSession( true );
		m_MyApp.getEngine().fromGuiStartPluginSession( 	m_ePluginType, 
														m_HisIdent->getMyOnlineId(),
														getUserData(),
														m_OfferSessionId );
		m_OffersMgr.startedSessionInReply( m_ePluginType, m_OfferSessionId, m_HisIdent );
		onInSession( true );
		return true;
	}

	return false;
}

//============================================================================  
void OfferSessionLogic::setCallState( int eCallState )
{
	switch( eCallState )
	{
	case eCallStateCalling:
		startPhoneRinging();
		break;

	case eCallStateInCall:
		break;

	case eCallStateHangedUp:
		stopPhoneRinging();
		break;

	default:
		break;
	}
}

//========================================================================
bool OfferSessionLogic::handleOfferResponse( GuiOfferSession  poSession )
{
	bool bResponesOk = false;
	if( ( poSession.getPluginType() == m_ePluginType ) &&
		( poSession.getHisIdent()->getMyOnlineId() == m_HisIdent->getMyOnlineId() )) 
	{
		stopPhoneRinging();
		bResponesOk = handleOfferResponseCode( poSession.getOfferResponse()  );
	}

	return bResponesOk;
}

//========================================================================
bool OfferSessionLogic::handleOfferResponseCode( EOfferResponse responseCode )
{
	bool responesOk = false;
	if( responseCode != eOfferResponseAccept )
	{
		std::string strExitReason =  this->describeResponse( responseCode );
		if( GuiHelpers::isPluginSingleSession( m_ePluginType ) )
		{
			m_OfferCallback->onSessionActivityShouldExit( strExitReason.c_str() );
		}
	}
	else
	{
		responesOk = true;
	}

	return responesOk;
}

//============================================================================   
void OfferSessionLogic::handleSessionEnded( GuiOfferSession * offerSession )
{
	if( isOurSessionInstance( offerSession ) ) 
	{
		handleSessionEnded( offerSession->getOfferResponse() );
	}
}

//============================================================================
std::string OfferSessionLogic::getHisOnlineName( void )
{
	return m_HisIdent->getOnlineName();
}

//============================================================================
std::string OfferSessionLogic::describePlugin( void )
{
	return GuiParams::describePlugin( m_ePluginType, m_IsOffer );
}

//============================================================================
void OfferSessionLogic::postStatusMsg( const char * statusMsg, ... )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	char szBuffer[2048];
	szBuffer[0] = 0;
	va_list arg_ptr;
	va_start(arg_ptr, statusMsg);
#ifdef TARGET_OS_WINDOWS
    vsnprintf(szBuffer, 2048, statusMsg,(char *) arg_ptr);
#else
    vsnprintf(szBuffer, 2048, statusMsg, arg_ptr);
#endif // TARGET_OS_WINDOWS
	szBuffer[2047] = 0;
	va_end(arg_ptr);

	if( 0 != szBuffer[0] )
	{
		m_MyApp.toGuiStatusMessage( szBuffer );
	}
}

//============================================================================
void OfferSessionLogic::postUserMsg( const char * userMsg, ... )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	char szBuffer[2048];
	szBuffer[0] = 0;
	va_list arg_ptr;
	va_start(arg_ptr, userMsg);
#ifdef TARGET_OS_WINDOWS
    vsnprintf(szBuffer, 2048, userMsg,(char *) arg_ptr);
#else
    vsnprintf(szBuffer, 2048, userMsg, arg_ptr);
#endif // TARGET_OS_WINDOWS
	szBuffer[2047] = 0;
	va_end(arg_ptr);

	if( 0 != szBuffer[0] )
	{
		m_MyApp.toGuiUserMessage( szBuffer );
	}
}

//============================================================================
std::string OfferSessionLogic::describeResponse( EOfferResponse eOfferResponse )
{
	std::string responseText = m_HisIdent->getOnlineName();
	responseText += " ";

	switch( eOfferResponse )
	{
	case eOfferResponseAccept:
		responseText += "Accepted Offer ";
		break;

	case eOfferResponseReject:
		responseText += "Rejected Offer ";
		break;

	case eOfferResponseBusy:
		responseText += "Is Unavailable for ";
		break;

	case eOfferResponseCancelSession:
		responseText += "Canceled Session ";
		break;

	case eOfferResponseEndSession:
		responseText += "Ended Session ";
		break;
	case eOfferResponseUserOffline:
		responseText += "Is Offline And Cannot ";
		break;
	default:
		responseText += "Ended Session ";
	}

	responseText += GuiParams::describePlugin( m_ePluginType, m_IsOffer );
	return responseText;
}

//========================================================================
void OfferSessionLogic::handleSessionEnded( EOfferResponse responseCode )
{
	if( false == m_SessionEndIsHandled )
	{
		m_SessionEndIsHandled = true;
		std::string responseDesc = this->describeResponse( responseCode );

		setCallState( eCallStateHangedUp );
		if( eOfferResponseBusy == responseCode )
		{
			if( ePluginTypeCamServer != m_ePluginType )
			{
				m_MyApp.playSound( eSndDefBusy );
			}
		}

		if( m_IsInSession )
		{
			m_IsInSession = false;
			onInSession( false );
			m_MyApp.getEngine().fromGuiStopPluginSession( m_ePluginType, m_HisIdent->getMyOnlineId(), getUserData(), m_OfferSessionId );
		}
		
		postStatusMsg( responseDesc.c_str() );
		if( ( ePluginTypeMessenger != m_ePluginType )
			&& ( ePluginTypeCamServer != m_ePluginType ) )
		{
			if( GuiHelpers::isPluginSingleSession( m_ePluginType ) )
			{
				m_OfferCallback->onSessionActivityShouldExit( responseDesc.c_str() );
			}
		}
	}
} 

//========================================================================
void OfferSessionLogic::handleUserWentOffline()
{
	setCallState( eCallStateHangedUp );
	showOfflineMsg( true );
}

//========================================================================
void OfferSessionLogic::startPhoneRinging()
{

}

//========================================================================
void OfferSessionLogic::stopPhoneRinging()
{

}
//========================================================================
void OfferSessionLogic::showOfflineMsg( bool bExitWhenClicked )
{
	std::string offlineMsg = m_HisIdent->getOnlineName();
	offlineMsg += " Is Offline.";
	m_MyApp.toGuiStatusMessage( offlineMsg.c_str() );
	//QMessageBox::information( m_Activity, "User Offline", offlineMsg.c_str(), QMessageBox::Ok );
	if( bExitWhenClicked )
	{
		m_OfferCallback->onSessionActivityShouldExit( offlineMsg.c_str() );
	}
}

/*
//============================================================================
//! show error message box to user
void OfferSessionLogic::errMsgBox( bool bExitWhenClicked, std::string strMsg )
{
	QMessageBox::information( m_Activity, "Session Error Message", strMsg.c_str(), QMessageBox::Ok );
	if( bExitWhenClicked )
	{
		m_OfferCallback->onSessionActivityShouldExit();
	}
}*/







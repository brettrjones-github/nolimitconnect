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
// http://www.nolimitconnect.com
//============================================================================

#include "ui_AppletMultiMessenger.h"
#include "AppletPeerBase.h"
#include "MultiSessionState.h"
#include "TodGameLogic.h"

class P2PEngine;
class GuiOfferSession;
class EngineSettings;
class MultiSessionState;

class AppletMultiMessenger : public AppletPeerBase, public ToGuiActivityInterface, public OfferSessionCallbackInterface
{
	Q_OBJECT
public:
	AppletMultiMessenger(	AppCommon&			    app,
							QWidget *				parent = nullptr );

	virtual ~AppletMultiMessenger();

	virtual void				toGuiClientPlayVideoFrame(	void *			userData, 
															VxGUID&			onlineId, 
                                                            uint8_t *		pu8Jpg,
                                                            uint32_t		u32JpgDataLen,
                                                            int				motion0To100000 ) override;
    virtual void				toGuiMultiSessionAction( void * callbackData, EMSessionAction mSessionAction, VxGUID& onlineId, int pos0to100000 ) override;

    virtual void 				onSessionActivityShouldExit( QString shouldExitReason ) override {};

    virtual void				onActivityFinish( void ) override;

signals:
	void						signalToGuiMultiSessionAction( VxGuidQt onlineId, EMSessionAction mSessionAction, int pos );

protected slots:
	void						slotToGuiMultiSessionAction( VxGuidQt onlineId, EMSessionAction mSessionAction, int pos );
	void						slotToGuiPluginSessionEnded(GuiOfferSession * offerSession );
	void						slotToGuiContactOnlineMultisession( VxNetIdent * hisIdent, bool newContact );
	void						slotToGuiContactOfflineMultisession( VxNetIdent * hisIdent );

	void						slotUserInputButtonClicked( void );

protected:
    virtual void				showEvent( QShowEvent * ev ) override;
    virtual void				hideEvent( QHideEvent * ev ) override;

	// override of ToGuiActivityInterface
    virtual void				doToGuiRxedPluginOffer( void * callbackData, GuiOfferSession * offer ) override;
    virtual void				doToGuiRxedOfferReply( void * callbackData, GuiOfferSession * offer ) override;

	virtual bool 				checkForSendAccess( bool sendOfferIfPossible );
	virtual void				showReasonAccessNotAllowed( void );

    virtual void 				onSessionStateChange( ESessionState eSessionState ) override;
	// called from session logic
    virtual void				onInSession( bool isInSession ) override;

	void						setupMultiSessionActivity(  VxNetIdent * hisIdent );
	void						setStatusMsg( QString strStatus );

	void						toGuiSetGameValueVar(	void *		    userData, 
														EPluginType     ePluginType, 
														VxGUID&		    onlineId, 
														int32_t			s32VarId, 
                                                        int32_t			s32VarValue ) override;

	void						toGuiSetGameActionVar(	void *		    userData, 
														EPluginType     ePluginType, 
														VxGUID&		    onlineId, 
														int32_t			s32VarId, 
                                                        int32_t			s32VarValue ) override;
	void						setup( void );
	//virtual bool				handleOfferResponse( EOfferResponse offerResponse, QWidget * parent );
    //void						onInSessionResponse( bool bResponseOk );

	MultiSessionState *			getMSessionState( EMSessionType sessionType );

	//=== vars ===//
	Ui::AppletMultiMessengerUi	ui;
    TodGameLogic				m_TodGameLogic;
    QVector<MultiSessionState*> m_MSessionsList;
    QFrame *					m_OffersFrame{ nullptr };
	QFrame *					m_ResponseFrame{ nullptr };
	QFrame *					m_HangupSessionFrame{ nullptr };
	TodGameWidget *				m_TodGameWidget{ nullptr };
	VidWidget *					m_VidChatWidget{ nullptr };
	bool						m_IsInitialized{ false };
	bool						m_OfferOrResponseIsSent{ false };
	bool						m_CanSend{ false };
};

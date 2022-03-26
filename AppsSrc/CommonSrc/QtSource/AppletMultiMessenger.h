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

class AppletMultiMessenger : public AppletPeerBase
{
	Q_OBJECT
public:
	AppletMultiMessenger(	AppCommon&			    app,
							QWidget *				parent = nullptr );

	virtual ~AppletMultiMessenger();

	virtual void				toGuiClientPlayVideoFrame(	VxGUID&			onlineId, 
                                                            uint8_t *		pu8Jpg,
                                                            uint32_t		u32JpgDataLen,
                                                            int				motion0To100000 ) override;
    virtual void				toGuiMultiSessionAction( EMSessionAction mSessionAction, VxGUID& onlineId, int pos0to100000 ) override;

    virtual void 				onSessionActivityShouldExit( QString shouldExitReason ) override {};

    virtual void				onActivityFinish( void ) override;

protected slots:
    void                        slotUserOnlineStatus( GuiUser* user, bool isOnline );

    void						slotEyeHostButtonClicked( void );
    void						slotEyeSessionButtonClicked( void );

    void						slotFriendsButtonClicked( void );
    void						slotGroupHostButtonClicked( void );
    void						slotChatRoomHostButtonClicked( void );
    void						slotRandomConnectHostButtonClicked( void );

    void						slotOfferFileButtonClicked( void );

	void						slotToGuiPluginSessionEnded(GuiOfferSession * offerSession );

	void						slotUserInputButtonClicked( void );

protected:
    virtual void				showEvent( QShowEvent * ev ) override;
    virtual void				hideEvent( QHideEvent * ev ) override;

	// override of ToGuiActivityInterface
    virtual void				toToGuiRxedPluginOffer( GuiOfferSession * offer ) override;
    virtual void				toToGuiRxedOfferReply( GuiOfferSession * offer ) override;

	virtual bool 				checkForSendAccess( bool sendOfferIfPossible );
	virtual void				showReasonAccessNotAllowed( void );

    virtual void 				onSessionStateChange( ESessionState eSessionState ) override;
	// called from session logic
    virtual void				onInSession( bool isInSession ) override;

	void						setupMultiSessionActivity( GuiUser * hisIdent );
	void						setStatusMsg( QString strStatus );

	void						toGuiSetGameValueVar(	EPluginType     ePluginType, 
														VxGUID&		    onlineId, 
														int32_t			s32VarId, 
                                                        int32_t			s32VarValue ) override;

	void						toGuiSetGameActionVar(	EPluginType     ePluginType, 
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


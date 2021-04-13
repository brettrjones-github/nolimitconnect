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
// http://www.nolimitconnect.com
//============================================================================

#include <GoTvInterface/IDefs.h>

#include <CoreLib/VxGUID.h>
#include <CoreLib/VxMutex.h>

#include <QObject>

class AppCommon;
class GuiOfferSession;
class GuiUser;
class OfferSessionState;
class ToGuiOfferInterface;

class GuiOfferMgr : public QObject
{
    Q_OBJECT
public:
    GuiOfferMgr() = delete;
    GuiOfferMgr( AppCommon& app );
    GuiOfferMgr( const GuiOfferMgr& rhs ) = delete;
	virtual ~GuiOfferMgr() = default;

    void                        onAppCommonCreated( void );

    void						lockOfferMgr( void )						{ m_OfferMgrMutex.lock(); }
    void						unlockOfferMgr( void )						{ m_OfferMgrMutex.unlock(); }
    int							getNewOffersCount( void )					{ return 0; }
    size_t						getTotalOffersCount( void )					{ return m_aoOffersList.size(); }

    std::map<VxGUID, GuiOfferSession*> getCallList( void ) { return m_CallOfferCache; }
    std::map<VxGUID, GuiOfferSession*> getOfferList( void ) { return m_OfferListCache; }

    void						toGuiRxedPluginOffer( GuiOfferSession * offerSession ) {};
    void						toGuiRxedOfferReply( GuiOfferSession * offerSession ) {};
    void						toGuiPluginSessionEnded( GuiOfferSession * offerSession ) {};

    virtual OfferSessionState *	getTopGuiOfferSession( void ) { return nullptr; };
    void				        wantToGuiOfferCallbacks( ToGuiOfferInterface * clientInterface, bool wantCallbacks ) {};

    void						onIsInSession( EPluginType ePluginType, VxGUID offerSessionId, GuiUser * hisIdent, bool isInSession ) {};
    void						onSessionExit( EPluginType ePluginType, VxGUID offerSessionId, GuiUser * hisIdent ) {};
    void						startedSessionInReply( EPluginType ePluginType, VxGUID offerSessionId, GuiUser * hisIdent ) {};

    void						acceptOfferButtonClicked( EPluginType ePluginType, VxGUID offerSessionId, GuiUser * hisIdent ) {};
    void						rejectOfferButtonClicked( EPluginType ePluginType, VxGUID offerSessionId, GuiUser * hisIdent ) {};

    // called if starting new session to know if responding to existing offer
    GuiOfferSession *			findActiveAndAvailableOffer( GuiUser * netIdent, EPluginType ePluginType ) {};
    void						sentOffer( EPluginType ePluginType, VxGUID offerSessionId, GuiUser * hisIdent ) {};
    void						sentOfferReply( EPluginType ePluginType, VxGUID offerSessionId, GuiUser * hisIdent, EOfferResponse eOfferResponse ) {};
    void						removePluginSessionOffer( EPluginType ePluginType, GuiUser * netIdent ) {};
    void						removePluginSessionOffer( VxGUID& offerSessionId ) {};

protected:
    
    AppCommon&                  m_MyApp;
    VxMutex 					m_OfferMgrMutex; 
    std::vector<OfferSessionState *> m_aoOffersList;	
    std::vector<ToGuiOfferInterface *> m_OfferCallbackList;	

    std::map<VxGUID, GuiOfferSession*> m_CallOfferCache;
    std::map<VxGUID, GuiOfferSession*> m_OfferListCache;
};

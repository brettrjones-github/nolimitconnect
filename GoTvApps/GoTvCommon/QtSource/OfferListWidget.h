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

#include <CoreLib/VxTimer.h>
#include <CoreLib/VxGUID.h>

#include <QListWidget>

enum EOfferViewType
{
    eOfferViewTypeNone,
    eOfferViewTypeFriends,
    eOfferViewTypeGroup,
    eOfferViewTypeChatRoom,
    eOfferViewTypeRandomConnect,
    eOfferViewTypeEverybody,
    eOfferViewTypeIgnored,

    eMaxOfferViewTypeNone,
};

class AppCommon;
class GuiOffer;
class GuiOfferMgr;
class GuiOfferSession;
class MyIcons;
class OfferListItem;
class P2PEngine;

class OfferListWidget : public QListWidget
{
	Q_OBJECT

public:
	OfferListWidget( QWidget * parent );

	AppCommon&					getMyApp( void ) { return m_MyApp; }
	MyIcons&					getMyIcons( void );
    void                        clearOfferList( void );

    void                        addOfferToList( EHostType hostType, VxGUID& sessionId,  GuiOffer * userIdent );
    OfferListItem*              addOrUpdateOfferSession( GuiOfferSession* hostSession );

    GuiOfferSession*            findSession( VxGUID& lclSessionId );
    OfferListItem*              findListEntryWidgetBySessionId( VxGUID& sessionId );
    OfferListItem*              findListEntryWidgetByOnlineId( VxGUID& onlineId );

    void                        updateOffer( GuiOfferSession * user );
    void                        removeOffer( VxGUID& onlineId );

    void                        setShowMyself( bool show )                  { m_ShowMyself = show; }
    bool                        getShowMyself( void )                       { return m_ShowMyself; }

    void                        setOfferViewType( EOfferViewType viewType );
    EOfferViewType              getOfferViewType( void )                     { return m_OfferViewType; }

signals:
    void                        signalOfferListItemClicked( GuiOfferSession* hostSession, OfferListItem* hostItem );

    void                        signalAvatarButtonClicked( GuiOfferSession* hostSession, OfferListItem* hostItem );
    void                        signalMenuButtonClicked( GuiOfferSession* hostSession, OfferListItem* hostItem );
    void                        signalFriendshipButtonClicked( GuiOfferSession* hostSession, OfferListItem* hostItem );

protected slots:

    /*
    void				        slotOfferAdded( GuiOffer* user ); 
    void				        slotOfferRemoved( VxGUID onlineId ); 
    void                        slotOfferUpdated( GuiOffer* user );
    void                        slotOfferOnlineStatus( GuiOffer* user, bool isOnline );
    */

    void                        slotOfferListItemClicked( OfferListItem* hostItem );
    void                        slotAvatarButtonClicked( OfferListItem* hostItem );
    void                        slotMenuButtonClicked( OfferListItem* hostItem );
    void                        slotFriendshipButtonClicked( OfferListItem* hostItem );

protected:
    OfferListItem*              sessionToWidget( GuiOfferSession* hostSession );
    GuiOfferSession*		    widgetToSession( OfferListItem* hostItem );

    virtual void                onOfferListItemClicked( OfferListItem* hostItem );
    virtual void                onAvatarButtonClicked( OfferListItem* hostItem );
    virtual void                onFriendshipButtonClicked( OfferListItem* hostItem );
    virtual void                onMenuButtonClicked( OfferListItem* hostItem );

    void                        refreshList( void );
    bool                        isListViewMatch( GuiOfferSession * user );

	//=== vars ===//
	AppCommon&					m_MyApp;
    GuiOfferMgr&				m_OfferMgr;
	P2PEngine&					m_Engine;
	VxTimer						m_ClickEventTimer; // avoid duplicate clicks
    bool                        m_ShowMyself{ true };
    EOfferViewType              m_OfferViewType{ eOfferViewTypeNone };

    std::map<VxGUID, GuiOfferSession*> m_CallCache;
    std::map<VxGUID, GuiOfferSession*> m_OfferCache;
};


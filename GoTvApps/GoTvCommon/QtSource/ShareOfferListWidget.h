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

#include <GuiInterface/IDefs.h>

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
class GuiOfferClientMgr;
class GuiOfferSession;
class MyIcons;
class ShareOfferListItem;
class P2PEngine;

class ShareOfferListWidget : public QListWidget
{
	Q_OBJECT

public:
	ShareOfferListWidget( QWidget * parent );

	AppCommon&					getMyApp( void ) { return m_MyApp; }
	MyIcons&					getMyIcons( void );
    void                        clearOfferList( void );

    void                        addOfferToList( EHostType hostType, VxGUID& sessionId,  GuiOffer * userIdent );
    ShareOfferListItem*         addOrUpdateOfferSession( GuiOfferSession* hostSession );

    GuiOfferSession*            findSession( VxGUID& lclSessionId );
    ShareOfferListItem*         findListEntryWidgetBySessionId( VxGUID& sessionId );
    ShareOfferListItem*         findListEntryWidgetByOnlineId( VxGUID& onlineId );

    void                        updateOffer( GuiOfferSession * user );
    void                        removeOffer( VxGUID& onlineId );

    void                        setShowMyself( bool show )                  { m_ShowMyself = show; }
    bool                        getShowMyself( void )                       { return m_ShowMyself; }

    void                        setOfferViewType( EOfferViewType viewType );
    EOfferViewType              getOfferViewType( void )                     { return m_OfferViewType; }

signals:
    void                        signalShareOfferListItemClicked( GuiOfferSession* hostSession, ShareOfferListItem* hostItem );

    void                        signalAvatarButtonClicked( GuiOfferSession* hostSession, ShareOfferListItem* hostItem );
    void                        signalMenuButtonClicked( GuiOfferSession* hostSession, ShareOfferListItem* hostItem );
    void                        signalFriendshipButtonClicked( GuiOfferSession* hostSession, ShareOfferListItem* hostItem );

protected slots:

    /*
    void				        slotOfferAdded( GuiOffer* user ); 
    void				        slotOfferRemoved( VxGUID onlineId ); 
    void                        slotOfferUpdated( GuiOffer* user );
    void                        slotOfferOnlineStatus( GuiOffer* user, bool isOnline );
    */

    void                        slotShareOfferListItemClicked( ShareOfferListItem* hostItem );
    void                        slotAvatarButtonClicked( ShareOfferListItem* hostItem );
    void                        slotMenuButtonClicked( ShareOfferListItem* hostItem );
    void                        slotFriendshipButtonClicked( ShareOfferListItem* hostItem );

protected:
    ShareOfferListItem*              sessionToWidget( GuiOfferSession* hostSession );
    GuiOfferSession*		    widgetToSession( ShareOfferListItem* hostItem );

    virtual void                onShareOfferListItemClicked( ShareOfferListItem* hostItem );
    virtual void                onAvatarButtonClicked( ShareOfferListItem* hostItem );
    virtual void                onFriendshipButtonClicked( ShareOfferListItem* hostItem );
    virtual void                onMenuButtonClicked( ShareOfferListItem* hostItem );

    void                        refreshList( void );
    bool                        isListViewMatch( GuiOfferSession * user );

	//=== vars ===//
	AppCommon&					m_MyApp;
    GuiOfferClientMgr&          m_OfferClientMgr;
	P2PEngine&					m_Engine;
	VxTimer						m_ClickEventTimer; // avoid duplicate clicks
    bool                        m_ShowMyself{ true };
    EOfferViewType              m_OfferViewType{ eOfferViewTypeNone };

    std::map<VxGUID, GuiOfferSession*> m_CallCache;
    std::map<VxGUID, GuiOfferSession*> m_OfferCache;
};


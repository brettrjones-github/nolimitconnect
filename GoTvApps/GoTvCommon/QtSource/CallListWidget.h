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

#include "ListWidgetBase.h"

enum ECallViewType
{
    eCallViewTypeNone,
    eCallViewTypeFriends,
    eCallViewTypeGroup,
    eCallViewTypeChatRoom,
    eCallViewTypeRandomConnect,
    eCallViewTypeEverybody,
    eCallViewTypeIgnored,

    eMaxCallViewTypeNone,
};

class CallListItem;
class GuiCall;
class GuiOfferSession;
class GuiOfferClientMgr;

class CallListWidget : public ListWidgetBase
{
	Q_OBJECT

public:
	CallListWidget( QWidget * parent );

    void                        clearCallList( void );

    std::map<VxGUID, GuiOfferSession*>& getCallList( void )             { return m_CallCache; }
    std::map<VxGUID, GuiOfferSession*>& getOfferList( void )            { return m_OfferCache; }

    void                        addCallToList( EHostType hostType, VxGUID& sessionId,  GuiOfferSession * userIdent );
    CallListItem*               addOrUpdateCallSession( GuiOfferSession* hostSession );

    GuiOfferSession*            findSession( VxGUID& lclSessionId );
    CallListItem*               findListEntryWidgetBySessionId( VxGUID& sessionId );
    CallListItem*               findListEntryWidgetByOnlineId( VxGUID& onlineId );

    void                        updateCall( GuiOfferSession * user );
    void                        removeCall( VxGUID& onlineId );

    void                        setShowMyself( bool show )                  { m_ShowMyself = show; }
    bool                        getShowMyself( void )                       { return m_ShowMyself; }

    void                        setCallViewType( ECallViewType viewType );
    ECallViewType               getCallViewType( void )                     { return m_CallViewType; }

signals:
    void                        signalAvatarButtonClicked( GuiOfferSession* hostSession, CallListItem* hostItem );
    void                        signalMenuButtonClicked( GuiOfferSession* hostSession, CallListItem* hostItem );

protected slots:
    /*
    void				        slotMyIdentUpdated( GuiCall* user ); 


    void				        slotCallAdded( GuiCall* user ); 
    void				        slotCallRemoved( VxGUID onlineId ); 
    void                        slotCallUpdated( GuiCall* user );
    void                        slotCallOnlineStatus( GuiCall* user, bool isOnline );
    */

    void                        slotAvatarButtonClicked( CallListItem* hostItem );
    void                        slotMenuButtonClicked( CallListItem* hostItem );

protected:
    CallListItem*               sessionToWidget( GuiOfferSession* hostSession );
    GuiOfferSession*			widgetToSession( CallListItem* hostItem );

    virtual void                onAvatarButtonClicked( CallListItem* hostItem );
    virtual void                onMenuButtonClicked( CallListItem* hostItem );

    void                        refreshList( void );
    bool                        isListViewMatch( GuiOfferSession * user );

	//=== vars ===//
    GuiOfferClientMgr&			m_OfferClientMgr;
    bool                        m_ShowMyself{ true };
    ECallViewType               m_CallViewType{ eCallViewTypeNone };

    std::map<VxGUID, GuiOfferSession*> m_CallCache;
    std::map<VxGUID, GuiOfferSession*> m_OfferCache;
};


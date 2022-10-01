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

#include "IdentWidget.h"

#include <QListWidgetItem>


class GuiUserSessionBase;
class GuiThumb;

class GuiUserListItem : public IdentWidget, public QListWidgetItem
{
	Q_OBJECT;
public:
	GuiUserListItem( QWidget*parent=0 );
	virtual ~GuiUserListItem();

    AppCommon&					getMyApp( void ) { return m_MyApp; }
    MyIcons&					getMyIcons( void );

    void                        setUserSession( GuiUserSessionBase* hostSession );
    GuiUserSessionBase*         getUserSession( void );

    VxPushButton*               getAvatarButton( void )                 { return getIdentAvatarButton(); }
    VxPushButton*               getFriendshipButton( void )             { return getIdentFriendshipButton(); }
    VxPushButton*               getOfferButton( void )                  { return getIdentOfferButton(); }
    VxPushButton*               getPushToTalkButton( void )             { return getIdentPushToTalkButton(); }
    VxPushButton*               getMenuButton( void )                   { return getIdentMenuButton(); }

    void						onIdentAvatarButtonClicked( void ) override;
    //void                        onIdentFriendshipButtonClicked( void ) override; // commented out to allow default behavior
    void						onIdentOfferButtonClicked( void ) override;
    void						onIdentPushToTalkButtonPressed( void ) override;
    void						onIdentPushToTalkButtonReleased( void ) override;
    void						onIdentMenuButtonClicked( void ) override;

    void						updateWidgetFromInfo( void );
    void                        updateThumb( GuiThumb* thumb );

    QSize                       calculateSizeHint( void );

    void                        setIsThumbUpdated( bool updated )       { m_IsThumbUpdated = updated; }
    bool                        getIsThumbUpdated( void )               { return m_IsThumbUpdated; }

    void                        callbackPushToTalkStatus( VxGUID& onlineId, EPushToTalkStatus pushToTalkStatus );

signals:
    void						signalGuiUserListItemClicked( GuiUserListItem * poItemWidget );
	void						signalAvatarButtonClicked( GuiUserListItem* listEntryWidget );
    void						signalFriendshipButtonClicked( GuiUserListItem* listEntryWidget );
    void						signalOfferButtonClicked( GuiUserListItem* listEntryWidget );
    void						signalPushToTalkButtonPressed( GuiUserListItem* listEntryWidget );
    void						signalPushToTalkButtonReleased( GuiUserListItem* listEntryWidget );
	void						signalMenuButtonClicked( GuiUserListItem* listEntryWidget );

protected:
    virtual void				mousePressEvent( QMouseEvent * event ) override;
    virtual void				resizeEvent( QResizeEvent* resizeEvent ) override;

	//=== vars ===//
    AppCommon&					m_MyApp;
    bool                        m_IsThumbUpdated{ false };
};





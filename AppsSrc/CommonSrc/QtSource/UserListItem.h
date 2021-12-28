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

#include "IdentWidget.h"

#include <QListWidgetItem>


class GuiUserSessionBase;
class GuiThumb;

class UserListItem : public IdentWidget, public QListWidgetItem
{
	Q_OBJECT;
public:
	UserListItem( QWidget *parent=0 );
	virtual ~UserListItem();

    AppCommon&					getMyApp( void ) { return m_MyApp; }
    MyIcons&					getMyIcons( void );

    void                        setUserSession( GuiUserSessionBase* hostSession );
    GuiUserSessionBase*         getUserSession( void );

    VxPushButton *              getAvatarButton( void )                 { return getIdentAvatarButton(); }
    VxPushButton *              getFriendshipButton( void )             { return getIdentFriendshipButton(); }
    VxPushButton *              getMenuButton( void )                   { return getIdentMenuButton(); }

    void						onIdentAvatarButtonClicked( void ) override;
    void						onIdentMenuButtonClicked( void ) override;

    void						updateWidgetFromInfo( void );
    void                        updateThumb( GuiThumb* thumb );

    QSize                       calculateSizeHint( void );

signals:
    void						signalUserListItemClicked( UserListItem * poItemWidget );
	void						signalAvatarButtonClicked( UserListItem* listEntryWidget );
	void						signalMenuButtonClicked( UserListItem* listEntryWidget );

protected:
    virtual void				mousePressEvent( QMouseEvent * event ) override;
    virtual void				resizeEvent( QResizeEvent* resizeEvent ) override;

	//=== vars ===//
    AppCommon&					m_MyApp;
};





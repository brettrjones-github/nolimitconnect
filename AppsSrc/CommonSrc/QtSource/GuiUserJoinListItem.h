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
#include <QWidget>

class GuiConnectIdListMgr;
class GuiOfferClientMgr;
class GuiUserJoinSession;
class GuiUser;
class GuiUserJoin;
class GuiUserJoinMgr;
class GuiUserMgr;
class GuiThumb;

class GuiUserJoinListItem : public IdentWidget, public QListWidgetItem
{
	Q_OBJECT;
public:
	GuiUserJoinListItem( QWidget*parent=0 );
	virtual ~GuiUserJoinListItem();

    AppCommon&					getMyApp( void ) { return m_MyApp; }
    MyIcons&					getMyIcons( void );

    void                        setUserJoinSession( GuiUserJoinSession* hostSession );
    GuiUserJoinSession*         getUserJoinSession( void );

    void						updateWidgetFromInfo( void );
    void                        updateThumb( GuiThumb* thumb );

    QSize                       calculateSizeHint( void );

    void                        updateUser( GuiUser* guiUser );

    void                        setIsThumbUpdated( bool updated )   { m_IsThumbUpdated = updated; }
    bool                        getIsThumbUpdated( void )           { return m_IsThumbUpdated; }

signals:
	void						signalAvatarButtonClicked( GuiUserJoinListItem* listEntryWidget );
	void						signalMenuButtonClicked( GuiUserJoinListItem* listEntryWidget );

protected:
    void						onIdentAvatarButtonClicked( void ) override;
    void						onIdentMenuButtonClicked( void ) override;

    virtual void				mousePressEvent( QMouseEvent * event ) override;
    virtual void				resizeEvent( QResizeEvent* resizeEvent ) override;

	//=== vars ===//
    AppCommon&					m_MyApp;
    GuiConnectIdListMgr&		m_ConnectIdListMgr;
    GuiOfferClientMgr&			m_OfferClientMgr;
    GuiUserMgr&					m_UserMgr;
    GuiUserJoinMgr&				m_UserJoinMgr;
    bool                        m_IsThumbUpdated{ false };
};





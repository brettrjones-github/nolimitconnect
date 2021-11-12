#pragma once
//============================================================================
// Copyright (C) 2020 Brett R. Jones
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

#include "MyIconsDefs.h"
#include "GuiParams.h"
#include <QWidget>

class AppCommon;
class VxGUID;
class VxPushButton;
class VxNetIdent;
class QLabel;
class GuiUser;
class GuiHostJoin;

class IdentLogicInterface : public QWidget
{
	Q_OBJECT
public:
	IdentLogicInterface() = delete;
	IdentLogicInterface( QWidget* parent );
	virtual ~IdentLogicInterface() = default;

	virtual void				setupIdentLogic( void ); // call after derived class ui is called
	virtual void				setupIdentLogic( EButtonSize buttonSize ); // call after derived class ui is called.. also calls setIdentWidgetSize
	virtual void				onIdentLogicIsSetup( void ) {};

	virtual void				setIdentWidgetSize( EButtonSize buttonSize );

	virtual VxPushButton*		getIdentAvatarButton( void ) = 0;
	virtual VxPushButton*		getIdentFriendshipButton( void ) = 0;
	virtual VxPushButton*		getIdentOfferButton( void )				{ return nullptr; }
	virtual VxPushButton*		getIdentMenuButton( void ) = 0;

	virtual QLabel*				getIdentLine1( void ) = 0;
	virtual QLabel*				getIdentLine2( void ) = 0;
	virtual QLabel*				getIdentLine3( void )					{ return nullptr; }

	virtual void				updateIdentity( GuiUser* guiUser );
	virtual void				updateIdentity( GuiHostJoin* hostIdent );
	virtual void				updateIdentity( VxNetIdent* netIdent );

	void						setDisableFriendshipChange( bool disable ) { m_DisableFriendshipChange = disable; }

	void						setIdentOnlineState( bool isOnline );
	void						setIdentGroupState( bool isInGroup );
	void						setIdentDirectConnectState( bool canDirectConnect );

	void						setIdentAvatarButtonVisible( bool visible );
	void						setIdentFriendshipButtonVisible( bool visible );
	void						setIdentOfferButtonVisible( bool visible );
	void						setIdentMenuButtonVisible( bool visible );

	void						setIdentAvatarThumbnail( VxGUID& thumbId );
	void						setIdentAvatarIcon( EMyIcons myIcon );
	void						setIdentFriendshipIcon( EMyIcons myIcon );
	void						setIdentOfferIcon( EMyIcons myIcon );
	void						setIdentMenuIcon( EMyIcons myIcon );

	virtual void				onIdentAvatarButtonClicked( void )		{};
	virtual void				onIdentFriendshipButtonClicked( void );
	virtual void				onIdentOfferButtonClicked( void )		{};
	virtual void				onIdentMenuButtonClicked( void )		{};

signals:
	void						signalIdentAvatarButtonClicked( void );
	void						signalIdentOfferButtonClicked( void );
	void						signalIdentMenuButtonClicked( void );

protected slots:
	void						slotIdentAvatarButtonClicked( void );
	void						slotIdentFrienshipButtonClicked( void );
	void						slotIdentOfferButtonClicked( void );
	void						slotIdentMenuButtonClicked( void );

protected:
	AppCommon&					m_MyApp;
	GuiUser*					m_NetIdent{ nullptr };
	bool						m_DisableFriendshipChange{ false };
};
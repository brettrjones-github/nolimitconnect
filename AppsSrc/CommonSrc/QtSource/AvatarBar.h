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
// http://www.nolimitconnect.org
//============================================================================

#include "ui_AvatarBarWidget.h"

#include <CoreLib/VxGUID.h>

class AppCommon;
class MyIcons;

class AvatarBar : public QWidget
{
	Q_OBJECT

public:
	AvatarBar( QWidget* parent = 0 );
	virtual ~AvatarBar() = default;

	AppCommon&					getMyApp( void ) { return m_MyApp; }
	MyIcons&					getMyIcons( void );

	void						setOnlineId( VxGUID& onlineId );
	void						setTime( time_t creationTime );
	void						setShredFile( QString fileName );
	void						setShredButtonIcon( EMyIcons iconTrash );

	virtual void				showAvatar( bool show );
	virtual void				showSendFail( bool show, bool permissionErr );
	virtual void				showShredder( bool show );
	virtual void				showResendButton( bool show );
	virtual void				showXferProgress( bool show );
	virtual void				setXferProgress( int sendProgress );

signals:
	void						signalShredAsset( void );
	void						signalResendAsset( void );

protected:
	//=== vars ===//
	Ui::AvatarBarWidgetUi		ui;
	AppCommon&					m_MyApp;
	VxGUID						m_OnlineId;
	bool						m_ThumbnailQueried{ false };
};

#pragma once
//============================================================================
// Copyright (C) 2009 Brett R. Jones 
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

#include "IdentWidget.h"

#include <QListWidgetItem>
#include <QObject>

class FriendListEntryWidget : public IdentWidget, public QListWidgetItem
{
	Q_OBJECT;
public:
	FriendListEntryWidget( QListWidget * parent = 0, int type = Type );
	virtual ~FriendListEntryWidget() = default;

	void						setUser( GuiUser* user );
	GuiUser*					getUser( void );

signals:
	void						listButtonClicked( FriendListEntryWidget* listEntryWidget );
	void						signalMenuButtonClicked( FriendListEntryWidget* listEntryWidget );

public slots:
	virtual void				onIdentAvatarButtonClicked( void ) override;
	virtual void				onIdentOfferButtonClicked( void ) override;
	virtual void				onIdentMenuButtonClicked( void ) override;

public:
	//=== vars ===//
};





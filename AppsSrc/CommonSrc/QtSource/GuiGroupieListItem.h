#pragma once
//============================================================================
// Copyright (C) 2021 Brett R. Jones 
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

#include "IdentWidget.h"

#include <QListWidgetItem>

class GuiGroupieListSession;

class GuiGroupieListItem : public IdentWidget, public QListWidgetItem
{
	Q_OBJECT;
public:
	GuiGroupieListItem( QWidget*parent=0 );
	virtual ~GuiGroupieListItem();

    AppCommon&					getMyApp( void ) { return m_MyApp; }
    MyIcons&					getMyIcons( void );

    void                        setHostSession( GuiGroupieListSession* hostSession );
    GuiGroupieListSession*      getHostSession( void );

    void						onIdentAvatarButtonClicked( void ) override;
    void						onIdentMenuButtonClicked( void ) override;

    void						updateWidgetFromInfo( void );

    QSize                       calculateSizeHint( void );

    void                        updateUser( GuiUser* guiUser );

    void                        setIsThumbUpdated( bool updated )   { m_IsThumbUpdated = updated; }
    bool                        getIsThumbUpdated( void )           { return m_IsThumbUpdated; }

signals:
    void						signalGuiGroupieListItemClicked( QListWidgetItem * poItemWidget );
	void						signalIconButtonClicked( GuiGroupieListItem* listEntryWidget );
    void						signalFriendshipButtonClicked( GuiGroupieListItem* listEntryWidget );
	void						signalMenuButtonClicked( GuiGroupieListItem* listEntryWidget );

 
protected:
    virtual void				mousePressEvent( QMouseEvent * event ) override;
    virtual void				resizeEvent( QResizeEvent* resizeEvent ) override;

	//=== vars ===//
    bool                        m_IsThumbUpdated{ false };
};





#pragma once
//============================================================================
// Copyright (C) 2013 Brett R. Jones
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

#include "AppletClientBase.h"

#include <GoTvInterface/IDefs.h>
#include <QString>
#include <QDialog>
#include "ui_AppletChatRoomJoinSearch.h"

class VxNetIdent;

class AppletChatRoomJoinSearch : public AppletClientBase
{
	Q_OBJECT
public:
	AppletChatRoomJoinSearch(	AppCommon&		    app, 
								QWidget *			parent = NULL );
	//=== destructor ===//
	virtual ~AppletChatRoomJoinSearch() override = default;

	void						searchResult( VxNetIdent * netIdent );
	void						setStatusLabel( QString strMsg );
    void                        setInfoLabel( QString strMsg );

    QString                     getSearchText( void ) { return ui.m_SearchsParamWidget->getSearchTextEdit()->text(); }

    virtual void				toGuiSearchResultSuccess( void * callbackData, EScanType eScanType, VxNetIdent * netIdent ) override;
    virtual void				toGuiClientScanSearchComplete( void * callbackData, EScanType eScanType ) override;

signals:
	void						signalSearchResult( VxNetIdent * netIdent );
	void						signalSearchComplete( void );

private slots:
	void						slotSearchResult( VxNetIdent * netIdent );
	void						slotSearchComplete( void );
    void						slotHomeButtonClicked( void ) override;
	void						slotStartSearchState(bool startSearch);
	//void						slotFriendClicked( VxNetIdent * netIdent );
    void						slotHostAnnounceStatus( EHostType, VxGUID sessionId, EHostAnnounceStatus hostStatus, QString strMsg );
    void						slotHostJoinStatus( EHostType, VxGUID sessionId, EHostJoinStatus hostStatus, QString strMsg );
    void						slotHostSearchStatus( EHostType, VxGUID sessionId, EHostSearchStatus hostStatus, QString strMsg );

protected:
    void						showEvent( QShowEvent * ev ) override;
    void						hideEvent( QHideEvent * ev ) override;

	//=== vars ===//
	Ui::AppletChatRoomJoinSearchUi		ui;
    SearchParams                m_SearchParams;
    bool                        m_SearchStarted{ false };
};

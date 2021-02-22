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

#include "AppletBase.h"

#include <GoTvInterface/IDefs.h>
#include <QString>
#include <QDialog>
#include "ui_AppletGroupJoinSearch.h"

class VxNetIdent;

class AppletGroupJoinSearch : public AppletBase
{
	Q_OBJECT
public:
	AppletGroupJoinSearch(	AppCommon&		    app, 
							QWidget *			parent = NULL );
	//=== destructor ===//
	virtual ~AppletGroupJoinSearch() override = default;

	EScanType					getScanType() { return m_eScanType; }
	void						searchResult( VxNetIdent * netIdent );
	void						setStatusLabel( QString strMsg );

    virtual void				toGuiScanResultSuccess( void * callbackData, EScanType eScanType, VxNetIdent * netIdent ) override;
    virtual void				toGuiClientScanSearchComplete( void * callbackData, EScanType eScanType ) override;

signals:
	void						signalSearchResult( VxNetIdent * netIdent );
	void						signalSearchComplete( void );

private slots:
	void						slotSearchResult( VxNetIdent * netIdent );
	void						slotSearchComplete( void );
    void						slotHomeButtonClicked( void ) override;
	void						slotStartSearchClicked();
	void						slotStopSearchClicked();
	//void						slotFriendClicked( VxNetIdent * netIdent );

protected:
    void						showEvent( QShowEvent * ev ) override;
    void						hideEvent( QHideEvent * ev ) override;

	//=== vars ===//
	Ui::AppletGroupJoinSearchUi		ui;
	EScanType					m_eScanType;
};

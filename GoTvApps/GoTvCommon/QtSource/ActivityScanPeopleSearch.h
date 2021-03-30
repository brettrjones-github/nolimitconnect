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

#include "ActivityBase.h"

#include <GoTvInterface/IDefs.h>
#include <QString>
#include <QDialog>
#include "ui_ActivityScanPeopleSearch.h"

class VxNetIdent;

class ActivityScanPeopleSearch : public ActivityBase
{
	Q_OBJECT
public:
	ActivityScanPeopleSearch(	AppCommon&		app, 
								EScanType			eSearchType,
								QWidget *			parent = NULL );
	//=== destructor ===//
	virtual ~ActivityScanPeopleSearch() override = default;

    // overrides required for dialogs with there own title bar and bottom bar widgets
    virtual TitleBarWidget *	getTitleBarWidget( void ) override { return ui.m_TitleBarWidget; }
    virtual BottomBarWidget *	getBottomBarWidget( void ) override { return ui.m_BottomBarWidget; }

	EScanType					getScanType() { return m_eScanType; }
	void						searchResult( GuiUser * netIdent );
	void						setTitle( QString strTitle );
	void						setStatusLabel( QString strMsg );

    virtual void				toGuiScanResultSuccess( void * callbackData, EScanType eScanType, GuiUser * netIdent ) override;
    virtual void				toGuiClientScanSearchComplete( void * callbackData, EScanType eScanType ) override;

signals:
	void						signalSearchResult( GuiUser * netIdent );
	void						signalSearchComplete( void );

private slots:
	void						slotSearchResult( GuiUser * netIdent );
	void						slotSearchComplete( void );
    void						slotHomeButtonClicked( void ) override;
	void						slotStartSearchClicked();
	void						slotStopSearchClicked();
	//void						slotFriendClicked( VxNetIdent * netIdent );

protected:
    void						showEvent( QShowEvent * ev ) override;
    void						hideEvent( QHideEvent * ev ) override;

	//=== vars ===//
	Ui::PeopleSearchDlg			ui;
	EScanType					m_eScanType;
};

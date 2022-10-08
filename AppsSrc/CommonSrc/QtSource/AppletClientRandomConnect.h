#pragma once
//============================================================================
// Copyright (C) 2019 Brett R. Jones
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

#include "ui_AppletClientRandomConnect.h"

#include "AppletClientBase.h"
#include <GuiInterface/IDefs.h>
#include <GuiInterface/IToGui.h> 

class AppletClientRandomConnect : public AppletClientBase
{
	Q_OBJECT
public:
	AppletClientRandomConnect( AppCommon& app, QWidget* parent );
	virtual ~AppletClientRandomConnect() override;

    EScanType					getScanType() { return m_eScanType; }
    void						searchResult( GuiUser* guiUser );

    virtual void				toGuiScanResultSuccess( EScanType eScanType, GuiUser* guiUser ) override;
    virtual void				toGuiClientScanSearchComplete( EScanType eScanType ) override;

signals:
    void						signalSearchResult( GuiUser* guiUser );
    void						signalSearchComplete( void );

private slots:
    void						slotSearchResult( GuiUser* guiUser );
    void						slotSearchComplete();

    void						slotHomeButtonClicked( void ) override;
    void						slotSimulateShakeClicked( void );
    void						slotRandomConnectStatus( ERandomConnectStatus eHostStatus, QString strMsg );

protected:
    virtual void				showEvent( QShowEvent* ev ) override;
    virtual void				hideEvent( QHideEvent* ev ) override;

    //=== vars ===//
    Ui::AppletRandomConnectUi	ui;
    EScanType					m_eScanType = eScanTypeRandomConnect;
};



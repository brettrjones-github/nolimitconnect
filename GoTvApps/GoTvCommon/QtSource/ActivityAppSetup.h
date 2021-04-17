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
// http://www.nolimitconnect.com
//============================================================================

#include "ActivityBase.h"
#include "AppSetup.h"

#include <GuiInterface/IDefs.h>

#include "ui_ActivityAppSetup.h"


class AppCommon;

class ActivityAppSetup : public ActivityBase
{
	Q_OBJECT

public:

    ActivityAppSetup( AppCommon& app, QWidget * parent );

	virtual ~ActivityAppSetup() override = default;

    // overrides required for dialogs with there own title bar and bottom bar widgets
    virtual TitleBarWidget *	getTitleBarWidget( void ) override { return ui.m_TitleBarWidget; }
    virtual BottomBarWidget *	getBottomBarWidget( void ) override { return ui.m_BottomBarWidget; }

    bool                        isSetupCompleted( void ) { return m_SetupCompleted; }


protected slots:
	void						slotCopyToClipboardButtonClicked( void );
    void						slotCopyResult( QObject * workerObj, int rc, qint64 bytesCopied );

protected:
    virtual void				showEvent( QShowEvent * ev ) override;

    void						initActivityAppSetup( void );
	//=== vars ===//
	Ui::ActivityAppSetupUi		ui;
    AppSetup *                  m_AppSetup = nullptr;
    bool                        m_SetupCompleted = false;
};

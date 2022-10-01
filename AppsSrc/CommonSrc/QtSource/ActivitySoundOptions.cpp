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
// http://www.nolimitconnect.org
//============================================================================

#include "ActivitySoundOptions.h"
#include "AppGlobals.h"
#include "AppCommon.h"
#include "AppSettings.h"

#include <ptop_src/ptop_engine_src/P2PEngine/EngineSettings.h>

#include <QFileDialog>
#ifdef _MSC_VER
# pragma warning(disable: 4003) //not enough actual parameters for macro 'mkdir'
#endif

//============================================================================
ActivitySoundOptions::	ActivitySoundOptions(	AppCommon& app, 
												QWidget* parent )
: ActivityBase( OBJNAME_ACTIVITY_SOUND_OPTIONS, app, parent, eAppletMessengerFrame, true )
{
	ui.setupUi(this);
	ui.m_TitleBarWidget->setTitleBarText( QObject::tr("Sound Options"));

    connectBarWidgets();

	connect( ui.AcceptButton, SIGNAL(clicked()), this, SLOT(onApplyButClick()) );
	connect( ui.CancelButton, SIGNAL(clicked()), this, SLOT(onCancelButClick()) );
	connect( ui.m_TitleBarWidget, SIGNAL(signalBackButtonClicked()), this, SLOT(onCancelButClick()) );

	bool bMutePhoneRing = m_MyApp.getAppSettings().getMutePhoneRing();
	bool bMuteNotifySound = m_MyApp.getAppSettings().getMuteNotifySound();

	ui.MutePhoneRingCheckBox->setCheckState( bMutePhoneRing ? Qt::Checked : Qt::Unchecked );
	ui.MuteNotifySouncCheckBox->setCheckState( bMuteNotifySound ? Qt::Checked : Qt::Unchecked );
}

//============================================================================  
void ActivitySoundOptions::onApplyButClick()
{
	bool bMutePhoneRing = ui.MutePhoneRingCheckBox->isChecked();
	bool bMuteNotifySound = ui.MuteNotifySouncCheckBox->isChecked();

	m_MyApp.getAppSettings().setMutePhoneRing( bMutePhoneRing );
	m_MyApp.getAppSettings().setMuteNotifySound( bMuteNotifySound );

	m_MyApp.applySoundSettings();

	accept();
}

//============================================================================   
void ActivitySoundOptions::onCancelButClick()
{
	reject();
}


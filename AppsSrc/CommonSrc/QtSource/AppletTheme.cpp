//============================================================================
// Copyright (C) 2010 Brett R. Jones
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

#include "AppletTheme.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "VxAppTheme.h"
#include "MyIcons.h"

#include <CoreLib/VxDebug.h>

//============================================================================
AppletTheme::AppletTheme( AppCommon& app, QWidget * parent )
: AppletBase( OBJNAME_APPLET_THEME, app, parent )
, m_SavedTheme( eThemeTypeUnknown )
, m_CurTheme( eThemeTypeUnknown )
{
	setAppletType( eAppletTheme );
	ui.setupUi( getContentItemsFrame() );
	setTitleBarText( DescribeApplet( m_EAppletType ) );
	ui.m_ThemeExampleButton->setIcon( eMyIconPeople );
	fillThemeCombo();
    fillExampleCombo();
    ui.m_PlainTextEdit->appendPlainText( QObject::tr( "Some Text To Edit" ) );

	connect( ui.m_AcceptCancelFrame, SIGNAL( signalAccepted() ), this, SLOT( slotThemeAccepted() ) );
	connect( this, SIGNAL( signalBackButtonClicked() ), this, SLOT( slotThemeCanceled() ) );
	connect( ui.m_AcceptCancelFrame, SIGNAL( signalCanceled() ), this, SLOT( slotThemeCanceled() ) );
    connect( ui.m_ExampleCheckBox, SIGNAL( stateChanged(int) ), this, SLOT( slotCheckBoxClicked(int) ) );

	m_MyApp.activityStateChange( this, true );
}

//============================================================================
void AppletTheme::slotThemeAccepted( void )
{
	m_MyApp.getAppSettings().setLastSelectedTheme( m_CurTheme );
    getMyApp().getAppTheme().selectTheme( m_CurTheme );
    m_MyApp.activityStateChange( this, false );
	closeApplet();
}

//============================================================================
void AppletTheme::onBackButtonClicked( void )
{
    slotThemeCanceled();
}

//============================================================================
void AppletTheme::slotThemeCanceled( void )
{
	restoreSavedTheme();
    m_MyApp.activityStateChange( this, false );
	closeApplet();
}

//============================================================================
void AppletTheme::slotCheckBoxClicked( int checkedState )
{
    LogMsg( LOG_DEBUG,  "check box state %d", checkedState );
}

//============================================================================
void AppletTheme::fillThemeCombo( void )
{
	VxAppTheme& appTheme = getMyApp().getAppTheme();
	for( int i = (int)(eThemeTypeUnknown + 1); i < eMaxThemeType; ++i )
	{
		ui.m_ThemeComboBox->addItem( appTheme.describeTheme( ( EThemeType )i ) );
	}

	m_SavedTheme = getMyApp().getAppSettings().getLastSelectedTheme();
	m_CurTheme = m_SavedTheme;
	if( ( eThemeTypeUnknown < m_CurTheme )
		&& ( eMaxThemeType > m_CurTheme ) )
	{
		ui.m_ThemeComboBox->setCurrentIndex( (int)(m_CurTheme - 1) );
	}

	connect( ui.m_ThemeComboBox, SIGNAL( currentIndexChanged( int ) ), this, SLOT( slotThemeSelectionChanged( int ) ) );
}

//============================================================================
void AppletTheme::fillExampleCombo( void )
{
    ui.m_ExampleComboBox->addItem( QObject::tr( "Selection 1" ) );
    ui.m_ExampleComboBox->addItem( QObject::tr( "Selection 2" ) );
    ui.m_ExampleComboBox->addItem( QObject::tr( "Selection 3" ) );
    ui.m_ExampleComboBox->addItem( QObject::tr( "Selection 4" ) );
    ui.m_ExampleComboBox->addItem( QObject::tr( "Selection 5" ) );
    ui.m_ExampleComboBox->setCurrentIndex( 0 );
}

//============================================================================
void AppletTheme::slotThemeSelectionChanged( int idx )
{
	applyTheme( ( EThemeType )( idx + 1 ) );
}

//============================================================================
void AppletTheme::saveCurrentTheme( void )
{
	
}

//============================================================================
void AppletTheme::applyTheme( EThemeType themeType )
{
	m_CurTheme = themeType;
	getMyApp().getAppTheme().selectTheme( m_CurTheme );
}

//============================================================================
void AppletTheme::restoreSavedTheme( void )
{
	getMyApp().getAppTheme().selectTheme( m_SavedTheme );
}

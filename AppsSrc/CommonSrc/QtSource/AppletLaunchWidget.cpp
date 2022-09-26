//============================================================================
// Copyright (C) 2017 Brett R. Jones 
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

#include "AppletLaunchWidget.h"
#include "AppletLaunchPage.h"
#include "AppCommon.h"

//============================================================================
AppletLaunchWidget::AppletLaunchWidget( AppCommon& myApp, EApplet appletType, AppletLaunchPage * parent )
: VxWidgetBase( parent->getContentItemsFrame() )
, m_MyApp( myApp )
, m_AppletType( appletType )
, m_AppletLaunchPage( parent )
{
	ui.setupUi( this );

	ui.m_LaunchButton->setIcon( GetAppletIcon( m_AppletType ) );
	ui.m_LauchLabel->setText( DescribeApplet( m_AppletType ) );

	connect( ui.m_LaunchButton, SIGNAL( pressed() ), this, SLOT( slotAppletIconPressed() ) );
}

//============================================================================
VxPushButton * AppletLaunchWidget::getButton( void )
{
	return ui.m_LaunchButton;
}

//============================================================================
void AppletLaunchWidget::slotAppletIconPressed( void )
{
    if( m_MyApp.getIsAppInitialized()
        || ( ( eAppletKodi == m_AppletType ) && m_MyApp.getLoginCompleted() )
        || ( eAppletCreateAccount == m_AppletType )
        || ( eAppletSearchPage == m_AppletType )
        || ( eAppletNetHostingPage == m_AppletType )
        || ( eAppletShareServicesPage == m_AppletType )
        || ( eAppletTheme == m_AppletType )
        || ( eAppletPlayerCamClip == m_AppletType )
        || ( eAppletPlayerPhoto == m_AppletType )
        || ( eAppletPlayerVideo == m_AppletType )
//        || ( eAppletPlayerMusic == m_AppletType )
//        
        )
    {
        m_MyApp.launchApplet( m_AppletType, m_MyApp.getAppletFrame( m_AppletType ) );
    }
    else
    {
        QMessageBox::information( this, QObject::tr( "Application Not Ready" ), QObject::tr( "Cannot Launch Applet Until Application Has Initialized" ) );
    }
}


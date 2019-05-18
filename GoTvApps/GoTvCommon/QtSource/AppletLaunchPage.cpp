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
// http://www.gotvptop.net
//============================================================================

#include "AppletLaunchPage.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "VxTilePositioner.h"
#include "MyIcons.h"
#include "AppletLaunchWidget.h"

#include <CoreLib/VxDebug.h>

//============================================================================
AppletLaunchPage::AppletLaunchPage( AppCommon& app, QWidget * parent )
: ActivityBase( OBJNAME_APPLET_LAUNCH_PAGE, app, parent, eAppletHomePage )
, m_IsInitialized( false )
{
	setTitleBarText( QObject::tr( "Home" ) );
	setupAppletLaunchPage();
	setHomeButtonVisibility( false );
	setBackButtonVisibility( false );
	setPowerButtonVisibility( true );
	setExpandWindowVisibility( true );
	connect( this, SIGNAL( signalPowerButtonClicked() ), this, SLOT( slotPowerButtonClicked() ) );
}

//============================================================================
void AppletLaunchPage::slotPowerButtonClicked( void )
{
	if( m_MyApp.confirmAppShutdown( this ) )
	{
		m_MyApp.shutdownAppCommon();
	}
}

//============================================================================
void AppletLaunchPage::setupAppletLaunchPage( void )
{
	if( ! m_IsInitialized )
    {
        // create launchers for the basic applets
        for( int i = int( eAppletHomePage + 1 ); i < eMaxBasicApplets; i++ )
        {
            AppletLaunchWidget * applet = new AppletLaunchWidget( m_MyApp, ( EApplet )i, this );
            m_AppletList.push_back( applet );
        }

        // optional features applets
        if( getMyApp().getAppSettings().getFeatureEnable( eAppFeatureProvider ) )
        {
            AppletLaunchWidget * applet = new AppletLaunchWidget( m_MyApp, ePluginAppletCamProvider, this );
            m_AppletList.push_back( applet );
        }

        if( getMyApp().getAppSettings().getFeatureEnable( eAppFeatureStation ) )
        {
            AppletLaunchWidget * applet = new AppletLaunchWidget( m_MyApp, ePluginAppletGoTvStation, this );
            m_AppletList.push_back( applet );
        }

        if( getMyApp().getAppSettings().getFeatureEnable( eAppFeatureNetworkHost ) )
        {
            AppletLaunchWidget * applet = new AppletLaunchWidget( m_MyApp, ePluginAppletGoTvNetworkHost, this );
            m_AppletList.push_back( applet );
        }

        m_IsInitialized = true;
    }
}

//============================================================================
void AppletLaunchPage::resizeEvent( QResizeEvent * ev )
{
	ActivityBase::resizeEvent( ev );
	LogMsg( LOG_DEBUG, "AppletLaunchPage::resizeEvent total height %d contentsFrame height %d\n", this->height(), getContentItemsFrame()->height() );
	getMyApp().getTilePositioner().repositionTiles( m_AppletList, getContentItemsFrame(), 2 );
}

//============================================================================
void AppletLaunchPage::showEvent( QShowEvent * showEvent )
{
	ActivityBase::showEvent( showEvent );
}

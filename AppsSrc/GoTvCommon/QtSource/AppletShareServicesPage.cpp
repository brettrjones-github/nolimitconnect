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
#include <app_precompiled_hdr.h>
#include "AppletShareServicesPage.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "VxTilePositioner.h"
#include "MyIcons.h"
#include "AppletLaunchWidget.h"

#include <CoreLib/VxDebug.h>

//============================================================================
AppletShareServicesPage::AppletShareServicesPage( AppCommon& app, QWidget * parent )
: AppletLaunchPage( app, parent, eAppletShareServicesPage, OBJNAME_APPLET_SERVICE_SHARE_PAGE )
, m_IsInitialized( false )
{
    setAppletType( eAppletShareServicesPage );
    setTitleBarText( DescribeApplet( m_EAppletType ) );
	setupAppletShareServicesPage();
	setHomeButtonVisibility( true );
	setBackButtonVisibility( true );
	setPowerButtonVisibility( false );
	setExpandWindowVisibility( true );

    slotRepositionToParent();
 //   connectSignals();

    // save original values so can restore them if need be
 //   m_Engine.getEngineSettings().getNetworkHostUrl( m_OrigHostUrl );
    //m_Engine.getEngineSettings().getNetworkKey( m_OrigNetworkName );
    //m_Engine.getEngineSettings().getConnectTestUrl( m_OrigConnectionTestUrl );

    //updateDlgFromSettings();

}

//============================================================================
void AppletShareServicesPage::slotPowerButtonClicked( void )
{
	if( m_MyApp.confirmAppShutdown( this ) )
	{
		m_MyApp.shutdownAppCommon();
	}
}

//============================================================================
void AppletShareServicesPage::setupAppletShareServicesPage( void )
{
	if( ! m_IsInitialized )
    {
        m_AppletList.clear();
        // create launchers for the basic applets
        for( int i = int( eMaxHostApplets + 1 ); i < eMaxSharedServicesApplets; i++ )
        {
            bool shouldShow = true;
            if( eAppletServiceStoryboard == i && !m_MyApp.getAppSettings().getFeatureEnable( eAppFeatureStoryboard ) )
            {
                shouldShow = false;
            }

            if( shouldShow )
            {
                AppletLaunchWidget* applet = new AppletLaunchWidget( m_MyApp, (EApplet)i, this );
                m_AppletList.push_back( applet );
            }
        }

        m_IsInitialized = true;
    }
}

//============================================================================
void AppletShareServicesPage::resizeEvent( QResizeEvent * ev )
{
	ActivityBase::resizeEvent( ev );
	//LogMsg( LOG_DEBUG, "AppletShareServicesPage::resizeEvent total height %d contentsFrame height %d\n", this->height(), getContentItemsFrame()->height() );
	getMyApp().getTilePositioner().repositionTiles( m_AppletList, getContentItemsFrame(), 2 );
}

//============================================================================
void AppletShareServicesPage::showEvent( QShowEvent * showEvent )
{
    AppletLaunchPage::showEvent( showEvent );
    getMyApp().getTilePositioner().repositionTiles( m_AppletList, getContentItemsFrame(), 2 );
}

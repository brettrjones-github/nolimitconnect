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

#include "AppletNetHostingPage.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "VxTilePositioner.h"
#include "MyIcons.h"
#include "AppletLaunchWidget.h"

#include <CoreLib/VxDebug.h>

//============================================================================
AppletNetHostingPage::AppletNetHostingPage( AppCommon& app, QWidget * parent )
: AppletLaunchPage( app, parent, eAppletNetHostingPage, OBJNAME_APPLET_NET_HOSTING_PAGE )
, m_IsInitialized( false )
{
    setAppletType( eAppletNetHostingPage );
    setTitleBarText( DescribeApplet( m_EAppletType ) );
	setupAppletNetHostingPage();
	setHomeButtonVisibility( true );
	setBackButtonVisibility( true );
	setPowerButtonVisibility( false );
	setExpandWindowVisibility( true );

    slotRepositionToParent();
}

//============================================================================
void AppletNetHostingPage::slotPowerButtonClicked( void )
{
	if( m_MyApp.confirmAppShutdown( this ) )
	{
		m_MyApp.shutdownAppCommon();
	}
}

//============================================================================
void AppletNetHostingPage::setupAppletNetHostingPage( void )
{
	if( ! m_IsInitialized )
    {
        m_AppletList.clear();
        // create launchers for the basic applets
        for( int i = int( eMaxSearchApplets + 1 ); i < eMaxHostApplets; i++ )
        {
            AppletLaunchWidget * applet = new AppletLaunchWidget( m_MyApp, ( EApplet )i, this );
            m_AppletList.push_back( applet );
        }

        m_IsInitialized = true;
    }
}

//============================================================================
void AppletNetHostingPage::resizeEvent( QResizeEvent * ev )
{
	ActivityBase::resizeEvent( ev );
	//LogMsg( LOG_DEBUG, "AppletNetHostingPage::resizeEvent total height %d contentsFrame height %d\n", this->height(), getContentItemsFrame()->height() );
	getMyApp().getTilePositioner().repositionTiles( m_AppletList, getContentItemsFrame(), 2 );
}

//============================================================================
void AppletNetHostingPage::showEvent( QShowEvent * showEvent )
{
    AppletLaunchPage::showEvent( showEvent );
    getMyApp().getTilePositioner().repositionTiles( m_AppletList, getContentItemsFrame(), 2 );
}

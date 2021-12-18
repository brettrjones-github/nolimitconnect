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

#include "AppletSearchPage.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "VxTilePositioner.h"
#include "MyIcons.h"
#include "AppletLaunchWidget.h"

#include <CoreLib/VxDebug.h>

//============================================================================
AppletSearchPage::AppletSearchPage( AppCommon& app, QWidget * parent )
: AppletLaunchPage( app, parent, eAppletSearchPage, OBJNAME_APPLET_SEARCH_PAGE )
{
    setAppletType( eAppletSearchPage );
    setTitleBarText( DescribeApplet( m_EAppletType ) );
	setupAppletSearchPage();
	setHomeButtonVisibility( false );
	setBackButtonVisibility( true );
	setPowerButtonVisibility( false );
	setExpandWindowVisibility( true );

    slotRepositionToParent();
}

//============================================================================
void AppletSearchPage::slotPowerButtonClicked( void )
{
	if( m_MyApp.confirmAppShutdown( this ) )
	{
		m_MyApp.shutdownAppCommon();
	}
}

//============================================================================
void AppletSearchPage::setupAppletSearchPage( void )
{
	if( ! m_IsInitialized )
    {
        m_AppletList.clear();
        // create launchers for the basic applets
        for( int i = int( eMaxSettingsApplets + 1 ); i < eMaxSearchApplets; i++ )
        {
            bool shouldShow = true;
            if( eAppletChatRoomJoinSearch == i && !m_MyApp.getAppSettings().getFeatureEnable( eAppFeatureChatRoom ) )
            {
                shouldShow = false;
            }
            else if( eAppletRandomConnectJoinSearch == i && !m_MyApp.getAppSettings().getFeatureEnable( eAppFeatureRandomConnect ) )
            {
                shouldShow = false;
            }
            else if( eAppletScanStoryboard == i && !m_MyApp.getAppSettings().getFeatureEnable( eAppFeatureStoryboard ) )
            {
                shouldShow = false;
            }
            else if( eAppletClientRandomConnect == i && !m_MyApp.getAppSettings().getFeatureEnable( eAppFeatureRandomConnect ) )
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
void AppletSearchPage::resizeEvent( QResizeEvent * ev )
{
	ActivityBase::resizeEvent( ev );
	//LogMsg( LOG_DEBUG, "AppletSearchPage::resizeEvent total height %d contentsFrame height %d\n", this->height(), getContentItemsFrame()->height() );
	getMyApp().getTilePositioner().repositionTiles( m_AppletList, getContentItemsFrame(), 2 );
}

//============================================================================
void AppletSearchPage::showEvent( QShowEvent * showEvent )
{
    AppletLaunchPage::showEvent( showEvent );
    getMyApp().getTilePositioner().repositionTiles( m_AppletList, getContentItemsFrame(), 2 );
}

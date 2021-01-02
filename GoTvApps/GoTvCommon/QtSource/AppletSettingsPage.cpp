//============================================================================
// Copyright (C) 2018 Brett R. Jones
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
#include "AppletSettingsPage.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "VxTilePositioner.h"
#include "MyIcons.h"
#include "AppletLaunchWidget.h"

#include <CoreLib/VxDebug.h>

//============================================================================
AppletSettingsPage::AppletSettingsPage( AppCommon& app, QWidget * parent )
: AppletLaunchPage(  app, parent, eAppletSettingsPage, OBJNAME_APPLET_SETTINGS_PAGE )
{
    setAppletType( eAppletSettingsPage );
	setTitleBarText( DescribeApplet( m_EAppletType ) );
    setupAppletSettingsgPage();
    setHomeButtonVisibility( false );
    setBackButtonVisibility( true );
    setPowerButtonVisibility( false );
    setExpandWindowVisibility( true );

    slotRepositionToParent();
}

//============================================================================
void AppletSettingsPage::setupAppletSettingsgPage( void )
{
    if( ! m_IsInitialized )
    {
        m_AppletList.clear();
        // create launchers for the basic applets
        for( int i = int( eMaxBasicApplets + 1 ); i < eMaxSettingsApplets; i++ )
        {
            AppletLaunchWidget * applet = new AppletLaunchWidget( m_MyApp, ( EApplet )i, this );
            m_AppletList.push_back( applet );
        }

        m_IsInitialized = true;
    }
}

//============================================================================
void AppletSettingsPage::resizeEvent( QResizeEvent * ev )
{
    ActivityBase::resizeEvent( ev );
    getMyApp().getTilePositioner().repositionTiles( m_AppletList, getContentItemsFrame(), 2 );
}

//============================================================================
void AppletSettingsPage::showEvent( QShowEvent * showEvent )
{
    AppletLaunchPage::showEvent( showEvent );
    getMyApp().getTilePositioner().repositionTiles( m_AppletList, getContentItemsFrame(), 2 );
}

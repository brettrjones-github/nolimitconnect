//============================================================================
// Copyright (C) 2020 Brett R. Jones
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
// http://www.nolimitconnect.com
//============================================================================

#include "AppletPermissionList.h"
#include "ActivityInformation.h"
#include "PermissionListItem.h"

#include "AppGlobals.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "MyIcons.h"
#include "AccountMgr.h"
#include "GuiHelpers.h"

#include <CoreLib/VxDebug.h>
#include <NetLib/NetHostSetting.h>
#include <NetLib/VxGetRandomPort.h>
#include <NetLib/VxSktUtil.h>

//============================================================================
AppletPermissionList::AppletPermissionList( AppCommon& app, QWidget * parent )
: AppletBase( OBJNAME_APPLET_PERMISSION_LIST, app, parent )
{
	setAppletType( eAppletPermissionList );
	ui.setupUi( getContentItemsFrame() );
	setTitleBarText( DescribeApplet( m_EAppletType ) );

    initializePermissionList();

    connectSignals();

	m_MyApp.activityStateChange( this, true );
}

//============================================================================
AppletPermissionList::~AppletPermissionList()
{
    m_MyApp.activityStateChange( this, false );
}

//============================================================================
void AppletPermissionList::connectSignals( void )
{
   // connect( ui.m_NetworkHostButton, SIGNAL( clicked() ), this, SLOT( slotShowNetworkHostInformation() ) );
}

//============================================================================
void AppletPermissionList::initializePermissionList( void )
{
    // ePluginTypeAdmin not implemented yet
    for( int i = ePluginTypeAdmin + 1; i < eMaxImplementedPluginType; ++i )
    {
        if( GuiHelpers::isPluginAPrimaryService( ( EPluginType )i ) )
        {
            EPluginType pluginType = ( EPluginType )i;
            if( pluginType == ePluginTypeHostChatRoom && !m_MyApp.getAppSettings().getFeatureEnable( eAppFeatureChatRoom ) )
            {
                continue;
            }

            if( pluginType == ePluginTypeHostRandomConnect && !m_MyApp.getAppSettings().getFeatureEnable( eAppFeatureRandomConnect ) )
            {
                continue;
            }

            if( pluginType == ePluginTypeStoryboard && !m_MyApp.getAppSettings().getFeatureEnable( eAppFeatureStoryboard ) )
            {
                continue;
            }

            if( pluginType == ePluginTypeAboutMePage && !m_MyApp.getAppSettings().getFeatureEnable( eAppFeatureAboutMePage ) )
            {
                continue;
            }

            m_PluginList.push_back( pluginType );
            createPermissionItem( pluginType );
        }
    }
}

//============================================================================
void AppletPermissionList::createPermissionItem( EPluginType pluginType )
{
    PermissionListItem * item = new PermissionListItem( ui.m_PermissionItemList, this );
    item->initPermissionListItem( pluginType );
    ui.m_PermissionItemList->addItem( ( QListWidgetItem * )item );
    ui.m_PermissionItemList->setItemWidget( ( QListWidgetItem * )item, ( QWidget * )item );
}

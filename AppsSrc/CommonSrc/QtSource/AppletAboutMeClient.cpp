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
// http://www.nolimitconnect.org
//============================================================================

#include "AppletAboutMeClient.h"
#include "AppletMgr.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "MyIcons.h"
#include "AppletSnapshot.h"
#include "AppGlobals.h"
#include "AppCommon.h"
#include "AccountMgr.h"
#include "GuiHelpers.h"

#include <QMessageBox>
#include <QUuid>
#include <QFileDialog>

#include <ptop_src/ptop_engine_src/P2PEngine/EngineSettings.h>
#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxGlobals.h>

//============================================================================
AppletAboutMeClient::AppletAboutMeClient( AppCommon& app, QWidget* parent )
: AppletBase( OBJNAME_APPLET_ABOUT_ME_CLIENT, app, parent )
{
    setAppletType( eAppletAboutMeClient );
    setPluginType( ePluginTypeAboutMePageClient );
    ui.setupUi( getContentItemsFrame() );
	setTitleBarText( DescribeApplet( m_EAppletType ) );

	m_MyApp.activityStateChange( this, true );
    m_MyApp.wantToGuiActivityCallbacks( this, true );
    m_MyApp.getWebPageMgr().wantWebPageCallbacks( this, true );
}

//============================================================================
AppletAboutMeClient::~AppletAboutMeClient()
{
    m_MyApp.getWebPageMgr().wantWebPageCallbacks( this, false );
    m_MyApp.getEngine().fromGuiCancelWebPage( eWebPageTypeAboutMe, m_HisOnlineId );

    m_MyApp.wantToGuiActivityCallbacks( this, false );
    m_MyApp.activityStateChange( this, false );
}

//============================================================================
void AppletAboutMeClient::setIdentity( GuiUser* guiUser )
{
    if( guiUser )
    {
        ui.m_IdentWidget->setupIdentLogic();
        ui.m_IdentWidget->updateIdentity( guiUser );
        m_HisOnlineId = guiUser->getMyOnlineId();
        m_MyApp.getEngine().fromGuiDownloadWebPage( eWebPageTypeAboutMe, m_HisOnlineId );
    }
}

//============================================================================
void AppletAboutMeClient::toGuiPluginMsg( EPluginType pluginType, VxGUID& onlineId, EPluginMsgType msgType, QString& paramValue )
{
    if( pluginType == getPluginType() )
    {
        ui.m_StatusLabel->setText( GuiParams::describePluginMsg( msgType ) );
        if( ePluginMsgDownloadComplete == msgType && !paramValue.isEmpty() )
        {
            loadRichTextFile( paramValue.toUtf8().constData() );
        }
    }
}

//============================================================================
void AppletAboutMeClient::loadRichTextFile( QString fileName )
{
    ui.m_RichEditWidget->loadRichTextFile( fileName.toUtf8().constData() );
}

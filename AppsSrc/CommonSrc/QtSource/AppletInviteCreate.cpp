//============================================================================
// Copyright (C) 2020 Brett R. Jones
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

#include "AppletInviteCreate.h"
#include "AppletHostSelect.h"
#include "AppletMgr.h"
#include "ActivityInformation.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "GuiHelpers.h"
#include "GuiParams.h"
#include "MyIcons.h"
#include "AccountMgr.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <CoreLib/Invite.h>
#include <NetLib/NetHostSetting.h>
#include <CoreLib/IsBigEndianCpu.h>
#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxUrl.h>

#include <QDesktopServices>
#include <QUrl>
#include <QClipboard>
#include <QScrollBar>
#include <QApplication>
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#include <QDesktopWidget>
#endif // QT_VERSION < QT_VERSION_CHECK(6,0,0)

namespace
{
    const int MAX_INFO_MSG_SIZE = 2048;
}

//============================================================================
AppletInviteCreate::AppletInviteCreate( AppCommon& app, QWidget * parent )
: AppletBase( OBJNAME_APPLET_INVITE_CREATE, app, parent )
{
    setAppletType( eAppletInviteCreate );
    ui.setupUi( getContentItemsFrame() );
	setTitleBarText( DescribeApplet( m_EAppletType ) );

    ui.m_SelectGroupButton->setIcon( eMyIconArrowUpRight );
    ui.m_SelectGroupButton->setFixedSize( eButtonSizeTiny );
    ui.m_CopyInviteButton->setIcon( eMyIconEditCopy );
    ui.m_CopyInviteButton->setFixedSize( eButtonSizeTiny );
    ui.m_NetworkSettingsInfoButton->setIcon( eMyIconInformation );
    ui.m_NetworkSettingsInfoButton->setFixedSize( eButtonSizeSmall );

    connect( ui.m_CopyInviteButton, SIGNAL( clicked() ), this, SLOT( slotCopyInviteButtonClicked() ) );
    connect( ui.m_NetworkSettingsInfoButton, SIGNAL( clicked() ), this, SLOT( slotNetworkSettingsInfoButtonClicked() ) );
    connect( ui.m_SelectGroupButton, SIGNAL( clicked() ), this, SLOT( slotSelectGroupHostButtonClicked() ) );

    connect( ui.m_MyUrlCheckBox, SIGNAL( stateChanged( int ) ), this, SLOT( slotUpdateInvite() ) );
    connect( ui.m_MyUrlEdit, SIGNAL( textChanged() ), this, SLOT( slotUpdateInvite() ) );
    connect( ui.m_GroupUrlCheckBox, SIGNAL( stateChanged( int ) ), this, SLOT( slotUpdateInvite() ) );
    connect( ui.m_GroupUrlEdit, SIGNAL( textChanged() ), this, SLOT( slotUpdateInvite() ) );
    connect( ui.m_NetworkSettingsCheckBox, SIGNAL( stateChanged( int ) ), this, SLOT( slotUpdateInvite() ) );
    connect( ui.m_InviteMessageTextEdit, SIGNAL( textChanged() ), this, SLOT( slotUpdateInvite() ) );

    m_MyApp.activityStateChange( this, true );
}

//============================================================================
AppletInviteCreate::~AppletInviteCreate()
{
    m_MyApp.activityStateChange( this, false );
}

//============================================================================
void AppletInviteCreate::slotCopyInviteButtonClicked( void )
{
    QClipboard* clipboard = QApplication::clipboard();
    if( !ui.m_InviteTextEdit->toPlainText().isEmpty() )
    {
        clipboard->setText( ui.m_InviteTextEdit->toPlainText() );
        okMessageBox( QObject::tr( "Invite Was Copied To Clipboard" ), QObject::tr( "Invite Was Copied To Clipboard" ) );
    }
    else
    {
        okMessageBox( QObject::tr( "Invite Is Empty" ), QObject::tr( "Cannot create an Invite if Invite is empty" ) );
    }
}

//============================================================================
void AppletInviteCreate::toGuiInfoMsg( char * infoMsg )
{
    QString infoStr( infoMsg );
#if QT_VERSION > QT_VERSION_CHECK(6,0,0)
    infoStr.remove(QRegularExpression("[\\n\\r]"));
#else
    infoStr.remove(QRegExp("[\\n\\r]"));
#endif // QT_VERSION > QT_VERSION_CHECK(6,0,0)
    emit signalInfoMsg( infoStr );
}

//============================================================================
void AppletInviteCreate::infoMsg( const char* errMsg, ... )
{
    char as8Buf[ MAX_INFO_MSG_SIZE ];
    va_list argList;
    va_start( argList, errMsg );
    vsnprintf( as8Buf, sizeof( as8Buf ), errMsg, argList );
    as8Buf[ sizeof( as8Buf ) - 1 ] = 0;
    va_end( argList );

    toGuiInfoMsg( as8Buf );
}

//============================================================================
void AppletInviteCreate::slotNetworkSettingsInfoButtonClicked( void )
{
    ActivityInformation* activityInfo = new ActivityInformation( m_MyApp, this, eInfoTypeNetworkSettingsInvite );
    activityInfo->show();
}


//============================================================================
void AppletInviteCreate::slotSelectGroupHostButtonClicked( void )
{
    AppletHostSelect* hostSelect = dynamic_cast<AppletHostSelect*>(m_MyApp.getAppletMgr().launchApplet( eAppletHostSelect, this, GuiParams::describeHostType( eHostTypeGroup ) ) );
    if( hostSelect )
    {
        connect( hostSelect, SIGNAL( signalHostSelected( QString ) ), this, SLOT( slotGroupHostSelected( QString ) ) );
    }
}

//============================================================================
void AppletInviteCreate::slotGroupHostSelected( QString hostUrl )
{
    ui.m_GroupUrlEdit->setText( hostUrl );
}

//============================================================================
void AppletInviteCreate::addInviteText( QString text )
{
    ui.m_InviteTextEdit->appendPlainText( text );
}

//============================================================================
void AppletInviteCreate::updateUrls( void )
{
    bool canDirectConnect = m_MyApp.getEngine().getNetStatusAccum().isRxPortOpen();
    VxNetIdent myIdent;
    m_FromGui.fromGuiQueryMyIdent( &myIdent );
    QString myUrl( myIdent.getMyOnlineUrl().c_str() );
    ui.m_MyUrlEdit->setText( myUrl );

    std::string connectedGroupUrl = m_MyApp.getEngine().getNetStatusAccum().getConnectedHostUrl(eHostTypeGroup);
    QString groupUrl = connectedGroupUrl.c_str();
    if( groupUrl.isEmpty() && canDirectConnect && myIdent.getPluginPermission( ePluginTypeHostGroup ) > eFriendStateIgnore )
    {
        groupUrl = myUrl;
    }

    ui.m_GroupUrlEdit->setText( groupUrl );
}

//============================================================================
void AppletInviteCreate::slotUpdateInvite( void )
{
    updateUrls();
    bool canDirectConnect = m_MyApp.getEngine().getNetStatusAccum().isRxPortOpen();
    bool isOnline = m_MyApp.getEngine().getNetStatusAccum().isP2PAvailable();

    bool haveInvite = false;
    ui.m_InviteTextEdit->clear();
    if( !isOnline )
    {
        addInviteText( QObject::tr( "You Must Be Connected To Network To Create An Invite" ) );
        return;
    }

    if( ui.m_MyUrlCheckBox->isChecked() || ui.m_GroupUrlCheckBox->isChecked() )
    {
        if( ui.m_MyUrlCheckBox->isChecked() && !ui.m_MyUrlEdit->text().isEmpty() )
        {
            if( !haveInvite )
            {
                haveInvite = true;
                addInviteText( canDirectConnect ? Invite::INVITE_HDR_DIRECT_CONNECT : Invite::INVITE_HDR_RELAYED );
            }

            addInviteText( ui.m_MyUrlEdit->text() + ( canDirectConnect ? Invite::SUFFIX_CHAR_PERSON_DIRECT : Invite::SUFFIX_CHAR_PERSON_RELAYED ) );
        }

        if( ui.m_GroupUrlCheckBox->isChecked() && !ui.m_GroupUrlEdit->text().isEmpty() )
        {
            if( !haveInvite )
            {
                haveInvite = true;
                addInviteText( canDirectConnect ? Invite::INVITE_HDR_DIRECT_CONNECT : Invite::INVITE_HDR_RELAYED );
            }

            addInviteText( ui.m_MyUrlEdit->text() + Invite::SUFFIX_CHAR_GROUP );
        }

        if( haveInvite && !ui.m_InviteMessageTextEdit->toPlainText().isEmpty() )
        {
            addInviteText( Invite::INVITE_HDR_MSG );
            addInviteText( ui.m_InviteMessageTextEdit->toPlainText() );
        }
    }

    if( ui.m_NetworkSettingsCheckBox->isChecked() )
    {
        addInviteText( Invite::INVITE_HDR_NET_SETTING );
        std::string strValue;

        m_Engine.getEngineSettings().getNetworkHostUrl( strValue );
        populateNetSettingUrl( eHostTypeNetwork, strValue );

        m_Engine.getEngineSettings().getGroupHostUrl( strValue );
        populateNetSettingUrl( eHostTypeGroup, strValue );

        m_Engine.getEngineSettings().getChatRoomHostUrl( strValue );
        populateNetSettingUrl( eHostTypeChatRoom, strValue );

        m_Engine.getEngineSettings().getRandomConnectUrl( strValue );
        populateNetSettingUrl( eHostTypeRandomConnect, strValue );

        m_Engine.getEngineSettings().getConnectTestUrl( strValue );
        populateNetSettingUrl( eHostTypeConnectTest, strValue );

        haveInvite = true;
    }

    if( haveInvite )
    {
        addInviteText( Invite::INVITE_END );
    }
}

//============================================================================
bool AppletInviteCreate::populateNetSettingUrl( EHostType hostType, std::string& ptopUrl )
{
    if( !ptopUrl.empty() )
    {
        QString url = ptopUrl.c_str();
        url += Invite::getHostTypeSuffix( hostType );
        addInviteText( url );
    }

    return true;
}
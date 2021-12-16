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

#include "ActivityCreateAccount.h"
#include "AppletMgr.h"
#include "AppletUserIdentity.h"
#include "AppletEditAvatarImage.h"
#include "AppCommon.h"
#include "AppGlobals.h"
#include "AppSettings.h"
#include "GuiHelpers.h"
#include "HomeWindow.h"
#include "MyIcons.h"
#include "AccountMgr.h"

#include <QMessageBox>
#include <QUuid>
#include <QFileDialog>

#include <ptop_src/ptop_engine_src/P2PEngine/EngineSettings.h>
#include <ptop_src/ptop_engine_src/ThumbMgr/ThumbInfo.h>
#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxGlobals.h>

#define SUPPORTED_IMAGE_FILES "Image files (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.xbm *.xpm)"

//============================================================================
AppletUserIdentity::AppletUserIdentity( AppCommon& app, QWidget * parent )
: AppletBase( OBJNAME_APPLET_USER_IDENTITY, app, parent )
, m_bUserPickedImage( false )
, m_bUsingDefaultImage( true )
{
    setAppletType( eAppletUserIdentity );
    ui.setupUi( getContentItemsFrame() );
	setTitleBarText( DescribeApplet( m_EAppletType ) );
    ui.m_EditAboutMeButton->setIcon( eMyIconServiceShareAboutMe );
    ui.m_EditAvatarImageButton->setIcon( eMyIconServiceAvatarImage );
    ui.m_EditStoryboardButton->setIcon( eMyIconServiceShareStoryboard );

    GuiHelpers::fillGender( ui.m_GenderComboBox );
    GuiHelpers::fillLanguage( ui.m_LanguageComboBox );
    GuiHelpers::fillContentRating( ui.m_ContentComboBox );
    GuiHelpers::fillAge( ui.m_AgeComboBox );

    VxNetIdent * curIdent = m_MyApp.getAppGlobals().getUserIdent();
    m_strOrigOnlineName = curIdent->getOnlineName();
    m_strOrigMoodMessage = curIdent->getOnlineDescription();

    m_MyApp.getAccountMgr().getAllAccounts( m_AccountList );
    for( VxNetIdent& ident : m_AccountList )
    {
        ui.m_AccountComboBox->addItem( ident.getOnlineName() );
        if( m_strOrigOnlineName != ident.getOnlineName() )
        {
            ui.m_AccountListWidget->addItem( ident.getOnlineName() );
        }
    }

    GuiHelpers::setValuesFromIdentity( this, curIdent, ui.m_AgeComboBox, ui.m_GenderComboBox, ui.m_LanguageComboBox, ui.m_ContentComboBox );

    connect( ui.m_ApplyMoodButton, SIGNAL( clicked() ), this, SLOT( onApplyMoodMsgClick() ) );
    connect( ui.m_ApplyAgeButton, SIGNAL( clicked() ), this, SLOT( onApplyAgeClick() ) );
    connect( ui.m_ApplyGenderButton, SIGNAL( clicked() ), this, SLOT( onApplyGenderClick() ) );

    connect( ui.m_EditAboutMeButton, SIGNAL( clicked() ), this, SLOT( slotEditAboutMeButClick() ) );
    connect( ui.m_EditAvatarImageButton, SIGNAL( clicked() ), this, SLOT( slotEditAvatarButClick() ) );
    connect( ui.m_AvatarImageButton, SIGNAL( clicked() ), this, SLOT( slotEditAvatarButClick() ) );
    connect( ui.m_EditStoryboardButton, SIGNAL( clicked() ), this, SLOT( slotEditStoryboardButClick() ) );

    connect( ui.m_CreateNewAccountButton, SIGNAL( clicked() ), this, SLOT( slotCreateNewAccount() ) );
    connect( ui.m_DeleteAccountButton, SIGNAL( clicked() ), this, SLOT( slotDeleteAccount() ) );

    connect( ui.m_AccountComboBox, SIGNAL( currentIndexChanged( int ) ), this, SLOT( slotAccountSelectionChanged( int ) ) );

    resetComboIdxToOriginalAccount();
    if( !m_MyApp.getAppSettings().getFeatureEnable( eAppFeatureAboutMePage ) )
    {
        ui.m_AboutMeGroupBox->setVisible( false );
    }

    if( !m_MyApp.getAppSettings().getFeatureEnable( eAppFeatureStoryboard ) )
    {
        ui.m_StoryBoardGroupBox->setVisible( false );
    }

    loadIdentity( *curIdent );

	m_MyApp.activityStateChange( this, true );
}

//============================================================================
void AppletUserIdentity::resetComboIdxToOriginalAccount()
{
    int identIdx = 0;
    for( VxNetIdent& ident : m_AccountList )
    {
        if( m_strOrigOnlineName == ident.getOnlineName() )
        {
            ui.m_AccountComboBox->setCurrentIndex( identIdx );
            break;
        }

        identIdx++;
    }
}

//============================================================================
void AppletUserIdentity::slotAccountSelectionChanged( int idx )
{
    if( ( ui.m_AccountComboBox->currentIndex() >= 0 ) && ( ui.m_AccountComboBox->currentIndex() < (int)m_AccountList.size() ) )
    {
        VxNetIdent& netIdent = m_AccountList[ ui.m_AccountComboBox->currentIndex() ];
        LogMsg( LOG_DEBUG, "AppletUserIdentity selected account %s", netIdent.getOnlineName() );
        updateAvatarImage( netIdent.getAvatarThumbGuid(), netIdent.getAvatarThumbModifiedTime() );
        if( netIdent.getOnlineName() != m_strOrigOnlineName )
        {
            QString title = QObject::tr( "Confirm Login To Account" );
            QString msgText = QObject::tr( "Are You Sure You Want To Login To Account " );
            msgText += netIdent.getOnlineName();
            msgText += QObject::tr( "?" );

            if( QMessageBox::Yes == QMessageBox::question( this, title, msgText, QMessageBox::Yes | QMessageBox::No ) )
            {
                // copy ident to global
                VxNetIdent* myIdent = m_MyApp.getAppGlobals().getUserIdent();
                if( myIdent )
                {
                    memcpy( myIdent, &netIdent, sizeof( VxNetIdent ) );
                    m_MyApp.getAccountMgr().updateLastLogin( myIdent->getOnlineName() );
                    m_MyApp.setupAccountResources( *myIdent );
                    m_MyApp.loadAccountSpecificSettings( myIdent->getOnlineName() );
                    m_Engine.fromGuiUserLoggedOn( myIdent );  
                    loadIdentity( *myIdent );
                }
                else
                {
                    LogMsg( LOG_SEVERE, "ERROR AppletUserIdentity could not get global identity" );
                }

                close();
            }
            else
            {
                resetComboIdxToOriginalAccount();
            }
        }
    }
}

//============================================================================
void AppletUserIdentity::loadIdentity( VxNetIdent& ident )
{
    QString strMoodMessage = ident.getOnlineDescription();
    ui.m_MoodMessageEdit->setText( strMoodMessage );
    updateAvatarImage( ident.getAvatarThumbGuid(), ident.getAvatarThumbModifiedTime() );
}

//============================================================================
void AppletUserIdentity::saveIdentity( VxNetIdent& ident )
{
    QString strMoodMessage = ui.m_MoodMessageEdit->text();
    ident.setOnlineDescription( strMoodMessage.toUtf8().constData() );
}

//============================================================================
//! apply mood message change
void AppletUserIdentity::onApplyMoodMsgClick( void )
{
    if( false == validateMoodMessage() )
    {
        return;
    }

    QString moodMsg = ui.m_MoodMessageEdit->text();

    if( moodMsg != m_strOrigMoodMessage )
    {
        if( ( ui.m_AccountComboBox->currentIndex() >= 0 ) && ( ui.m_AccountComboBox->currentIndex() < (int)m_AccountList.size() ) )
        {
            VxNetIdent& ident = m_AccountList[ ui.m_AccountComboBox->currentIndex() ];
            std::string strOnlineDesc = moodMsg.toUtf8().constData();
            ident.setOnlineDescription( strOnlineDesc.c_str() );
            m_MyApp.getAccountMgr().updateAccount( ident );
            if( m_strOrigOnlineName == ident.getOnlineName() )
            {
                m_Engine.fromGuiMoodMessageChanged( strOnlineDesc.c_str() );
            }

            QString msgText = QObject::tr( "Applied Mood Message Change " );
            QMessageBox::information( this, QObject::tr( "Mood Message Change Success" ), msgText );
        }
    }
}

//============================================================================
void AppletUserIdentity::onApplyAgeClick( void )
{
    if( ( ui.m_AccountComboBox->currentIndex() >= 0 ) && ( ui.m_AccountComboBox->currentIndex() < (int)m_AccountList.size() ) )
    {
        VxNetIdent& ident = m_AccountList[ ui.m_AccountComboBox->currentIndex() ];
        GuiHelpers::setIdentityFromValues( this, &ident, ui.m_AgeComboBox, ui.m_GenderComboBox, ui.m_LanguageComboBox, ui.m_ContentComboBox );

        m_MyApp.getAccountMgr().updateAccount( ident );
        if( m_strOrigOnlineName == ident.getOnlineName() )
        {
            m_Engine.fromGuiIdentPersonalInfoChanged( ident.getAgeType(), ident.getGender(), ident.getPrimaryLanguage(), ident.getPreferredContent() );
        }

        QString msgText = QObject::tr( "Applied Age and Preferred Content Change " );
        QMessageBox::information( this, QObject::tr( "Age and Preferred Content Success" ), msgText );
    }
}

//============================================================================
void AppletUserIdentity::onApplyContentClick( void )
{
    if( ( ui.m_AccountComboBox->currentIndex() >= 0 ) && ( ui.m_AccountComboBox->currentIndex() < (int)m_AccountList.size() ) )
    {
        VxNetIdent& ident = m_AccountList[ ui.m_AccountComboBox->currentIndex() ];
        GuiHelpers::setIdentityFromValues( this, &ident, ui.m_AgeComboBox, ui.m_GenderComboBox, ui.m_LanguageComboBox, ui.m_ContentComboBox );

        m_MyApp.getAccountMgr().updateAccount( ident );
        if( m_strOrigOnlineName == ident.getOnlineName() )
        {
            m_Engine.fromGuiIdentPersonalInfoChanged( ident.getAgeType(), ident.getGender(), ident.getPrimaryLanguage(), ident.getPreferredContent() );
        }

        QString msgText = QObject::tr( "Applied Mood Message Change " );
        QMessageBox::information( this, QObject::tr( "Mood Message Change Success" ), msgText );
    }
}

//============================================================================
void AppletUserIdentity::onApplyGenderClick( void )
{
    if( ( ui.m_GenderComboBox->currentIndex() >= 0 ) && ( ui.m_GenderComboBox->currentIndex() < (int)m_AccountList.size() ) )
    {
        VxNetIdent& ident = m_AccountList[ ui.m_GenderComboBox->currentIndex() ];
        GuiHelpers::setIdentityFromValues( this, &ident, ui.m_AgeComboBox, ui.m_GenderComboBox, ui.m_LanguageComboBox, ui.m_ContentComboBox );

        m_MyApp.getAccountMgr().updateAccount( ident );
        if( m_strOrigOnlineName == ident.getOnlineName() )
        {
            m_Engine.fromGuiIdentPersonalInfoChanged( ident.getAgeType(), ident.getGender(), ident.getPrimaryLanguage(), ident.getPreferredContent() );
        }

        QString msgText = QObject::tr( "Applied Gender Change " );
        QMessageBox::information( this, QObject::tr( "Gender Change Success" ), msgText );
    }
}

//============================================================================
void AppletUserIdentity::slotEditAboutMeButClick( void )
{
    m_MyApp.getAppletMgr().launchApplet( eAppletEditAboutMe, this );
}

//============================================================================
void AppletUserIdentity::slotEditAvatarButClick( void )
{
    AppletEditAvatarImage * editAvatarImage = dynamic_cast< AppletEditAvatarImage * >( m_MyApp.getAppletMgr().launchApplet( eAppletEditAvatarImage, this ) );
    if( editAvatarImage )
    {
        connect( editAvatarImage, SIGNAL( signalAvatarImageChanged( ThumbInfo * ) ), this, SLOT( slotAvatarImageChanged( ThumbInfo * ) ) );
        connect( editAvatarImage, SIGNAL( signalAvatarImageRemoved() ), this, SLOT( slotAvatarImageRemoved() ) );
    }
}

//============================================================================
void AppletUserIdentity::slotEditStoryboardButClick( void )
{
    m_MyApp.getAppletMgr().launchApplet( eAppletEditStoryboard, this );
}

//============================================================================
//! Implement the OnClickListener callback    
void AppletUserIdentity::slotCreateNewAccount( void )
{
    QString title = QObject::tr( "Confirm Create New Account" );
    QString msgText = QObject::tr( "Are You Sure You Want To Create A New Account?" );

    if( QMessageBox::Yes == QMessageBox::question( this, title, msgText, QMessageBox::Yes | QMessageBox::No ) )
    {
        ActivityCreateAccount * createAccountActivity = new ActivityCreateAccount( m_MyApp, &m_MyApp.getHomePage() );
        createAccountActivity->show();
        close();
    }
}

//============================================================================
//! Implement the OnClickListener callback    
void AppletUserIdentity::slotDeleteAccount( void )
{
    if( ( m_AccountList.size() > 1 ) && ui.m_AccountListWidget->currentItem() )
    {
        const QString& accountName = ui.m_AccountListWidget->currentItem()->text();
        if( !accountName.isEmpty() )
        {
            if( accountName == m_strOrigOnlineName )
            {
                QString title = QObject::tr( "Cannot Delete Account" );
                QString msgText = QObject::tr( "Cannot Delete An Account That Is In Use" );
                QMessageBox::information( this, title, msgText, QMessageBox::Ok );
            }
            else
            {
                QString title = QObject::tr( "Confirm Delete Account" );
                QString msgText = QObject::tr( "Are You Sure You Want To Delete This Account?" );

                if( QMessageBox::Yes == QMessageBox::question( this, title, msgText, QMessageBox::Yes | QMessageBox::No ) )
                {
                    if( m_MyApp.getAccountMgr().removeAccountByName( accountName.toUtf8().constData() ) )
                    {
                        QString title = QObject::tr( "Account was removed" );
                        QString msgText = QObject::tr( "Account was removed" );
                        QMessageBox::information( this, title, msgText, QMessageBox::Ok );
                    }
                    else
                    {
                        QString title = QObject::tr( "A error occured and account was not removed" );
                        QString msgText = QObject::tr( "A error occured and account was not removed" );
                        QMessageBox::information( this, title, msgText, QMessageBox::Ok );
                    }
                }
            }
        }
    }
    else
    {
        QString title = QObject::tr( "Connot delete the last account" );
        QString msgText = QObject::tr( "Connot delete the last account" );
        QMessageBox::information( this, title, msgText, QMessageBox::Ok );
    }
}

//============================================================================
//! validate user input
QString AppletUserIdentity::validateString( QString charSeq )
{
    //return charSeq.toString();    	
    return charSeq;
}

//============================================================================
bool AppletUserIdentity::validateMoodMessage( void )
{
    QString strMoodMsg = ui.m_MoodMessageEdit->text();
    return GuiHelpers::validateMoodMessage( this, strMoodMsg );
}

//============================================================================
void AppletUserIdentity::updateAvatarImage( VxGUID& thumbId, int64_t thumbModifiedTime )
{
    QImage thumbImage;
    m_MyApp.getThumbMgr().getAvatarImage( thumbId, thumbImage );
    if( !thumbImage.isNull() )
    {
        ui.m_AvatarImageButton->setIconOverrideImage( thumbImage );
    }
}

//============================================================================
void AppletUserIdentity::slotAvatarImageChanged( ThumbInfo* avatarThumb )
{
    updateAvatarImage( avatarThumb->getAssetUniqueId(), avatarThumb->getModifiedTime() );
}

//============================================================================
void AppletUserIdentity::slotAvatarImageRemoved( void )
{
    ui.m_AvatarImageButton->clearIconOverrideImage();
}
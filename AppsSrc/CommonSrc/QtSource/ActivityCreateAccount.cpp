//============================================================================
// Copyright (C) 2010 Brett R. Jones
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

#include "ActivityCreateAccount.h"
#include "AppGlobals.h"
#include "AppCommon.h"
#include "AccountMgr.h"
#include "AppSettings.h"
#include "GuiHelpers.h"

#include <ptop_src/ptop_engine_src/P2PEngine/EngineSettings.h>
#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxGlobals.h>

#include <QMessageBox>
#include <QUuid>

//============================================================================
ActivityCreateAccount::ActivityCreateAccount( AppCommon& app, QWidget * parent )
: ActivityBase( OBJNAME_ACTIVITY_CREATE_ACCOUNT, app, parent, eAppletCreateAccount, true, false, true )
{
    ui.setupUi( this );
    setTitleBarText( QObject::tr( "Create Account" ) );

    connectBarWidgets();

    GuiHelpers::fillGender( ui.m_GenderComboBox );
    GuiHelpers::fillLanguage( ui.m_LanguageComboBox );
    GuiHelpers::fillContentRating( ui.m_ContentComboBox );
    GuiHelpers::fillAge( ui.m_AgeComboBox );

    connect( ui.m_LoginButton, SIGNAL( clicked() ), this, SLOT( slotButtonLoginClicked() ) );
}

//============================================================================
//! button clicked
void ActivityCreateAccount::slotButtonLoginClicked( void )
{
    if( true == accountValidate() )
    {
        std::string strUserName = ui.m_UserNameEdit->text().toUtf8().data();

        m_MyApp.loadAccountSpecificSettings( strUserName.c_str() );
        m_MyApp.createAccountForUser( strUserName, m_UserAccount, (const char *)ui.m_MoodMessageEdit->text().toUtf8().constData(), 
                                      ui.m_GenderComboBox->currentIndex(), (EAgeType)ui.m_AgeComboBox->currentIndex(), ui.m_LanguageComboBox->currentIndex(), ui.m_ContentComboBox->currentIndex() );
        m_MyApp.setAccountUserName( strUserName.c_str() );
        m_UserAccount.setAgeType( (EAgeType)ui.m_AgeComboBox->currentIndex() );
        m_UserAccount.setGender( ui.m_GenderComboBox->currentIndex() );
        m_UserAccount.setPrimaryLanguage( ui.m_LanguageComboBox->currentIndex() );
        m_UserAccount.setPreferredContent( ui.m_ContentComboBox->currentIndex() );
        m_UserAccount.setOnlineDescription( ( const char* )ui.m_MoodMessageEdit->text().toUtf8().constData() );

        m_UserAccount.setPluginPermissionsToDefaultValues();

        // save ident to global
        memcpy( m_MyApp.getAppGlobals().getUserIdent(), &m_UserAccount, sizeof( VxNetIdent ) );

        // fill in database info
        LogMsg( LOG_VERBOSE, "created acct name %s UUID 0x%llX 0x%llX\n", m_UserAccount.getOnlineName(), m_UserAccount.getMyOnlineIdLoPart(), m_UserAccount.getMyOnlineIdHiPart() );
        if( true == m_MyApp.getAccountMgr().insertAccount( m_UserAccount ) )
        {
            m_MyApp.getAccountMgr().updateLastLogin( m_UserAccount.getOnlineName() );
            accept();
        }
        else
        {
            LogMsg( LOG_ERROR, "Insert New User into Database failed." );
            QMessageBox::warning( this, QObject::tr( "Application" ), QObject::tr( "Insert New User into Database failed." ) );
        }
    }
}

//============================================================================
//! validate user input
bool ActivityCreateAccount::accountValidate( void )
{
    QString strUserName = ui.m_UserNameEdit->text();
    bool validAccount = GuiHelpers::validateUserName( this, strUserName );

    QString strMoodMsg = ui.m_MoodMessageEdit->text();
    validAccount &= GuiHelpers::validateMoodMessage( this, strMoodMsg );

    validAccount &= GuiHelpers::validateAge( this, ui.m_AgeComboBox->currentIndex() );

    return validAccount;
}

//============================================================================
bool ActivityCreateAccount::wasLoginNameEntered( void )
{
    QString strUserName = ui.m_UserNameEdit->text();
    return strUserName.length() > 3 ? true : false;
}

//============================================================================
void ActivityCreateAccount::onBackButtonClicked( void )
{
    // do nothing.. user must click login
}
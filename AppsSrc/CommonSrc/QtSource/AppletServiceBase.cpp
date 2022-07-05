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
// http://www.nolimitconnect.org
//============================================================================

#include "AppletServiceBase.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "MyIcons.h"
#include "GuiHelpers.h"
#include "GuiParams.h"

#include <CoreLib/VxDebug.h>

//============================================================================
AppletServiceBase::AppletServiceBase( const char * objName, AppCommon& app, QWidget * parent )
    : AppletBase( objName, app, parent )
{
}

//============================================================================
void AppletServiceBase::setupServiceBaseApplet( EApplet applet, EPluginType pluginType )
{
    setAppletType( applet );
    setPluginType( pluginType );

    ui.setupUi( getContentItemsFrame() );
    setTitleBarText( DescribeApplet( m_EAppletType ) );

    getServiceUrlLabel()->setVisible( false );
    getServiceUrlEdit()->setVisible( false );
    getStartButton()->setVisible( false );
    getStopButton()->setVisible( false );
    getInformationWidget()->setVisible( false );

    // search keyworkds not yet supported
    getServiceKeyWordsLabel()->setVisible( false );
    getServiceKeyWordsEdit()->setVisible( false );

    getServiceDescriptionEdit()->setMaximumHeight( GuiParams::getDefaultFontHeight() * 3 + 4 );

    GuiHelpers::fillLanguage( ui.m_LanguageComboBox );
    GuiHelpers::fillContentRating( ui.m_ContentRatingComboBox );

    if( ePluginTypeInvalid == pluginType )
    {
        pluginType = GuiHelpers::getAppletAssociatedPlugin( m_EAppletType );
    }

    if( ePluginTypeInvalid != pluginType )
    {
        getPermissionWidget()->setPluginType( pluginType );
        getServiceTitle()->setText( GuiParams::describePlugin( pluginType, false ).c_str() );
        ui.m_InfoWidget->setPluginType( pluginType );
    }

    connectServiceWidgets();
}

//============================================================================
void AppletServiceBase::connectServiceWidgets()
{
    connect( ui.m_ApplyButton, SIGNAL( clicked() ), this, SLOT( slotApplyServiceSettings() ) );
}

//============================================================================
void AppletServiceBase::loadPluginSetting()
{
    if( ePluginTypeInvalid != getPluginType() )
    {
        m_OrigPermissionLevel = m_MyApp.getAppGlobals().getUserIdent()->getPluginPermission( getPluginType() );
        getPermissionWidget()->setPermissionLevel( m_OrigPermissionLevel );
        m_MyApp.getEngine().getPluginSettingMgr().getPluginSetting( getPluginType(), m_PluginSetting );
        loadUiFromSetting();
    }
}

//============================================================================
void AppletServiceBase::savePluginSetting()
{
    if( ( ePluginTypeInvalid != getPluginType() ) && ( ePluginTypeInvalid != m_PluginSetting.getPluginType() ) )
    {
        saveUiToSetting();
        m_MyApp.getEngine().getPluginSettingMgr().setPluginSetting( m_PluginSetting );
    }
}

//============================================================================
void AppletServiceBase::loadUiFromSetting()
{
    if( ePluginTypeInvalid != getPluginType() )
    {
        ui.m_ContentRatingComboBox->setCurrentIndex( GuiHelpers::contentRatingToIndex( m_PluginSetting.getContentRating() ) );
        ui.m_LanguageComboBox->setCurrentIndex( GuiHelpers::languageToIndex( m_PluginSetting.getLanguage() ) );
        ui.m_UrlEdit->setText( m_PluginSetting.getPluginUrl().c_str() );
        ui.m_NameEdit->setText( m_PluginSetting.getTitle().c_str() );
        ui.m_DescriptionEdit->appendPlainText( m_PluginSetting.getDescription().c_str() );
        ui.m_ThumbnailEditWidget->loadThumbnail( m_PluginSetting.getThumnailId() );
        ui.m_KeyWordsEdit->setText( m_PluginSetting.getKeyWords().c_str() );
    }
}

//============================================================================
void AppletServiceBase::saveUiToSetting()
{
    if( ePluginTypeInvalid != getPluginType() )
    {
        m_PluginSetting.setContentRating( ( EContentRating)ui.m_ContentRatingComboBox->currentIndex() );
        m_PluginSetting.setLanguage( ( ELanguageType )ui.m_LanguageComboBox->currentIndex() );
        m_PluginSetting.setPluginUrl( ui.m_UrlEdit->text().toUtf8().constData() );
        m_PluginSetting.setTitle( ui.m_NameEdit->text().toUtf8().constData() );
        m_PluginSetting.setKeyWords( ui.m_KeyWordsEdit->text().toUtf8().constData() );
        m_PluginSetting.setThumnailId( ui.m_ThumbnailEditWidget->updateAndGetThumbnailId(), ui.m_ThumbnailEditWidget->getThumbnailIsCircular() );

        QString description = ui.m_DescriptionEdit->toPlainText().trimmed();
        if( !description.isEmpty() )
        {
            m_PluginSetting.setDescription( description.toUtf8().constData() );
        }
        else
        {
            m_PluginSetting.setDescription( "" );
        }
    }
}

//============================================================================
void AppletServiceBase::slotApplyServiceSettings()
{
    saveUiToSetting();
    m_MyApp.getEngine().getPluginSettingMgr().setPluginSetting( m_PluginSetting );
    QMessageBox::information( this, QObject::tr( "Service Settings" ), QObject::tr( "Service Settings Applied" ), QMessageBox::Ok );
}

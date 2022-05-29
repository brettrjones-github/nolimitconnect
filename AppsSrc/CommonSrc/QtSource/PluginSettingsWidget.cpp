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
// http://www.nolimitconnect.org
//============================================================================

#include "PluginSettingsWidget.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "MyIcons.h"
#include "GuiHelpers.h"

#include <CoreLib/VxDebug.h>

//============================================================================
PluginSettingsWidget::PluginSettingsWidget( QWidget * parent )
    : QWidget( parent )
    , m_MyApp( GetAppInstance() )
{
    m_ParentApplet = GuiHelpers::findParentApplet( parent );
    ui.setupUi( this );
    connect( ui.m_ThumbnailChooseWidget, SIGNAL( signalThumbnailAssetChanged( ThumbInfo * ) ), this, SLOT( slotThumbnailAssetChanged( ThumbInfo * ) ) );
    ui.m_DescriptionEdit->setFixedHeight( GuiParams::getButtonSize( eButtonSizeSmall ).height() );
    ui.m_GreetingEdit->setFixedHeight( GuiParams::getButtonSize( eButtonSizeSmall ).height() );
    ui.m_RejectEdit->setFixedHeight( GuiParams::getButtonSize( eButtonSizeSmall ).height() );
}

//============================================================================
void PluginSettingsWidget::setupSettingsWidget( EApplet applet, EPluginType pluginType )
{
    m_EAppletType = applet;
    m_PluginType = pluginType;

    getServiceUrlLabel()->setVisible( false );
    getServiceUrlEdit()->setVisible( false );
    getStartButton()->setVisible( false );
    getStopButton()->setVisible( false );
    getInformationWidget()->setVisible( false );

    GuiHelpers::fillAge( ui.m_AgeComboBox );
    GuiHelpers::fillGender( ui.m_GenderComboBox );
    GuiHelpers::fillLanguage( ui.m_LanguageComboBox );
    GuiHelpers::fillContentRating( ui.m_ContentRatingComboBox );

    if( ePluginTypeInvalid == pluginType )
    {
        pluginType = GuiHelpers::getAppletAssociatedPlugin( m_EAppletType );
    }

    if( ePluginTypeInvalid != pluginType )
    {
        getPermissionWidget()->setPluginType( pluginType );
        ui.m_InfoWidget->setPluginType( pluginType );
    }

    connectServiceWidgets();
}

//============================================================================
void PluginSettingsWidget::connectServiceWidgets()
{
    connect( ui.m_ApplyButton, SIGNAL( clicked() ), this, SLOT( slotApplyServiceSettings() ) );
}

//============================================================================
void PluginSettingsWidget::loadPluginSetting()
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
void PluginSettingsWidget::savePluginSetting()
{
    if( ( ePluginTypeInvalid != getPluginType() ) && ( ePluginTypeInvalid != m_PluginSetting.getPluginType() ) )
    {
        saveUiToSetting();
        m_MyApp.getEngine().getPluginSettingMgr().setPluginSetting( m_PluginSetting );
    }
}

//============================================================================
void PluginSettingsWidget::loadUiFromSetting()
{
    if( ePluginTypeInvalid != getPluginType() )
    {
        ui.m_ContentRatingComboBox->setCurrentIndex( GuiHelpers::contentRatingToIndex( m_PluginSetting.getContentRating() ) );
        ui.m_LanguageComboBox->setCurrentIndex( GuiHelpers::languageToIndex( m_PluginSetting.getLanguage() ) );
        ui.m_UrlEdit->setText( m_PluginSetting.getPluginUrl().c_str() );
        ui.m_ServiceTitleEdit->setText( m_PluginSetting.getTitle().c_str() );
        ui.m_DescriptionEdit->appendPlainText( m_PluginSetting.getDescription().c_str() );
        ui.m_ThumbnailChooseWidget->loadThumbnail( m_PluginSetting.getThumnailId(), m_PluginSetting.getThumbnailIsCircular() );
    }
}

//============================================================================
void PluginSettingsWidget::saveUiToSetting()
{
    if( ePluginTypeInvalid != getPluginType() )
    {
        m_PluginSetting.setContentRating( ( EContentRating)ui.m_ContentRatingComboBox->currentIndex() );
        m_PluginSetting.setLanguage( ( ELanguageType )ui.m_LanguageComboBox->currentIndex() );
        m_PluginSetting.setPluginUrl( ui.m_UrlEdit->text().toUtf8().constData() );
        m_PluginSetting.setTitle( ui.m_ServiceTitleEdit->text().toUtf8().constData() );
        m_PluginSetting.setThumnailId( ui.m_ThumbnailChooseWidget->getThumbnailId(), ui.m_ThumbnailChooseWidget->getThumbnailIsCircular() );

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
void PluginSettingsWidget::slotApplyServiceSettings()
{
    saveUiToSetting();
    m_MyApp.getEngine().getPluginSettingMgr().setPluginSetting( m_PluginSetting );
    emit signalPluginSettingsApplied();
}

//============================================================================
void PluginSettingsWidget::slotThumbnailAssetChanged( ThumbInfo* thumbAsset )
{
    VxGUID thumbGuid = ui.m_ThumbnailChooseWidget->getAssetId();
    LogMsg( LOG_DEBUG, "slotThumbnailAssetChanged %s", thumbGuid.toGUIDStandardFormatedString().c_str() );
    m_PluginSetting.setThumnailId( thumbGuid, ui.m_ThumbnailChooseWidget->getThumbnailIsCircular() );
    ui.m_ThumbnailChooseWidget->getThumbnailViewWidget()->updateAssetImage( thumbAsset );
}

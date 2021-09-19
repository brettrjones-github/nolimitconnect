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

#include <app_precompiled_hdr.h>
#include "SearchParamsWidget.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "MyIcons.h"
#include "GuiHelpers.h"

#include <CoreLib/VxDebug.h>

//============================================================================
SearchParamsWidget::SearchParamsWidget( QWidget * parent )
    : QWidget( parent )
    , m_MyApp( GetAppInstance() )
{
    m_ParentApplet = GuiHelpers::findParentApplet( parent );
    ui.setupUi( this );

    getSearchTypeComboBox()->setVisible( false );
    getStopButton()->setEnabled( false );

    GuiHelpers::fillAge( ui.m_AgeComboBox );
    GuiHelpers::fillGender( ui.m_GenderComboBox );
    GuiHelpers::fillLanguage( ui.m_LanguageComboBox );
    GuiHelpers::fillContentRating( ui.m_ContentRatingComboBox );

    connect( getStartButton(), SIGNAL( clicked() ), this, SLOT( slotStartSearch() ) );
    connect( getStopButton(), SIGNAL( clicked() ), this, SLOT( slotStopSearch() ) );
}

//============================================================================
void SearchParamsWidget::setupSearchParamsWidget( EApplet applet, EPluginType pluginType )
{
    m_EAppletType = applet;
    m_PluginType = pluginType;

    if( ePluginTypeInvalid == pluginType )
    {
        pluginType = GuiHelpers::getAppletAssociatedPlugin( m_EAppletType );
    }
}

//============================================================================
void SearchParamsWidget::slotStartSearch()
{
    if( !m_SearchStarted )
    {
        getStartButton()->setEnabled( false );
        getStopButton()->setEnabled( true );
        m_SearchStarted = true;
        emit signalSearchState( true );
    }
}

//============================================================================
void SearchParamsWidget::slotStopSearch()
{
    if( m_SearchStarted )
    {
        m_SearchStarted = false;
        getStartButton()->setEnabled( true );
        getStopButton()->setEnabled( false );
        emit signalSearchState( false );
    }
}

//============================================================================
void SearchParamsWidget::slotSearchCancel()
{
    if( m_SearchStarted )
    {
        m_SearchStarted = false;
        getStartButton()->setEnabled( true );
        getStopButton()->setEnabled( false );
    }
}

//============================================================================
void SearchParamsWidget::slotSearchComplete()
{
    if( m_SearchStarted )
    {
        m_SearchStarted = false;
        getStartButton()->setEnabled( true );
        getStopButton()->setEnabled( false );
    }
}

//============================================================================
bool SearchParamsWidget::toSearchParams( SearchParams& params )
{
    params.setAgeType( GuiHelpers::getAge( ui.m_AgeComboBox ) );
    params.setGender( GuiHelpers::getGender( ui.m_GenderComboBox ) );
    params.setLanguage( GuiHelpers::getLanguage( ui.m_LanguageComboBox ) );
    params.setContentRating( GuiHelpers::getContentRating( ui.m_ContentRatingComboBox ) );
    std::string searchText = ui.m_SearchTextEdit->text().isEmpty() ? "" : ui.m_SearchTextEdit->text().toUtf8().constData();
    params.setSearchListAll( m_SearchParams.getSearchListAll() );
    params.setSearchText( searchText );
    VxGUID::generateNewVxGUID(params.getSearchSessionId());
    return true;
}

//============================================================================
bool SearchParamsWidget::fromSearchParams( SearchParams& params )
{
    bool result = GuiHelpers::setAge( ui.m_AgeComboBox, params.getAgeType() );
    result &= GuiHelpers::setGender( ui.m_GenderComboBox, params.getGender() );
    result &= GuiHelpers::setLanguage( ui.m_LanguageComboBox, params.getLanguage() );
    result &= GuiHelpers::setContentRating( ui.m_ContentRatingComboBox, params.getContentRating() );
    std::string searchText = params.getSearchText();
    ui.m_SearchTextEdit->setText( searchText.empty() ? "" : searchText.c_str() );
    return result;
}
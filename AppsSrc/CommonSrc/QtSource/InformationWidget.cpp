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

#include "InformationWidget.h"
#include "AppCommon.h"
#include "MyIcons.h"
#include "ActivityInformation.h"

//============================================================================
InformationWidget::InformationWidget( QWidget * parent )
	: QWidget( parent )
    , m_MyApp( GetAppInstance() )
{
	initInformationWidget();
}

//============================================================================
void InformationWidget::initInformationWidget( void )
{
	ui.setupUi( this );

    ui.m_InfoButton->setIcon( eMyIconInformation );

    connect( ui.m_ContextButton, SIGNAL( clicked() ), this, SLOT( slotShowInformation() ) );
    connect( ui.m_InfoButton, SIGNAL( clicked() ), this, SLOT( slotShowInformation() ) );
}

//============================================================================
void InformationWidget::updateInformation( void )
{
    if( m_PluginType == ePluginTypeInvalid )
    {
        return;
    }

    ui.m_ContextButton->setIcon( m_MyApp.getMyIcons().getPluginIcon( m_PluginType ) );
}

//============================================================================
void InformationWidget::slotShowInformation()
{
   if( m_PluginType == ePluginTypeInvalid )
   {
       return;
   }

   m_ActivityInfo = new ActivityInformation( m_MyApp, this, m_PluginType );
   m_ActivityInfo->show();
}
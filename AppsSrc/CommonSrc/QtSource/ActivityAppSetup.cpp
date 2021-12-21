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
// http://www.nolimitconnect.com
//============================================================================

#include "ActivityAppSetup.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "GuiHelpers.h"
#include "GuiParams.h"

#include <CoreLib/VxDebug.h>

#include <QClipboard>

//============================================================================
ActivityAppSetup::ActivityAppSetup( AppCommon& app, QWidget * parent )
: ActivityBase( OBJNAME_ACTIVITY_APP_SETUP, app, parent, eActivityAppSetup, true, false, true )
{
    initActivityAppSetup();
}

//============================================================================
void ActivityAppSetup::initActivityAppSetup( void )
{
    ui.setupUi( this );
    ui.m_TitleBarWidget->setTitleBarText( QObject::tr( "Initialize Application " ) );
    ui.m_TitleBarWidget->setBackButtonVisibility( false );
    ui.m_TitleBarWidget->setHomeButtonVisibility( false );

    ui.m_BottomBarWidget->setExpandWindowVisibility( false );

    ui.m_AvailSpaceBefore->setText( GuiHelpers::getAvailableStorageSpaceText() );

    connectBarWidgets();
    m_AppSetup = new AppSetup();
    connect( m_AppSetup, SIGNAL( signalSetupResult( QObject *, int, qint64 ) ), this, SLOT( slotCopyResult( QObject *, int, qint64 ) ) );
    m_AppSetup->initializeUserAssets();
}

//============================================================================
void ActivityAppSetup::showEvent( QShowEvent * ev )
{
    ActivityBase::showEvent( ev );
}

//============================================================================
void ActivityAppSetup::slotCopyToClipboardButtonClicked( void )
{
    //QClipboard * clipboard = QApplication::clipboard();
   // clipboard->setText( ui.m_InfoText->toPlainText() );
}

//============================================================================
void ActivityAppSetup::slotCopyResult( QObject * workerObj, int rc, qint64 bytesCopied )
{
    if( rc )
    {
        LogMsg( LOG_INFO, "Setup error %d at %" PRId64 " bytes", rc, bytesCopied );
        ui.m_TitleBarWidget->setBackButtonVisibility( true );
    }
    else
    {
        LogMsg( LOG_INFO, "Setup copied %" PRId64 " bytes", bytesCopied );
        QString bytesStr = GuiParams::describeFileLength( bytesCopied );

        QMessageBox::information( this, QObject::tr("Setup completed"), QObject::tr( "Setup success. Bytes Copied " ) + bytesStr, QMessageBox::Ok );
        close();
    }

    m_SetupCompleted = true;
}

//============================================================================
// Copyright (C) 2018 Brett R. Jones
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
// http://www.nolimitconnect.org
//============================================================================

#include <QWidget> // must be declared first or linux Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include "LogWidget.h"
#include "AppSettings.h"

#include <QScrollBar>
#include <QClipboard>

namespace
{
    const int MAX_LOG_EDIT_BLOCK_CNT = 1000;
}

//============================================================================
LogWidget::LogWidget( QWidget * parent )
    : QWidget( parent )
    , m_MyApp( GetAppInstance() )
{
    ui.setupUi( this );
    getLogEdit()->setMaximumBlockCount( MAX_LOG_EDIT_BLOCK_CNT );
    getLogEdit()->setReadOnly( true );

    m_VerboseLog = m_MyApp.getAppSettings().getVerboseLog();
    ui.m_VerboseCheckBox->setChecked( m_VerboseLog );
    connect( ui.m_VerboseCheckBox, SIGNAL( clicked() ), this, SLOT( slotVerboseCheckBoxClicked() ) );
    connect( ui.m_CopyToClipboardButton, SIGNAL( clicked() ), this, SLOT( slotCopyToClipboardClicked() ) );

    connect( this, SIGNAL( signalLogMsg( const QString& ) ), this, SLOT( slotLogMsg( const QString& ) ) );

}

//============================================================================
LogWidget::~LogWidget()
{
    VxRemoveLogHandler( this );
}

//============================================================================
void LogWidget::initLogCallback()
{
    VxAddLogHandler( this );
}

//============================================================================
void LogWidget::onLogEvent( uint32_t u32LogFlags, const char * logMsg )
{
    m_LogMutex.lock();
    if( m_VerboseLog
        || ( u32LogFlags & ~LOG_VERBOSE ) )
    {
        QString logStr( logMsg );
#if QT_VERSION > QT_VERSION_CHECK(6,0,0)
        logStr.remove(QRegularExpression("[\\n\\r]"));
#else
        logStr.remove(QRegExp("[\\n\\r]"));
#endif // QT_VERSION > QT_VERSION_CHECK(6,0,0)
        emit signalLogMsg( logStr );
    }

    m_LogMutex.unlock();
}

//============================================================================
void LogWidget::slotLogMsg( const QString& text )
{  
    getLogEdit()->appendPlainText( text ); // Adds the message to the widget
    getLogEdit()->verticalScrollBar()->setValue( getLogEdit()->verticalScrollBar()->maximum() ); // Scrolls to the bottom
    //m_LogFile.write( text ); // Logs to file
}

//============================================================================
void LogWidget::slotVerboseCheckBoxClicked( void )
{
    m_VerboseLog = ui.m_VerboseCheckBox->isChecked();
    m_MyApp.getAppSettings().setVerboseLog( m_VerboseLog );
}

//============================================================================
void LogWidget::slotCopyToClipboardClicked( void )
{
    QClipboard * clipboard = QApplication::clipboard();
    clipboard->setText( getLogEdit()->toPlainText() );
}

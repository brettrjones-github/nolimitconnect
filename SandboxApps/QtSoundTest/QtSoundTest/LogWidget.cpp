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

#include "LogWidget.h"
#include "VxDebug.h"


#include <QScrollBar>

namespace
{
    const int MAX_LOG_EDIT_BLOCK_CNT = 1000;

    void LogHandler( void * userData, uint32_t u32LogFlags, char * logMsg )
    {
        if( userData )
        {
            ( (LogWidget *)userData )->toGuiLog( u32LogFlags, logMsg );
        }
    }
}


//============================================================================
LogWidget::LogWidget( QWidget * parent )
    : QPlainTextEdit( parent )
{
    setMaximumBlockCount( MAX_LOG_EDIT_BLOCK_CNT );
    VxSetLogHandler( LogHandler, this );
    connect( this, SIGNAL( signalLogMsg( const QString& ) ), this, SLOT( slotLogMsg( const QString& ) ) );
}

//============================================================================
void LogWidget::toGuiLog( uint32_t u32LogFlags, char * logMsg )
{
    m_LogMutex.lock();

    QString logStr( logMsg );
    logStr.remove( QRegExp( "[\\n\\r]" ) );
    emit signalLogMsg( logStr );

    m_LogMutex.unlock();
}

//============================================================================
void LogWidget::slotLogMsg( const QString& text )
{  
    appendPlainText( text ); // Adds the message to the widget
    verticalScrollBar()->setValue( verticalScrollBar()->maximum() ); // Scrolls to the bottom
    //m_LogFile.write( text ); // Logs to file
}
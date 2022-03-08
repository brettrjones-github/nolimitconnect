
//============================================================================
// Copyright (C) 2022 Brett R. Jones
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

#define QT_NO_PRINTER
#include "RichTextWidget.h"

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QColorDialog>
#include <QComboBox>
#include <QFontComboBox>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontDatabase>
//#include <QPrintDialog>
//#include <QPrinter>
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
//#include <QTextDecoder>
#else
#include <QTextCodec>
#endif // QT_VERSION >= QT_VERSION_CHECK(6,0,0)
#include <QTextEdit>
#include <QToolBar>
#include <QTextCursor>
#include <QTextDocumentWriter>
#include <QTextList>
#include <QtDebug>
#include <QCloseEvent>
#include <QMessageBox>
//#include <QPrintPreviewDialog>
#include <QUrl>
#include <QImageReader>
#include <QMimeData>
#include <QInputDialog>

#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxDebug.h>

//============================================================================
RichTextWidget::RichTextWidget(QWidget *parent, QString initialFile)
: QTextEdit(parent)
, m_strFileName(initialFile)
{
    setReadOnly( true );
}

//============================================================================
void RichTextWidget::loadRichTextFile( const char * pFileName )
{
    if( pFileName )
    {
        m_strFileName = pFileName;
        if( !m_strFileName.isEmpty() )
        {
            
            if( !load( m_strFileName ) )
            {
                LogMsg( LOG_ERROR, "loadRichTextBoardFile failed %s", m_strFileName.toUtf8().constData() );
            }
        }
        else
        {
            LogMsg( LOG_ERROR, "loadRichTextBoardFile empty file name " );
        }
    }
    else
    {
        LogMsg( LOG_ERROR, "loadRichTextBoardFile null file name " );
    }
}

//============================================================================
bool RichTextWidget::load( const QString &richFileName )
{
    if( richFileName.isEmpty() )
    {
        LogMsg( LOG_ERROR, "RichTextWidget::load empty file name " );
        return false;
    }


    QFileInfo fileInfo( richFileName );
    if( !fileInfo.exists( ) )
    {
        LogMsg( LOG_ERROR, "RichTextWidget::load file %s does not exist ", fileInfo.absoluteFilePath().toUtf8().constData() );
        return false;
    }

    VxFileUtil::setCurrentWorkingDirectory( fileInfo.dir().path().toUtf8().constData() );

    QFile file( richFileName );
    if( !file.open( QFile::ReadOnly ) )
	{
        return false;
	}

    QByteArray data = file.readAll();

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
	QString strText = QString::fromUtf8( data );
#else
	QTextCodec *codec = Qt::codecForHtml( data );
	QString strText = codec->toUnicode( data );
#endif // QT_VERSION >= QT_VERSION_CHECK(6,0,0)

    if( Qt::mightBeRichText( strText ) ) 
	{
        setHtml( strText );
    } 
	else 
	{
        strText = QString::fromLocal8Bit( data );
        setPlainText( strText );
    }


    setCurrentFileName( richFileName );
    return true;
}

//============================================================================
bool RichTextWidget::maybeSave( void )
{
    if (!document()->isModified())
        return true;
    if (m_strFileName.startsWith(QLatin1String(":/")))
        return true;
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("RichText Board"),
                               tr("The document has been modified.\n"
                                  "Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Discard
                               | QMessageBox::Cancel);
    if( ret == QMessageBox::Save )
	{
        return fileSave();
	}
    else if( ret == QMessageBox::Cancel )
	{
        return false;
	}

    return true;
}

//============================================================================
void RichTextWidget::setCurrentFileName(const QString &strFileName)
{
    this->m_strFileName = strFileName;
    document()->setModified(false);

    QString shownName;
    if (m_strFileName.isEmpty())
        shownName = "untitled.txt";
    else
        shownName = QFileInfo(strFileName).fileName();

    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("Rich Text")));
    setWindowModified(false);
}

//============================================================================
void RichTextWidget::fileOpen( void )
{
    QString fileName = QFileDialog::getOpenFileName(this, 
			tr("Open File..."),
            QString(), tr("HTML-Files (*.htm *.html);;All Files (*)"));
    if( !fileName.isEmpty() )
	{
        load(fileName);
	}
}

//============================================================================
bool RichTextWidget::fileSave( void )
{
    if( m_strFileName.isEmpty() )
	{
        //return fileSaveAs();
	}

    QTextDocumentWriter writer( m_strFileName );
    bool bSuccess = writer.write( document() );
    if( bSuccess )
	{
        document()->setModified( false );
	}

    return bSuccess;
}

//============================================================================
bool RichTextWidget::fileSaveAs( void )
{
    QString fn = QFileDialog::getSaveFileName(this, tr("Save as..."),
                                              QString(), tr("ODF files (*.odt);;HTML-Files (*.htm *.html);;All Files (*)"));
    if (fn.isEmpty())
        return false;
    if (! (fn.endsWith(".odt", Qt::CaseInsensitive) || fn.endsWith(".htm", Qt::CaseInsensitive) || fn.endsWith(".html", Qt::CaseInsensitive)) )
        fn += ".odt"; // default
    setCurrentFileName(fn);
    return fileSave();
}

//============================================================================
void RichTextWidget::filePrint( void )
{
#ifndef QT_NO_PRINTER
    QPrinter printer( QPrinter::HighResolution );
    QPrintDialog *dlg = new QPrintDialog( &printer, this );
    if( m_TextEdit->textCursor().hasSelection() )
	{
        dlg->addEnabledOption( QAbstractPrintDialog::PrintSelection );
	}
    dlg->setWindowTitle( tr("Print Document") );
    if( dlg->exec() == QDialog::Accepted ) 
	{
        m_TextEdit->print(&printer);
    }
    delete dlg;
#endif
}

//============================================================================
void RichTextWidget::filePrintPreview( void )
{
#ifndef QT_NO_PRINTER
    QPrinter printer( QPrinter::HighResolution );
    QPrintPreviewDialog preview( &printer, this );
    connect( &preview, SIGNAL(paintRequested(QPrinter*)), SLOT(printPreview(QPrinter*)) );
    preview.exec();
#endif
}

//============================================================================
void RichTextWidget::printPreview(QPrinter *printer)
{
#ifdef QT_NO_PRINTER
    Q_UNUSED(printer);
#else
    print(printer);
#endif
}

//============================================================================
void RichTextWidget::filePrintPdf( void )
{
#ifndef QT_NO_PRINTER
    QString m_strFileName = QFileDialog::getSaveFileName(this, "Export PDF",
                                                    QString(), "*.pdf");
    if (!m_strFileName.isEmpty()) 
	{
        if (QFileInfo(m_strFileName).suffix().isEmpty())
		{
            m_strFileName.append(".pdf");
		}

        QPrinter printer( QPrinter::HighResolution );
        printer.setOutputFormat( QPrinter::PdfFormat );
        printer.setOutputFileName( m_strFileName );
        document()->print( &printer );
    }
#endif
}

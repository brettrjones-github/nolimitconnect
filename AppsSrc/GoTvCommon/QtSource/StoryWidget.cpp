
#include <app_precompiled_hdr.h>

#define QT_NO_PRINTER
#include "StoryWidget.h"

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

//============================================================================
StoryWidget::StoryWidget(QWidget *parent, QString initialFile)
: QWidget(parent)
, m_strFileName(initialFile)
{
	ui.setupUi(this);
	m_TextEdit = ui.m_StoryTextEdit;
	ui.m_FileButtonsFrame->setVisible( false );

    int buttonWidth = 16;
    QSize buttonSize( buttonWidth, buttonWidth );
    ui.m_CreateHyperLinkButton->setFixedSize( buttonSize );
    ui.m_FileNewButton->setFixedSize( buttonSize );
    ui.m_FileOpenButton->setFixedSize( buttonSize );
    ui.m_FileSaveAsButton->setFixedSize( buttonSize );
    // ui.m_ExportPdfButton->setFixedSize( buttonSize );
    ui.m_FileSaveButton->setFixedSize( buttonSize );
    ui.m_InsertPictureButton->setFixedSize( buttonSize );
    ui.m_UndoButton->setFixedSize( buttonSize );
    ui.m_RedoButton->setFixedSize( buttonSize );
    ui.m_CutButton->setFixedSize( buttonSize );
    ui.m_CopyButton->setFixedSize( buttonSize );
    ui.m_PasteButton->setFixedSize( buttonSize );
    ui.m_PrinterButton->setFixedSize( buttonSize );
    ui.m_BoldButton->setFixedSize( buttonSize );
    ui.m_ItalicsButton->setFixedSize( buttonSize );
    ui.m_UnderlineButton->setFixedSize( buttonSize );
    ui.m_LeftJustifyButton->setFixedSize( buttonSize );
    ui.m_CenterTextButton->setFixedSize( buttonSize );
    ui.m_RightJustifyButton->setFixedSize( buttonSize );
    ui.m_FullLineButton->setFixedSize( buttonSize );
    ui.m_TextColorButton->setFixedSize( buttonSize );


    ui.m_CreateHyperLinkButton->setIcon( eMyIconEditHyperLink );
    ui.m_FileNewButton->setIcon( eMyIconEditFileNew );
    ui.m_FileOpenButton->setIcon( eMyIconFileOpen );
    ui.m_FileSaveAsButton->setIcon( eMyIconFileSaveAs );

    // ui.m_ExportPdfButton->setIcon( eMyIconEditFileNew );

    ui.m_FileSaveButton->setIcon( eMyIconFileSave );
    ui.m_InsertPictureButton->setIcon( eMyIconEditFileNew );

    ui.m_UndoButton->setIcon( eMyIconEditUndo );
    ui.m_RedoButton->setIcon( eMyIconEditRedo );
    ui.m_CutButton->setIcon( eMyIconEditCut );
    ui.m_CopyButton->setIcon( eMyIconEditCopy );

    ui.m_PasteButton->setIcon( eMyIconEditPaste );
    ui.m_PrinterButton->setIcon( eMyIconEditPrint );
    ui.m_BoldButton->setIcon( eMyIconEditBold );
    ui.m_ItalicsButton->setIcon( eMyIconEditItalic );
    ui.m_UnderlineButton->setIcon( eMyIconEditUnderline );

    ui.m_LeftJustifyButton->setIcon( eMyIconEditAlignLeft );
    ui.m_CenterTextButton->setIcon( eMyIconEditAlignCenter );
    ui.m_RightJustifyButton->setIcon( eMyIconEditAlignRight );
    ui.m_FullLineButton->setIcon( eMyIconEditAlignLeftAndRight );

    ui.m_TextColorButton->setIcon( eMyIconEditText );

    setupFileActions();
    setupEditActions();
    setupTextActions();
    connect(m_TextEdit, SIGNAL(currentCharFormatChanged(QTextCharFormat)),
            this, SLOT(currentCharFormatChanged(QTextCharFormat)));
    connect(m_TextEdit, SIGNAL(cursorPositionChanged()),
            this, SLOT(cursorPositionChanged()));

    m_TextEdit->setFocus();
    setCurrentFileName(QString());

    fontChanged( m_TextEdit->font() );
    colorChanged( m_TextEdit->textColor() );

    connect(m_TextEdit->document(), SIGNAL(modificationChanged(bool)),
            ui.m_FileSaveButton, SLOT(setEnabled(bool)));
    connect(m_TextEdit->document(), SIGNAL(modificationChanged(bool)),
            this, SLOT(setWindowModified(bool)));
    connect(m_TextEdit->document(), SIGNAL(undoAvailable(bool)),
            ui.m_UndoButton, SLOT(setEnabled(bool)));
    connect(m_TextEdit->document(), SIGNAL(redoAvailable(bool)),
            ui.m_RedoButton, SLOT(setEnabled(bool)));

    setWindowModified( m_TextEdit->document()->isModified() );
    ui.m_FileSaveButton->setEnabled(m_TextEdit->document()->isModified());
    ui.m_UndoButton->setEnabled(m_TextEdit->document()->isUndoAvailable());
    ui.m_RedoButton->setEnabled(m_TextEdit->document()->isRedoAvailable());

    connect(ui.m_UndoButton, SIGNAL(clicked()), m_TextEdit, SLOT(undo()));
    connect(ui.m_RedoButton, SIGNAL(clicked()), m_TextEdit, SLOT(redo()));

    ui.m_CutButton->setEnabled(false);
    ui.m_CopyButton->setEnabled(false);

    connect(ui.m_CutButton, SIGNAL(clicked()), m_TextEdit, SLOT(cut()));
    connect(ui.m_CopyButton, SIGNAL(clicked()), m_TextEdit, SLOT(copy()));
    connect(ui.m_PasteButton, SIGNAL(clicked()), m_TextEdit, SLOT(paste()));

    connect(m_TextEdit, SIGNAL(copyAvailable(bool)), ui.m_CutButton, SLOT(setEnabled(bool)));
    connect(m_TextEdit, SIGNAL(copyAvailable(bool)), ui.m_CopyButton, SLOT(setEnabled(bool)));

#ifndef QT_NO_CLIPBOARD
    connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(clipboardDataChanged()));
#endif
	connect(m_TextEdit, SIGNAL(currentCharFormatChanged(QTextCharFormat)),
		this, SLOT(currentCharFormatChanged(QTextCharFormat)));
	connect(m_TextEdit, SIGNAL(cursorPositionChanged()),
		this, SLOT(cursorPositionChanged()));

	m_TextEdit->setFocus();
	ui.m_FontSizeComboBox->setCurrentIndex( 6 );
}

//============================================================================
void StoryWidget::loadStoryBoardFile( const char * pFileName )
{
	m_strFileName = pFileName;
	if( !load( m_strFileName ) )
	{
		fileNew();
	}
}

//============================================================================
void StoryWidget::setupFileActions()
{
	connect(ui.m_FileNewButton, SIGNAL(clicked()), this, SLOT(fileNew()));
	connect(ui.m_FileOpenButton, SIGNAL(clicked()), this, SLOT(fileOpen()));
	connect(ui.m_FileSaveButton, SIGNAL(clicked()), this, SLOT(fileSave()));
	connect(ui.m_FileSaveAsButton, SIGNAL(clicked()), this, SLOT(fileSaveAs()));

    ui.m_ExportPdfButton->setVisible( false );
#ifndef QT_NO_PRINTER
	connect(ui.m_PrinterButton, SIGNAL(clicked()), this, SLOT(filePrint()));
	//connect(ui.m_ExportPdfButton, SIGNAL(clicked()), this, SLOT(filePrintPdf()));
#endif
}

//============================================================================
void StoryWidget::setupEditActions()
{
	connect(ui.m_InsertPictureButton, SIGNAL(clicked()), this, SLOT(insertPicture()));
	connect(ui.m_UndoButton, SIGNAL(clicked()), this, SLOT(editActionUndo()));
	connect(ui.m_RedoButton, SIGNAL(clicked()), this, SLOT(editActionRedo()));
	connect(ui.m_CutButton, SIGNAL(clicked()), this, SLOT(editActionCut()));
	connect(ui.m_CopyButton, SIGNAL(clicked()), this, SLOT(editActionCopy()));
	connect(ui.m_PasteButton, SIGNAL(clicked()), this, SLOT(editActionPaste()));

	QAction *a;
	a = new QAction(tr("&Undo"), m_TextEdit);
	a->setShortcut(QKeySequence::Undo);
	a->setPriority(QAction::LowPriority);
	ui.m_UndoButton->addAction(a);
	a->setParent(ui.m_StoryTextEdit);

	a = new QAction(tr("&Redo"), m_TextEdit);
	a->setShortcut(QKeySequence::Redo);
	a->setPriority(QAction::LowPriority);
	ui.m_RedoButton->addAction(a);
	a->setParent(ui.m_StoryTextEdit);

	a = new QAction(tr("Cu&t"), m_TextEdit);
	a->setShortcut(QKeySequence::Cut);
	a->setPriority(QAction::LowPriority);
	ui.m_CutButton->addAction(a);
	a->setParent(ui.m_StoryTextEdit);

	a = new QAction(tr("&Copy"), m_TextEdit);
	a->setShortcut(QKeySequence::Copy);
	a->setPriority(QAction::LowPriority);
	ui.m_CopyButton->addAction(a);
	a->setParent(ui.m_StoryTextEdit);

	a = new QAction(tr("&Paste"), m_TextEdit);
	a->setShortcut(QKeySequence::Paste);
	a->setPriority(QAction::LowPriority);
	ui.m_PasteButton->addAction(a);
	a->setParent(ui.m_StoryTextEdit);

#ifndef QT_NO_CLIPBOARD
    if (const QMimeData *md = QApplication::clipboard()->mimeData())
        ui.m_PasteButton->setEnabled(md->hasText());
#endif
}

//============================================================================
void StoryWidget::setupTextActions()
{
	// format
	connect(ui.m_BoldButton, SIGNAL(clicked()), this, SLOT(textBold()));
	connect(ui.m_ItalicsButton, SIGNAL(clicked()), this, SLOT(textItalic()));
	connect(ui.m_UnderlineButton, SIGNAL(clicked()), this, SLOT(textUnderline()));

	// justify
	connect(ui.m_LeftJustifyButton, SIGNAL(clicked()), this, SLOT(leftJustify()));
	connect(ui.m_CenterTextButton, SIGNAL(clicked()), this, SLOT(centerJustify()));
	connect(ui.m_RightJustifyButton, SIGNAL(clicked()), this, SLOT(rightJustify()));
	connect(ui.m_FullLineButton, SIGNAL(clicked()), this, SLOT(bothJustify()));

	// font color
	QPixmap pix(16, 16);
	pix.fill(Qt::black);
	connect(ui.m_TextColorButton, SIGNAL(clicked()), this, SLOT(textColor()));

	// fonts
	connect(ui.m_FontComboBox , SIGNAL(activated(QString)),
		this, SLOT(textFamily(QString)));

	ui.m_FontSizeComboBox->setEditable(true);
	QFontDatabase db;
	foreach(int size, db.standardSizes())
		ui.m_FontSizeComboBox->addItem(QString::number(size));

	connect(ui.m_FontSizeComboBox, SIGNAL(activated(QString)),
		this, SLOT(textSize(QString)));
	ui.m_FontSizeComboBox->setCurrentIndex(ui.m_FontSizeComboBox->findText(QString::number(QApplication::font()
		.pointSize())));
}

//============================================================================
// shamelessly copied from Qt Demo Browser
static QUrl guessUrlFromString(const QString &string)
{
	QString urlStr = string.trimmed();
#if QT_VERSION > QT_VERSION_CHECK(6,0,0)
	QRegularExpression test(QLatin1String("^[a-zA-Z]+\\:.*"));
	QRegularExpressionMatch match = test.match(urlStr);
	bool hasSchema = match.hasMatch(); // true
#else
	QRegExp test(QLatin1String("^[a-zA-Z]+\\:.*"));
	bool hasSchema = test.exactMatch(urlStr);
#endif // QT_VERSION > QT_VERSION_CHECK(6,0,0)	

	// Check if it looks like a qualified URL. Try parsing it and see.
	if (hasSchema) {
		QUrl url(urlStr, QUrl::TolerantMode);
		if (url.isValid())
			return url;
	}

	// Might be a file.
	if (QFile::exists(urlStr))
		return QUrl::fromLocalFile(urlStr);

	// Might be a shorturl - try to detect the schema.
	if (!hasSchema) {
		int dotIndex = urlStr.indexOf(QLatin1Char('.'));
		if (dotIndex != -1) {
			QString prefix = urlStr.left(dotIndex).toLower();
			QString schema = (prefix == QLatin1String("ftp")) ? prefix : QLatin1String("http");
			QUrl url(schema + QLatin1String("://") + urlStr, QUrl::TolerantMode);
			if (url.isValid())
				return url;
		}
	}

	// Fall back to QUrl's own tolerant parser.
	return QUrl(string, QUrl::TolerantMode);
}

//============================================================================
bool StoryWidget::createLink( void )
{
	QString link = QInputDialog::getText( this, tr("Create link"), "Enter URL" );
	if( !link.isEmpty() ) 
	{
		QUrl url = guessUrlFromString(link);
		if( url.isValid() )
		{
			//execCommand("createLink", url.toString());
			return true;
		}
	}

	return false;
}

//============================================================================
bool StoryWidget::load( const QString &fileName )
{
    if( !QFile::exists( fileName ) )
	{
        return false;
	}
    QFile file( fileName );
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
        m_TextEdit->setHtml( strText );
    } 
	else 
	{
        strText = QString::fromLocal8Bit( data );
        m_TextEdit->setPlainText( strText );
    }

	m_TextEdit->setReadOnly( false );

    setCurrentFileName( fileName );
    return true;
}

//============================================================================
bool StoryWidget::maybeSave()
{
    if (!m_TextEdit->document()->isModified())
        return true;
    if (m_strFileName.startsWith(QLatin1String(":/")))
        return true;
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("Story Board"),
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
void StoryWidget::setCurrentFileName(const QString &strFileName)
{
    this->m_strFileName = strFileName;
    m_TextEdit->document()->setModified(false);

    QString shownName;
    if (m_strFileName.isEmpty())
        shownName = "untitled.txt";
    else
        shownName = QFileInfo(strFileName).fileName();

    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("Rich Text")));
    setWindowModified(false);
}

//============================================================================
void StoryWidget::fileNew()
{
    if( maybeSave() ) 
	{
        m_TextEdit->clear();
        //setCurrentFileName(QString());
		setCurrentFileName( m_strFileName );
    }
}

//============================================================================
void StoryWidget::fileOpen()
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
bool StoryWidget::fileSave()
{
    if( m_strFileName.isEmpty() )
	{
        //return fileSaveAs();
	}

    QTextDocumentWriter writer( m_strFileName );
    bool bSuccess = writer.write( m_TextEdit->document() );
    if( bSuccess )
	{
        m_TextEdit->document()->setModified( false );
		emit signalStoryBoardSavedModified();
	}

    return bSuccess;
}

//============================================================================
bool StoryWidget::fileSaveAs()
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
void StoryWidget::filePrint()
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
void StoryWidget::filePrintPreview()
{
#ifndef QT_NO_PRINTER
    QPrinter printer( QPrinter::HighResolution );
    QPrintPreviewDialog preview( &printer, this );
    connect( &preview, SIGNAL(paintRequested(QPrinter*)), SLOT(printPreview(QPrinter*)) );
    preview.exec();
#endif
}

//============================================================================
void StoryWidget::printPreview(QPrinter *printer)
{
#ifdef QT_NO_PRINTER
    Q_UNUSED(printer);
#else
    m_TextEdit->print(printer);
#endif
}

//============================================================================
void StoryWidget::filePrintPdf()
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
        m_TextEdit->document()->print( &printer );
    }
#endif
}

//============================================================================
void StoryWidget::textBold()
{
    QTextCharFormat fmt;
    fmt.setFontWeight( ui.m_BoldButton->isChecked() ? QFont::Bold : QFont::Normal );
    mergeFormatOnWordOrSelection( fmt );
}

//============================================================================
void StoryWidget::textUnderline()
{
    QTextCharFormat fmt;
    fmt.setFontUnderline( ui.m_UnderlineButton->isChecked() );
    mergeFormatOnWordOrSelection( fmt );
}

//============================================================================
void StoryWidget::textItalic()
{
    QTextCharFormat fmt;
    fmt.setFontItalic( ui.m_ItalicsButton->isChecked() );
    mergeFormatOnWordOrSelection(fmt);
}

//============================================================================
void StoryWidget::textFamily(const QString &f)
{
    QTextCharFormat fmt;
    fmt.setFontFamily( f );
    mergeFormatOnWordOrSelection( fmt );
}

//============================================================================
void StoryWidget::textSize(const QString &p)
{
    qreal pointSize = p.toFloat();
    if (p.toFloat() > 0) 
	{
        QTextCharFormat fmt;
        fmt.setFontPointSize( pointSize );
        mergeFormatOnWordOrSelection( fmt );
    }
}

//============================================================================
void StoryWidget::textColor()
{
    QColor col = QColorDialog::getColor( m_TextEdit->textColor(), this );
    if (!col.isValid())
	{
        return;
	}
    QTextCharFormat fmt;
    fmt.setForeground( col );
    mergeFormatOnWordOrSelection( fmt );
    colorChanged( col );
}

//============================================================================
void StoryWidget::leftJustify()
{
	ui.m_LeftJustifyButton->setChecked( true );
	ui.m_CenterTextButton->setChecked( false );
	ui.m_RightJustifyButton->setChecked( false );
	ui.m_FullLineButton->setChecked( false );

	m_TextEdit->setAlignment( Qt::AlignLeft | Qt::AlignAbsolute );
}

//============================================================================
void  StoryWidget::centerJustify()
{
	ui.m_LeftJustifyButton->setChecked( false );
	ui.m_CenterTextButton->setChecked( true );
	ui.m_RightJustifyButton->setChecked( false );
	ui.m_FullLineButton->setChecked( false );

	m_TextEdit->setAlignment( Qt::AlignHCenter );
}

//============================================================================
void  StoryWidget::rightJustify()
{
	ui.m_LeftJustifyButton->setChecked( false );
	ui.m_CenterTextButton->setChecked( false );
	ui.m_RightJustifyButton->setChecked( true );
	ui.m_FullLineButton->setChecked( false );

	m_TextEdit->setAlignment( Qt::AlignRight | Qt::AlignAbsolute );
}

//============================================================================
void StoryWidget::bothJustify()
{
	ui.m_LeftJustifyButton->setChecked( false );
	ui.m_CenterTextButton->setChecked( false );
	ui.m_RightJustifyButton->setChecked( false );
	ui.m_FullLineButton->setChecked( true );

	m_TextEdit->setAlignment( Qt::AlignJustify );
}

//============================================================================
void StoryWidget::insertPicture()
{
	QString fileName = QFileDialog::getOpenFileName(this, 
		tr("Select an image"), 
		".", 
		tr("Image Files (*.bmp *.jpg *jpeg *.gif *.png)")); 


	QImage image = QImageReader( fileName ).read();
	if(false == image.isNull())
	{
		//! remove the path and return just the file name
		std::string	strJustFileName;
		VxFileUtil::getJustFileName(	fileName.toUtf8().constData(),	// file name may be full or just file name
										strJustFileName );		// return file name
		if( strJustFileName.length() )
		{
            std::string storyboardImage = VxGetUserProfileDirectory();
			storyboardImage += strJustFileName;
			RCODE copyResult = 0;
			if( false == VxFileUtil::fileExists(strJustFileName.c_str()))
			{
				// if was not already in the story board directory then copy it there
				copyResult = VxFileUtil::copyFile(fileName.toUtf8().constData(), storyboardImage.c_str() );
			}

			if( 0 == copyResult )
			{
				QUrl Uri( strJustFileName.c_str() ); 

				QTextDocument * textDocument = m_TextEdit->document(); 
				textDocument->addResource( QTextDocument::ImageResource, Uri, QVariant ( image ) ); 
				QTextCursor cursor = m_TextEdit->textCursor(); 
				QTextImageFormat imageFormat; 
				imageFormat.setWidth( image.width() ); 
				imageFormat.setHeight( image.height() ); 
				imageFormat.setName( Uri.toString() ); 
				cursor.insertImage( imageFormat ); 
			}
			else
			{
				QMessageBox::warning(this, tr("Story Board"),
					tr("Could not copy image to story board directory"), QMessageBox::Ok);
			}
		}
	}
	else
	{
		QMessageBox::warning(this, tr("Story Board"),
			tr("Could not load the image"), QMessageBox::Ok);
	}
}

//============================================================================
void StoryWidget::editActionUndo()
{
	ui.m_UndoButton->actions()[0]->trigger();
	m_TextEdit->undo();
}

//============================================================================
void StoryWidget::editActionRedo()
{
	ui.m_RedoButton->actions()[0]->trigger();
	m_TextEdit->redo();
}

//============================================================================
void StoryWidget::editActionCut()
{
	ui.m_CutButton->actions()[0]->trigger();
	m_TextEdit->cut();
}

//============================================================================
void StoryWidget::editActionCopy()
{
	ui.m_CopyButton->actions()[0]->trigger();
	m_TextEdit->copy();
}

//============================================================================
void StoryWidget::editActionPaste()
{
	ui.m_PasteButton->actions()[0]->trigger();
	m_TextEdit->paste();
}

//============================================================================
//============================================================================
void StoryWidget::currentCharFormatChanged(const QTextCharFormat &format)
{
    fontChanged(format.font());
    colorChanged(format.foreground().color());
}

//============================================================================
void StoryWidget::cursorPositionChanged()
{
}

//============================================================================
void StoryWidget::clipboardDataChanged()
{
#ifndef QT_NO_CLIPBOARD
    if (const QMimeData *md = QApplication::clipboard()->mimeData())
        ui.m_PasteButton->setEnabled(md->hasText());
#endif
}

//============================================================================
void StoryWidget::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = m_TextEdit->textCursor();
    if (!cursor.hasSelection())
	{
        cursor.select(QTextCursor::WordUnderCursor);
	}

    cursor.mergeCharFormat(format);
    m_TextEdit->mergeCurrentCharFormat(format);
}

//============================================================================
void StoryWidget::fontChanged(const QFont &f)
{
    ui.m_FontComboBox->setCurrentIndex(ui.m_FontComboBox ->findText(QFontInfo(f).family()));
    ui.m_FontSizeComboBox->setCurrentIndex(ui.m_FontSizeComboBox->findText(QString::number(f.pointSize())));
    ui.m_BoldButton->setChecked(f.bold());
    ui.m_ItalicsButton->setChecked(f.italic());
    ui.m_UnderlineButton->setChecked(f.underline());
}

//============================================================================
void StoryWidget::colorChanged(const QColor &c)
{
    QPixmap pix(16, 16);
    pix.fill(c);
	//ui.m_TextColorButton->setIcon(pix);
}


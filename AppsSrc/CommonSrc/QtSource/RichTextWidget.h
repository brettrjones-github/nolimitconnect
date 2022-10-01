#pragma once
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
// http://www.nolimitconnect.org
//============================================================================

#include <QMap>
#include <QPointer>
#include <QTextEdit>

QT_FORWARD_DECLARE_CLASS(QAction)
QT_FORWARD_DECLARE_CLASS(QComboBox)
QT_FORWARD_DECLARE_CLASS(QFontComboBox)
QT_FORWARD_DECLARE_CLASS(QTextEdit)
QT_FORWARD_DECLARE_CLASS(QTextCharFormat)
QT_FORWARD_DECLARE_CLASS(QMenu)
QT_FORWARD_DECLARE_CLASS(QPrinter)

class RichTextWidget : public QTextEdit
{
    Q_OBJECT

public:
    RichTextWidget( QWidget*parent = 0, QString initialFile = "" );

	void						loadRichTextFile( const char * pFileName );
	bool						maybeSave();

private:
    bool						load( const QString &fileName );
    void						setCurrentFileName( const QString &fileName );

private slots:
    void						printPreview(QPrinter *);

private:
    void                        fileOpen( void );
    bool                        fileSave( void );
    bool                        fileSaveAs( void );

    void                        filePrint( void );
    void                        filePrintPreview( void );
    void                        filePrintPdf( void );

    //=== vars ===//
    QString                     m_strFileName;
};



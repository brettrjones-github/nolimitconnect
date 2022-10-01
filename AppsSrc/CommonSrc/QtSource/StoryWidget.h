#pragma once
//============================================================================
// Copyright (C) 2013 Brett R. Jones
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

#include <QMap>
#include <QPointer>
#include "ui_StoryWidget.h"

QT_FORWARD_DECLARE_CLASS(QAction)
QT_FORWARD_DECLARE_CLASS(QComboBox)
QT_FORWARD_DECLARE_CLASS(QFontComboBox)
QT_FORWARD_DECLARE_CLASS(QTextEdit)
QT_FORWARD_DECLARE_CLASS(QTextCharFormat)
QT_FORWARD_DECLARE_CLASS(QMenu)
QT_FORWARD_DECLARE_CLASS(QPrinter)

class StoryWidget : public QWidget
{
    Q_OBJECT

public:
    StoryWidget( QWidget*parent = 0, QString initialFile = "" );

	void						loadStoryBoardFile( const char * pFileName );
	bool						maybeSave();

signals:
	void						signalStoryBoardSavedModified();

private:
    void						setupFileActions();
    void						setupEditActions();
    void						setupTextActions();
    bool						load( const QString &fileName );
    void						setCurrentFileName( const QString &fileName );

private slots:
	bool						createLink( void );
	void						fileNew( void );
    void						fileOpen( void );
    bool						fileSave( void );
    bool						fileSaveAs( void );
    void						filePrint( void );
    void						filePrintPreview( void );
    void						filePrintPdf( void );

    void						textBold( void );
    void						textUnderline( void );
    void						textItalic( void );
    void						textFamily(const QString &f);
    void						textSize(const QString &p);
    void						textColor( void );

	void						leftJustify( void );
	void						centerJustify( void );
	void						rightJustify( void );
	void						bothJustify( void );

	void						insertPicture( void );
	void						editActionUndo( void );
	void						editActionRedo( void );
	void						editActionCut( void );
	void						editActionCopy( void );
	void						editActionPaste( void );

    void						currentCharFormatChanged(const QTextCharFormat &format);
    void						cursorPositionChanged( void );

    void						clipboardDataChanged( void );
    void						printPreview(QPrinter *);

private:
    void                        mergeFormatOnWordOrSelection(const QTextCharFormat &format);
    void                        fontChanged(const QFont &f);
    void                        colorChanged(const QColor &c);

    //=== vars ===//
    Ui::StoryWidgetClass        ui;
    QString                     m_strFileName;
    QTextEdit *                 m_TextEdit;
};



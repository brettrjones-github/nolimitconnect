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
// http://www.nolimitconnect.com
//============================================================================

#include <app_precompiled_hdr.h>
#include "FileItemInfo.h"
#include "FileItemWidget.h"

//============================================================================
FileItemWidget::FileItemWidget(QWidget *parent)
: QWidget(parent)
{
	ui.setupUi(this);
	connect( ui.FileIconButton, SIGNAL(clicked()), this, SLOT(slotFileIconButtonClicked()) );
}

//============================================================================
FileItemWidget::~FileItemWidget()
{
	FileItemInfo * fileItemInfo = (FileItemInfo *)QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
	delete fileItemInfo;
}

//============================================================================
void FileItemWidget::setXferStatus(QString strStatus)
{
	ui.FileStatus->setText(strStatus);
}

//============================================================================
void FileItemWidget::slotFileIconButtonClicked( void )
{
	emit signalFileItemClicked( this );
}
//
////============================================================================
//void FileItemWidget::slotThisItemClicked( void )
//{
//	emit signalFileItemClicked( this );
//}

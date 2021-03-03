//============================================================================
// Copyright (C) 2014 Brett R. Jones
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
#include "FriendListDelegate.h"
#include "FriendListEntryWidget.h"
#include "GuiParams.h"

#include <CoreLib/VxDefs.h>

#include <QPainter>
#include <QAbstractItemDelegate>

FriendListDelegate::FriendListDelegate(QObject *parent)
{
}
FriendListDelegate::~FriendListDelegate()
{
}

void FriendListDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
	QRect r = option.rect;

	uint64_t u64Widget = index.data(Qt::UserRole + 1).toULongLong();
	QWidget * poEntryWidget = ( QWidget * )u64Widget;
	if( poEntryWidget )
	{

		poEntryWidget->resize(r.size());
		poEntryWidget->move(r.topLeft());
		poEntryWidget->show();

	}
}

QSize FriendListDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
	return QSize(200, GuiParams::getButtonSize() + 4); // very dumb value
}


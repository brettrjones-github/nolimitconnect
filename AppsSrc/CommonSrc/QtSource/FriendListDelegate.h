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

#include "config_apps.h"

#include <QPainter>
#include <QAbstractItemDelegate>

class FriendListDelegate : public QAbstractItemDelegate
{
public:
	FriendListDelegate(QObject *parent = 0);
	virtual ~FriendListDelegate();

	//=== overrides ===//
	//! custom paint
	void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
	//! size of each item
	QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;

};
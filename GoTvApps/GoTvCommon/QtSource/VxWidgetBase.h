#pragma once
//============================================================================
// Copyright (C) 2015 Brett R. Jones
// Issued to MIT style license by Brett R. Jones in 2017
//
// You may use, copy, modify, merge, publish, distribute, sub-license, and/or sell this software
// provided this Copyright is not modified or removed and is included all copies or substantial portions of the Software
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// brett.jones@engineer.com
// http://www.gotvptop.net
//============================================================================

#include "config_gotvapps.h"
#include <CoreLib/ObjectCommon.h>
#include <QWidget>

class VxWidgetBase : public QWidget, public ObjectCommon
{
	Q_OBJECT
public:
    VxWidgetBase( QWidget * parent = 0, Qt::WindowFlags f = 0 );
	VxWidgetBase( const char * objName, QWidget * parent = 0, Qt::WindowFlags f = 0 );
	virtual ~VxWidgetBase();

signals:
	void						clicked();

protected:
	virtual void				mousePressEvent( QMouseEvent * event );
};



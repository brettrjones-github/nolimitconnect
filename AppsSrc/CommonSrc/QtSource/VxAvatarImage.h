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
// bjones.engineer@gmail.com
// http://www.nolimitconnect.com
//============================================================================

#include "config_apps.h"

#include <QLabel>

class QTimer;

class VxAvatarImage : public QLabel 
{
	Q_OBJECT
public:
	VxAvatarImage(QWidget *parent=0, Qt::WindowFlags f= Qt::Widget);
	VxAvatarImage(const QString &text, QWidget *parent=0, Qt::WindowFlags f= Qt::Widget);
	virtual ~VxAvatarImage();

	QSize						sizeHint() const;
	void						setImage( const char * resourceUrl );

signals:
	void						clicked();

protected:
	virtual void				mousePressEvent(QMouseEvent * event);

	void						initQAvatarImage( void );
};


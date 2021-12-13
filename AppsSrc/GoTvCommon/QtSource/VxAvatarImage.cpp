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

#include <app_precompiled_hdr.h>
#include "VxAvatarImage.h"

#include <CoreLib/VxDebug.h>

#include <QTimer>
#include <QPixmap>

//============================================================================
VxAvatarImage::VxAvatarImage(QWidget *parent, Qt::WindowFlags f) 
: QLabel(parent,f) 
{
	initQAvatarImage();
}

//============================================================================
VxAvatarImage::VxAvatarImage(const QString &text, QWidget *parent, Qt::WindowFlags f) 
: QLabel(text,parent,f) 
{
    setObjectName( "VxAvatarImage" );
	initQAvatarImage();
}

//============================================================================
VxAvatarImage::~VxAvatarImage()
{
}

//============================================================================
void VxAvatarImage::initQAvatarImage( void )
{
	setImage( ":/AppRes/Resources/avatar.svg" );
}

//============================================================================
void VxAvatarImage::setImage( const char * resourceUrl )
{
	setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
	QPixmap picBitmap( resourceUrl ); 
	setPixmap( picBitmap );
}

//============================================================================
QSize VxAvatarImage::sizeHint() const
{
	return QSize(32,32);
}

//============================================================================
void VxAvatarImage::mousePressEvent(QMouseEvent * event)
{
	QWidget::mousePressEvent(event);
	emit clicked();
}

////============================================================================
//void VxAvatarImage::updateImage( void )
//{
//	QSize screenSize( width(), height() );
//	QImage picBitmap( ":/AppRes/Resources/spinner-clockwise.svg" ); 
//	m_ProgressPixmap = picBitmap.scaled(screenSize, Qt::KeepAspectRatio);
//}

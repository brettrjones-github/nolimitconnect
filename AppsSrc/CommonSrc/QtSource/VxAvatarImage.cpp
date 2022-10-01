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
// http://www.nolimitconnect.org
//============================================================================


#include "VxAvatarImage.h"

#include <CoreLib/VxDebug.h>

#include <QTimer>
#include <QPixmap>
#include <QImage>

//============================================================================
VxAvatarImage::VxAvatarImage(QWidget*parent, Qt::WindowFlags f) 
: QLabel(parent,f) 
{
	initQAvatarImage();
}

//============================================================================
VxAvatarImage::VxAvatarImage(const QString &text, QWidget*parent, Qt::WindowFlags f) 
: QLabel(text,parent,f) 
{
    setObjectName( "VxAvatarImage" );
	initQAvatarImage();
}

//============================================================================
void VxAvatarImage::initQAvatarImage( void )
{
	setImage( ":/AppRes/Resources/avatar.svg" );
}

//============================================================================
void VxAvatarImage::setImage( const char * resourceUrl )
{
	if( !m_HasThumbImage )
	{
		setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
		QPixmap picBitmap( resourceUrl );
		setPixmap( picBitmap );
	}
}

//============================================================================
void VxAvatarImage::setImage( QImage& avatarImage )
{
	if( !avatarImage.isNull() )
	{
		QPixmap avatarPixmap = QPixmap::fromImage( avatarImage ).scaled(geometry().size());
		if( !avatarPixmap.isNull() )
		{
			m_HasThumbImage = true;
			setPixmap( avatarPixmap );
		}
	}
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

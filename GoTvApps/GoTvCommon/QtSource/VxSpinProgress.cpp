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
#include "VxSpinProgress.h"

#include <CoreLib/VxDebug.h>

#include <QTimer>
#include <QPixmap>

//============================================================================
VxSpinProgress::VxSpinProgress(QWidget *parent, Qt::WindowFlags f) 
: QLabel(parent,f) 
, m_AnimationTimer( new QTimer(this) )
, m_Angle( 0 )
, m_PercentText("")
{
	initQSpinProgress();
}

//============================================================================
VxSpinProgress::VxSpinProgress(const QString &text, QWidget *parent, Qt::WindowFlags f) 
: QLabel(text,parent,f) 
, m_AnimationTimer( new QTimer(this) )
, m_Angle( 0 )
, m_PercentText("")
{
	initQSpinProgress();
}

//============================================================================
void VxSpinProgress::initQSpinProgress( void )
{
    setObjectName( "VxSpinProgress" );

	setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
	m_AnimationTimer->setInterval( 1000 );
	connect( m_AnimationTimer, SIGNAL(timeout()), this, SLOT(slotAnimationTickTock()) );
}

//============================================================================
QSize VxSpinProgress::sizeHint() const
{
	return QSize(32,32);
}

//============================================================================
void VxSpinProgress::setValue( int progressPercent )
{
	m_PercentText = QString("%1").arg(progressPercent);
	//setText( m_PercentText );
	if( progressPercent >= 100 )
	{
		m_AnimationTimer->stop();
	}
}

//============================================================================
void VxSpinProgress::setImage( int width, int height, QString resourceUrl )
{
	QSize screenSize( width, height );
	this->setFixedSize( width, height );
	QImage picBitmap( resourceUrl ); 
	m_ProgressPixmap = picBitmap.scaled(screenSize, Qt::KeepAspectRatio);
	m_AnimationTimer->start();
}

//============================================================================
void VxSpinProgress::mousePressEvent(QMouseEvent * event)
{
	QWidget::mousePressEvent(event);
	emit clicked();
}

//============================================================================
void VxSpinProgress::showEvent(QShowEvent * showEvent)
{
	m_AnimationTimer->start( 40 );
}

//============================================================================
void VxSpinProgress::hideEvent(QHideEvent * hideEvent)
{
	m_AnimationTimer->stop();
}

//============================================================================
void VxSpinProgress::slotAnimationTickTock( void )
{
	if( m_ProgressPixmap.isNull() 
		|| ( ( this->width() != m_ProgressPixmap.width() ) && ( this->height() != m_ProgressPixmap.height() ) ) )
	{
		updateImage();
	}

	if( m_ProgressPixmap.isNull() 
		|| ( ( this->width() != m_ProgressPixmap.width() ) && ( this->height() != m_ProgressPixmap.height() ) ) )
	{
		return;
	}

	m_Angle = ( m_Angle + 6 ) % 360;

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
	QTransform rm;
#else
	QMatrix rm;
#endif // QT_VERSION >= QT_VERSION_CHECK(6,0,0)
	rm.rotate( m_Angle );
	QPixmap pixmap = QPixmap::fromImage( m_ProgressPixmap ).transformed(rm);
	setPixmap( pixmap );
	repaint();
}

//============================================================================
void VxSpinProgress::updateImage( void )
{
	QSize screenSize( width(), height() );
	QImage picBitmap( ":/AppRes/Resources/spinner-clockwise.svg" ); 
	m_ProgressPixmap = picBitmap.scaled(screenSize, Qt::KeepAspectRatio);
}

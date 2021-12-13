
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
#include "VxShredderButton.h"
#include "AppCommon.h"

#include <CoreLib/VxDebug.h>

#include <QTimer>
#include <QDebug>
#include <QMouseEvent>

//============================================================================
VxShredderButton::VxShredderButton( QWidget *parent ) 
: QPushButton( parent ) 
, m_MyApp( GetAppInstance() )
, m_ConfirmRequired( true )
, m_ShreadFileName("")
, m_ESndDefPressed( eSndDefPaperShredder )
, m_IsPressed( false )
, m_BlinkState( 0 )
, m_BlinkTimer( new QTimer( this ) )
, m_BlinkRateMs( 300 )
{
    setObjectName( "VxShredderButton" );
	initQShreaderButton();
}


//============================================================================
VxShredderButton::~VxShredderButton()
{
	m_BlinkTimer->stop();
	disconnect( m_BlinkTimer, SIGNAL(timeout()), this, SLOT(slotBlinkTimeout()) );
}

//============================================================================
void VxShredderButton::initQShreaderButton( void )
{
	QIcon icon1( ":/AppRes/Resources/button_shreader_disabled.png" );
	m_DisabledIcon = icon1;
	QIcon icon2( ":/AppRes/Resources/button_shreader_normal.png" );
	m_NormalOnIcon = icon2;
	QIcon icon3( ":/AppRes/Resources/button_shreader_pressed.png" );
	m_PressedOnIcon = icon3;
	QIcon icon4( ":/AppRes/Resources/button_shreader_red_pressed.png" );
	m_Blink1PressedIcon = icon4;
	QIcon icon5( ":/AppRes/Resources/button_shreader_yellow_pressed.png" );
	m_Blink2PressedIcon = icon5;
	QIcon icon6( ":/AppRes/Resources/button_shreader_red_normal.png" );
	m_Blink1NormalIcon = icon6;
	QIcon icon7( ":/AppRes/Resources/button_shreader_yellow_normal.png" );
	m_Blink2NormalIcon = icon7;
	updateCurIcon();

	connect( m_BlinkTimer, SIGNAL(timeout()), this, SLOT(slotBlinkTimeout()) );
}

//============================================================================
void VxShredderButton::setEnabled( bool enabled )
{
	QPushButton::setEnabled( enabled );
	updateCurIcon();
}
//============================================================================
void VxShredderButton::updateCurIcon( void )
{
	if( isEnabled() )
	{
		if( m_IsPressed )
		{
			setIcon( m_PressedOnIcon );
		}
		else
		{
			setIcon( m_NormalOnIcon );
		}
	}
	else
	{
		setIcon( m_DisabledIcon );
	}
}

//============================================================================
void VxShredderButton::mousePressEvent( QMouseEvent * event )
{
	if( false == m_ShreadFileName.isEmpty() )
	{
		if( m_ConfirmRequired )
		{
			QString title = QObject::tr("Confirm Shred Asset File");
			QString msg = QObject::tr("Are you sure you want to shred the file\n %1\nThe file will overwritten with random data then deleted.").arg(m_ShreadFileName);

			if( QMessageBox::Yes != QMessageBox::question( this, title, msg, QMessageBox::Yes | QMessageBox::No ) )
			{
				return;
			}
		}
	}

	m_IsPressed = true;
	playSndAndAnimation();
	emit signalShreddingFile( m_ShreadFileName );
}

//============================================================================
void VxShredderButton::playSndAndAnimation( int blinkCountdown )
{
	m_BlinkCountdown = blinkCountdown;
	setNotifyEnabled( true );
	m_MyApp.playSound( m_ESndDefPressed );
}

//============================================================================
void VxShredderButton::stopAnimation( void )
{
	setNotifyEnabled( false );
}

//============================================================================
void VxShredderButton::setNotifyEnabled( bool setNotifyEnabled )
{
	if( setNotifyEnabled )
	{
		if( false == isEnabled() )
		{
			return;
		}
		
		qDebug() << "VxShredderButton::setNotifyEnabled ";
		m_BlinkState = 0;
		setIcon( m_IsPressed ? m_Blink1PressedIcon : m_Blink1NormalIcon );
		m_BlinkTimer->start( m_BlinkRateMs );
	}
	else
	{
		qDebug() << "VxShredderButton::disableBlink ";
		m_BlinkCountdown = -1;
		m_BlinkTimer->stop();
		updateCurIcon();
	}
}

//============================================================================
void VxShredderButton::slotBlinkTimeout( void )
{
	qDebug() << "VxShredderButton::slotBlinkTimeout ";
	m_BlinkCountdown--;
	if( 0 >= m_BlinkCountdown )
	{
		setNotifyEnabled( false );
		emit animationCompleted();
		shreadCurFile();
		return;
	}

	m_BlinkState = m_BlinkState ? 0 : 1;
	if( m_BlinkState )
	{
		setIcon( m_IsPressed ? m_Blink2PressedIcon : m_Blink2NormalIcon );
	}
	else
	{
		setIcon( m_IsPressed ? m_Blink1PressedIcon : m_Blink1NormalIcon );
	}
}

//============================================================================
void VxShredderButton::mouseReleaseEvent( QMouseEvent * event )
{
	m_IsPressed = false;
	stopAnimation();
	qDebug() << "VxShredderButton::mouseReleaseEvent " << event->pos();
	QPushButton::mouseReleaseEvent( event );
	updateCurIcon();
}

//============================================================================
void VxShredderButton::setShredFileAndShred( QString fileName )
{
	setShredFile( fileName );
	shreadCurFile();
}

//============================================================================
void VxShredderButton::shreadCurFile( void )
{
	if( !m_ShreadFileName.isEmpty() )
	{
		emit signalShreddingFile( m_ShreadFileName );
	}
}

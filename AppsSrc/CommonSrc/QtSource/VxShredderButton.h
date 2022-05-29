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
// http://www.nolimitconnect.org
//============================================================================

#include "SoundDefs.h"

#include <QPushButton>
#include <QString>


class AppCommon;
class QTimer;

class VxShredderButton : public QPushButton
{
	Q_OBJECT
public:
	VxShredderButton(QWidget *parent=0);
	virtual ~VxShredderButton();

	void						setEnabled( bool enabled );
	void						setConfirmShredRequried( bool confirmRequried )		{ m_ConfirmRequired = confirmRequried; }
	bool						getConfirmShredRequried( void )						{ return m_ConfirmRequired; }

	void						setBlinkRateMs( int blinkRateMs )					{ m_BlinkRateMs = blinkRateMs; }
	int							getBlinkRateMs( void )								{ return m_BlinkRateMs; }

	void						setPressedSound( ESndDef sndDef )					{ m_ESndDefPressed = sndDef; }
	void						setShredFile( QString fileName )					{ m_ShreadFileName = fileName; }
	QString						getShredFile( void )								{ return m_ShreadFileName; }
	void						setShredFileAndShred( QString fileName );
	void						shreadCurFile( void );

	void						playSndAndAnimation( int blinkCountdown = 3 );
	void						stopAnimation( void );

signals:
	void						signalShreddingFile( QString shredFileName );
	void						animationCompleted( void );

protected slots:
	void						slotBlinkTimeout( void );

protected:
	void						initQShreaderButton( void );
	void						updateCurIcon( void );

	void						setNotifyEnabled( bool setNotifyEnabled );

	virtual void				mousePressEvent( QMouseEvent * event );
	virtual void				mouseReleaseEvent( QMouseEvent * event );


	//=== vars ===//
	AppCommon&					m_MyApp;

	bool						m_ConfirmRequired;
	QString						m_ShreadFileName;
	ESndDef						m_ESndDefPressed;
	QIcon						m_DisabledIcon;
	QIcon						m_NormalOnIcon;
	QIcon						m_PressedOnIcon;
	QIcon						m_Blink1PressedIcon;
	QIcon						m_Blink2PressedIcon;
	QIcon						m_Blink1NormalIcon;
	QIcon						m_Blink2NormalIcon;
	bool						m_IsPressed;
	int							m_BlinkState;
	QTimer *					m_BlinkTimer;
	int							m_BlinkRateMs;
	int							m_BlinkCountdown;
};


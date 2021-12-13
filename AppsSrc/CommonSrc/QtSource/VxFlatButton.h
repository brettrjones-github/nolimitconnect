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

#include "MyIcons.h"

#include <QPushButton>

class AppCommon;
class MyIcons;

class VxFlatButton : public QPushButton
{
	Q_OBJECT
public:
	VxFlatButton(QWidget *parent=0);
	virtual ~VxFlatButton() = default;

	AppCommon&					getMyApp( void )						{ return m_MyApp; }
	virtual void				setMyIcon( EMyIcons myIcon )			{ m_MyIcon = myIcon; }
	MyIcons&					getMyIcons( void );

protected:
	virtual void                paintEvent( QPaintEvent* ev ) override;

	//=== vars ===//
	AppCommon&					m_MyApp;
	EMyIcons					m_MyIcon;
	QPixmap						m_IconImage;
	QColor						m_LastIconColor;
	QSize						m_LastIconSize;
};


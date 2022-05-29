#pragma once
//============================================================================
// Copyright (C) 2013 Brett R. Jones
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

#include <QBitmap>
#include "ui_TodGameWidget.h"

class VxGUID;

class TodGameWidget : public QWidget
{
	Q_OBJECT

public:
	Ui::TodGameWidget ui;

	TodGameWidget( QWidget *parent = 0 );

	VidWidget *		getVidWidget( void );

	void			playVideoFrame( VxGUID& feedId, unsigned char * pu8Jpg, unsigned long u32JpgLen, int motion0To100000 );
	void			enableGameButtons( bool bEnable );

signals:
	void			truthButtonClicked();
	void			dareButtonClicked();

};

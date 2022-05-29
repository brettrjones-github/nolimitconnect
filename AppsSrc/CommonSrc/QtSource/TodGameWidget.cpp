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


#include "TodGameWidget.h"

//============================================================================
TodGameWidget::TodGameWidget(QWidget *parent)
: QWidget(parent)
{
	ui.setupUi(this);

	connect( ui.m_DareButton, SIGNAL(clicked()), this, SIGNAL(dareButtonClicked()));
	connect( ui.m_TruthButton, SIGNAL(clicked()), this, SIGNAL(truthButtonClicked()));
}

//============================================================================
void TodGameWidget::playVideoFrame( VxGUID& feedId, unsigned char * pu8Jpg, unsigned long u32JpgLen, int motion0To100000 )
{
	ui.TruthOrDareWidget->playVideoFrame( feedId, pu8Jpg, u32JpgLen, motion0To100000 );
}

//============================================================================
void TodGameWidget::enableGameButtons( bool bEnable )
{
	ui.m_TruthButton->setVisible( bEnable );
	ui.m_DareButton->setVisible( bEnable );
}

//============================================================================
VidWidget * TodGameWidget::getVidWidget( void )
{
	return ui.TruthOrDareWidget;
}

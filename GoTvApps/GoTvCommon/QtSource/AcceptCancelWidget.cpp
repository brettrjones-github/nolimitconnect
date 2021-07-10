//============================================================================
// Copyright (C) 2017 Brett R. Jones
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

#include "app_precompiled_hdr.h"
#include "AcceptCancelWidget.h"
#include "VxAppTheme.h"


//============================================================================
AcceptCancelWidget::AcceptCancelWidget( QWidget *parent )
: QFrame( parent )
{
	ui.setupUi( this );
	ui.m_AcceptButton->setIconOverrideColor( QColor( COLOR_GREEN ) );
	ui.m_AcceptButton->setIcon( eMyIconCheckMark );
	ui.m_CancelButton->setIconOverrideColor( QColor( COLOR_RED ) );
	ui.m_CancelButton->setIcon( eMyIconRedX );
	connect( ui.m_AcceptButton, SIGNAL( clicked() ), this, SLOT( slotAccepted() ) );
	connect( ui.m_CancelButton, SIGNAL( clicked() ), this, SLOT( slotCanceled() ) );
}

//============================================================================
void AcceptCancelWidget::slotAccepted( void )
{
	emit signalAccepted();
}

//============================================================================
void AcceptCancelWidget::slotCanceled( void )
{
	emit signalCanceled();
}

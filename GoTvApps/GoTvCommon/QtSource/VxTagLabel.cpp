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
// http://www.gotvptop.net
//============================================================================

#include "VxTagLabel.h"

#include <CoreLib/VxDebug.h>

//============================================================================
VxTagLabel::VxTagLabel(QWidget *parent, Qt::WindowFlags f) 
: VxLabel(parent,f) 
, m_AssetInfo( 0 )
{
}

//============================================================================
VxTagLabel::VxTagLabel(const QString &text, QWidget *parent, Qt::WindowFlags f) 
: VxLabel(text,parent,f) 
, m_AssetInfo( 0 )
{
	setupQTagLabel();
}

//============================================================================
VxTagLabel::~VxTagLabel()
{
	setupQTagLabel();
}

//============================================================================
void VxTagLabel::setupQTagLabel( void )
{
	connect( this, SIGNAL(clicked()), this, SLOT(slotOnClicked()) );
}

//============================================================================
void VxTagLabel::slotOnClicked( void )
{
	if( 0 == m_AssetInfo )
	{
		return;
	}


}
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


#include "config_apps.h"

#include "VxComboBox.h"
#include "AppCommon.h"
#include "VxAppTheme.h"

#include <QPainter>
#include <QLineEdit>
#include <QPushButton>

//============================================================================
VxComboBox::VxComboBox(QWidget *parent) 
: QComboBox(parent) 
{
    setObjectName( "VxComboBox" );
    //setEditable( true );
    /*
    setStyleSheet( "QComboBox::down-arrow{\
    image: :/AppRes/Resources/arrow-thick-thin-down.svg;\
    width: 24px;\
    height: 24px; }" );*/

}

/*
//============================================================================
void VxComboBox::paintEvent( QPaintEvent *ev )
{
    VxAppTheme& appTheme = GetAppInstance().getAppTheme();
    QPalette pal = appTheme.getBasePalette();
    pal.setColor( QPalette::Text, appTheme.getColor( eButtonTextNormal ) );
    setPalette( pal );
    QLineEdit * lineEditor = this->lineEdit();
    if( lineEditor )
    {
        lineEditor->setPalette( pal );
        lineEditor->setReadOnly( true );
   }

    QComboBox::paintEvent( ev );
 }*/

//============================================================================
void VxComboBox::showEvent( QShowEvent *ev )
{
    QComboBox::showEvent( ev );
}

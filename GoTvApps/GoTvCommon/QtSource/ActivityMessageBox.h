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
// brett.jones@engineer.com
// http://www.gotvptop.net
//============================================================================

#include "ActivityBase.h"

#include <PktLib/VxCommon.h>

#include <QDialog>
#include "ui_ActivityMessageBox.h"

class ActivityMessageBox : public ActivityBase
{
	Q_OBJECT
public:
	ActivityMessageBox( AppCommon& app, QWidget * parent = NULL );
	ActivityMessageBox( AppCommon& app, QWidget * parent, int infoLevel, const char * msgFormat, ... );
	ActivityMessageBox( AppCommon& app, QWidget * parent, int infoLevel, QString msg );
	virtual ~ActivityMessageBox();

	void						setTitleText( QString titleText );
	void						setBodyText( QString bodyText );

	bool						wasOkButtonClicked( void )						{ return m_OkButtonClicked; }
	void						showCancelButton( bool showButton );

public slots:
	void						onOkButClick( void );
	void						onCancelButClick( void ); 

protected:
	//=== vars ===//
	Ui::MessageBoxDialogClass	ui;
	bool						m_OkButtonClicked;

};
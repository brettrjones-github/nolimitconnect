#pragma once
//============================================================================
// Copyright (C) 2017 Brett R. Jones
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

class VxWidgetBase;

class AppletLaunchPage : public ActivityBase
{
	Q_OBJECT
public:
	AppletLaunchPage( AppCommon& app, QWidget * parent );
	virtual ~AppletLaunchPage() = default;


protected slots:
	void						slotPowerButtonClicked( void );

protected:
	virtual void				showEvent( QShowEvent * );
	//virtual void				hideEvent( QHideEvent * );
	void						resizeEvent( QResizeEvent * );

private:
	void						setupAppletLaunchPage( void );

	bool						m_IsInitialized;
	QVector<VxWidgetBase*>		m_AppletList;
};



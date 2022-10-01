#pragma once
//============================================================================
// Copyright (C) 2010 Brett R. Jones
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

#include "ActivityBase.h"

class MessengerPage : public ActivityBase
{
	Q_OBJECT
public:
	MessengerPage( AppCommon& app, QWidget* parent );
	virtual ~MessengerPage() override = default;

signals:
	void						signalMessengerPageChanged( bool isVisible, bool isFullScreen );

protected slots :
    virtual void				slotBackButtonClicked( void ) override;

protected:
    virtual void				showEvent( QShowEvent* ) override;
    virtual void				hideEvent( QHideEvent* ) override;

	bool						m_IsVisible;
	bool						m_IsFullSize;
};



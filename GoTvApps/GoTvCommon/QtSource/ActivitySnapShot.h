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
// http://www.nolimitconnect.com
//============================================================================

#include "ActivityBase.h"

#include <QDialog>
#include "ui_ActivitySnapShot.h"

#include <CoreLib/VxDefs.h>

class IVxVidCap;

class ActivitySnapShot : public ActivityBase
{
	Q_OBJECT
public:
	ActivitySnapShot( AppCommon& app, QWidget * parent = NULL );
	virtual ~ActivitySnapShot() override = default;

    // overrides required for dialogs with there own title bar and bottom bar widgets
    virtual TitleBarWidget *	getTitleBarWidget( void ) override { return ui.m_TitleBarWidget; }
    virtual BottomBarWidget *	getBottomBarWidget( void ) override { return ui.m_BottomBarWidget; }

signals:
	void						signalJpgSnapshot(uint8_t* pu8JpgData,uint32_t u32DataLen,int iWidth,int iHeight );

public slots:
    void						onSnapShotButClick( void );
    void						onCancelButClick( void );

protected:
	Ui::snapshotDialog			ui;
	IVxVidCap *					m_VidCap;
    bool 					    m_CameraSourceAvail = false;
    QTimer *                    m_CloseDlgTimer = nullptr;

	uint8_t *					m_pu8BitmapData = nullptr;	// snapshot bitmap
	uint32_t					m_u32BitmapLen = 0;		// bitmap length
};

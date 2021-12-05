#pragma once
//============================================================================
// Copyright (C) 2020 Brett R. Jones
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

#include "AppletBase.h"
#include "ui_AppletSnapshot.h"

#include <CoreLib/MediaCallbackInterface.h>

class ThumbnailViewWidget;
class AssetMgr;
class IVxVidCap;

class AppletSnapshot : public AppletBase, public MediaCallbackInterface
{
	Q_OBJECT
public:
	AppletSnapshot( AppCommon& app, QWidget * parent = NULL );
	virtual ~AppletSnapshot() override;

signals:
    void						signalSnapshotImage( QImage snapshotImage );

public slots:
    void						onSnapShotButClick( void );
    void						onCancelButClick( void );
    void                        onDoneButClick( void );

protected:
    virtual void				callbackVideoJpgSmall( void * userData, VxGUID& vidFeedId, uint8_t * jpgData, uint32_t jpgDataLen, int motion0to100000 ) override;
    virtual void                onCloseEvent( void ) override;


    Ui::AppletSnapshotUi		ui;
    bool 					    m_CameraSourceAvail = false;
    QTimer *                    m_CloseDlgTimer = nullptr;
    bool 					    m_SnapShotPending = false;
    QImage	                    m_ImageBitmap;
};

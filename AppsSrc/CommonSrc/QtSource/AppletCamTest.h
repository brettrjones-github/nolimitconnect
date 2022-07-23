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
// http://www.nolimitconnect.org
//============================================================================

#include "AppletBase.h"
#include "ui_AppletCamTest.h"

#include <CoreLib/MediaCallbackInterface.h>

class ThumbnailViewWidget;
class AssetMgr;
class IVxVidCap;

class AppletCamTest : public AppletBase, public MediaCallbackInterface
{
	Q_OBJECT
public:
	AppletCamTest( AppCommon& app, QWidget * parent = NULL );
	virtual ~AppletCamTest() override;

    void						webCamSourceOffline();
    void                        startCamFeed( void );
    void                        stopCamFeed( void );
    void                        setIsCamFeedStarted( bool isStarted )   { m_CamFeedStarted = isStarted; }
    bool                        isCamFeedStarted( void )                { return m_CamFeedStarted; }
    bool                        isMyself( void )                        { return m_IsMyself; }

signals:
    void						signalSnapshotImage( QImage snapshotImage );

protected slots:
    void						slotToGuiRxedOfferReply( GuiOfferSession * offerSession );
    void						slotToGuiSessionEnded( GuiOfferSession * offerSession );
    void						slotToGuiContactOffline( VxNetIdent * hisIdent );

    void						slotMicToSpeakerStateChanged( int checkBoxState );

    void						onCancelButClick( void );

protected:
    virtual void				showEvent( QShowEvent * ev ) override;
    virtual void				hideEvent( QHideEvent * ev ) override;
    virtual void				closeEvent( QCloseEvent * ev ) override;

    void						setupCamFeed( VxNetIdent* feedNetIdent );
    void						resizeBitmapToFitScreen( QLabel * VideoScreen, QImage& oPicBitmap );
    void						setMuteSpeakerVisibility( bool visible );
    void						setMuteMicrophoneVisibility( bool visible );
    void						setCameraButtonVisibility( bool visible );

    //=== vars ===//
    Ui::AppletCamTestUi	    ui;
    bool						m_IsMyself{ false };
    bool 					    m_CameraSourceAvail{ false };
    QTimer *                    m_CloseAppletTimer{ nullptr };
    QImage	                    m_ImageBitmap;
    VxNetIdent*                 m_CamFeedIdent{ nullptr };
    VxGUID                      m_CamFeedId;
    bool                        m_CamFeedStarted{ false };
};

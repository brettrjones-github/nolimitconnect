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
#include "ui_AppletCamSettings.h"

#include <CoreLib/MediaCallbackInterface.h>

class ThumbnailViewWidget;
class AssetMgr;
class IVxVidCap;

class AppletCamSettings : public AppletBase, public MediaCallbackInterface
{
	Q_OBJECT
public:
	AppletCamSettings( AppCommon& app, QWidget * parent = NULL );
	virtual ~AppletCamSettings() override;

    void						playVideoFrame( VxGUID& feedId, unsigned char * pu8Jpg, unsigned long u32JpgLen, int motion0To100000 );

    void						webCamSourceOffline();
    void                        startCamFeed( void );
    void                        stopCamFeed( void );
    void                        setIsCamFeedStarted( bool isStarted )   { m_CamFeedStarted = isStarted; }
    bool                        isCamFeedStarted( void )                { return m_CamFeedStarted; }
    bool                        isMyself( void )                        { return m_IsMyself; }

signals:
    void						signalPlayVideoFrame( QPixmap oPicBitmap, int iRotate );
    void						signalPlayAudio( unsigned short * pu16PcmData, unsigned short u16PcmDataLen );

protected slots:
    //void						slotMuteMicButtonClicked( bool muteMicrophone );
    //void						slotMuteSpeakerButtonClicked( bool muteSpeaker );
    void						onAboutToDestroyDialog( void ); 
    void						slotToGuiRxedOfferReply( GuiOfferSession * offerSession );
    void						slotToGuiSessionEnded( GuiOfferSession * offerSession );
    void						slotToGuiContactOffline( VxNetIdent * hisIdent );

protected:
    virtual void				showEvent( QShowEvent * ev ) override;
    virtual void				hideEvent( QHideEvent * ev ) override;
    virtual void				closeEvent( QCloseEvent * ev ) override;

    void						setupCamFeed( VxNetIdent* feedNetIdent );
    void						resizeBitmapToFitScreen( QLabel * VideoScreen, QImage& oPicBitmap );
    void						setMuteSpeakerVisibility( bool visible );
    void						setMuteMicrophoneVisibility( bool visible );
    void						setCameraButtonVisibility( bool visible );

signals:
    void						signalSnapshotImage( QImage snapshotImage );

public slots:
    //void						onSnapShotButClick( void );
    void						onCancelButClick( void );

protected:
    virtual void				callbackVideoJpgSmall( void * userData, VxGUID& vidFeedId, uint8_t * jpgData, uint32_t jpgDataLen, int motion0to100000 ) override;
    virtual void				toGuiClientPlayVideoFrame( void * userData, VxGUID& onlineId, uint8_t * pu8Jpg, uint32_t u32JpgDataLen, int motion0To100000 ) override;
    virtual void                onCloseEvent( void ) override;

    //=== vars ===//
    Ui::AppletCamSettingsUi	    ui;
    bool						m_IsMyself{ false };
    bool 					    m_CameraSourceAvail{ false };
    QTimer *                    m_CloseAppletTimer{ nullptr };
    QImage	                    m_ImageBitmap;
    VxNetIdent*                 m_CamFeedIdent{ nullptr };
    VxGUID                      m_CamFeedId;
    bool                        m_CamFeedStarted{ false };
};

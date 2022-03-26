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

#include "AppletAssetPlayerBase.h"
#include "ui_AppletCamClipPlayer.h"

#include <CoreLib/MediaCallbackInterface.h>

class ThumbnailViewWidget;
class AssetMgr;
class IVxVidCap;

class AppletCamClipPlayer : public AppletAssetPlayerBase, public MediaCallbackInterface
{
	Q_OBJECT
public:
	AppletCamClipPlayer( AppCommon& app, QWidget * parent = NULL, VxGUID assetId = VxGUID::nullVxGUID() );
	virtual ~AppletCamClipPlayer() override;

	virtual void				setAssetInfo( AssetBaseInfo& assetInfo ) override;
	virtual void				setAssetInfo( AssetInfo& assetInfo ) override;
	virtual void				updateAssetInfo( void );
    virtual void				onActivityStop( void ) override;

    virtual void				toGuiClientPlayVideoFrame( VxGUID& onlineId, uint8_t* pu8Jpg, uint32_t u32JpgDataLen, int motion0To100000 ) override;
    virtual int				    toGuiClientPlayVideoFrame( VxGUID& onlineId, uint8_t* picBuf, uint32_t picBufLen, int picWidth, int picHeight ) override;

    virtual void				showSendFail( bool show, bool permissionErr = false ) override;
    virtual void				showResendButton( bool show ) override;
    virtual void				showShredder( bool show ) override;
    virtual void				showXferProgress( bool show ) override;
    virtual void				setXferProgress( int sendProgress ) override;

signals:
	void						signalPlayProgress( int pos );
	void						signalPlayEnd( void );

protected slots:
	void						slotPlayButtonClicked( void );
    void						slotShredAsset( void ) override;
	void						slotSliderPressed( void );
	void						slotSliderReleased( void );

	void						slotPlayProgress( int pos0to100000 );
	void						slotPlayEnd( void );

protected:
	void						toGuiClientAssetAction( EAssetAction assetAction, VxGUID& assetId, int pos0to100000 ) override;
	void						initAppletCamClipPlayer( void );

    void						showEvent( QShowEvent* ev ) override;
    void						resizeEvent( QResizeEvent* ev ) override;

	void						setReadyForCallbacks( bool isReady );
	void						updateGuiPlayControls( bool isPlaying );
	void						startMediaPlay( int startPos );
	void						stopMediaIfPlaying( void );

	//=== vars ===//
	bool						m_ActivityCallbacksEnabled{ false };
	bool						m_IsPlaying{ false };
	bool						m_SliderIsPressed{ false };

    //=== vars ===//
    Ui::AppletCamClipPlayerUi	ui;
};

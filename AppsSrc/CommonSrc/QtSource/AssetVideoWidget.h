#pragma once
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

#include "AssetBaseWidget.h"
#include "ui_AssetVideoWidget.h"

class AssetVideoWidget : public AssetBaseWidget
{
	Q_OBJECT

public:
	AssetVideoWidget( QWidget * parent = 0 );
	AssetVideoWidget( AppCommon& appCommon, QWidget * parent = 0 );
	virtual ~AssetVideoWidget() = default;

    virtual void				setAssetInfo( AssetBaseInfo& assetInfo ) override;
    virtual void				onActivityStop( void ) override;

    virtual void				toGuiClientPlayVideoFrame( VxGUID& onlineId, uint8_t * pu8Jpg, uint32_t u32JpgDataLen, int motion0To100000 ) override;
    virtual int				    toGuiClientPlayVideoFrame( VxGUID& onlineId, uint8_t * picBuf, uint32_t picBufLen, int picWidth, int picHeight ) override;

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
	virtual void				toGuiClientAssetAction( EAssetAction assetAction, VxGUID& assetId, int pos ) override;
	void						initAssetVideoWidget( void );

    void						showEvent( QShowEvent * ev ) override;
    void						resizeEvent( QResizeEvent * ev ) override;

	void						setReadyForCallbacks( bool isReady );
	void						updateGuiPlayControls( bool isPlaying );
	void						startMediaPlay( int startPos );
	void						stopMediaIfPlaying( void );

	//=== vars ===//
	bool						m_ActivityCallbacksEnabled{ false };
	bool						m_IsPlaying{ false };
	bool						m_SliderIsPressed{ false };

	Ui::AssetVideoWidget		ui;
};

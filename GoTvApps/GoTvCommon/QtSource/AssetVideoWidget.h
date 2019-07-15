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
// http://www.gotvptop.com
//============================================================================

#include "AssetBaseWidget.h"
#include "ui_AssetVideoWidget.h"

class AssetVideoWidget : public AssetBaseWidget
{
	Q_OBJECT

public:
	AssetVideoWidget( QWidget * parent = 0 );
	AssetVideoWidget( AppCommon& appCommon, QWidget * parent = 0 );
	virtual ~AssetVideoWidget();

	virtual void				setAssetInfo( AssetInfo& assetInfo );
	virtual void				onActivityStop( void );

	virtual void				toGuiClientPlayVideoFrame( void * userData, VxGUID& onlineId, uint8_t * pu8Jpg, uint32_t u32JpgDataLen, int motion0To100000 );
    virtual int				    toGuiClientPlayVideoFrame( void * userData, VxGUID& onlineId, uint8_t * picBuf, uint32_t picBufLen, int picWidth, int picHeight );

	virtual void				showSendFail( bool show, bool permissionErr = false );
	virtual void				showResendButton( bool show );
	virtual void				showShredder( bool show );
	virtual void				showXferProgress( bool show );
	virtual void				setXferProgress( int sendProgress );

signals:
	void						signalPlayProgress( int pos );
	void						signalPlayEnd( void );

protected slots:
	virtual void				slotToGuiAssetAction( EAssetAction assetAction, int pos );
	void						slotPlayButtonClicked( void );
	void						slotShredAsset( void );
	void						slotSliderPressed( void );
	void						slotSliderReleased( void );

	void						slotPlayProgress( int pos0to100000 );
	void						slotPlayEnd( void );


protected:
	void						initAssetVideoWidget( void );

	void						showEvent( QShowEvent * ev );
	void						resizeEvent( QResizeEvent * ev );

	void						setReadyForCallbacks( bool isReady );
	void						updateGuiPlayControls( bool isPlaying );
	void						startMediaPlay( int startPos );
	void						stopMediaIfPlaying( void );

	//=== vars ===//
	bool						m_ActivityCallbacksEnabled;
	bool						m_IsPlaying;
	bool						m_SliderIsPressed;

	Ui::AssetVideoWidget		ui;
};

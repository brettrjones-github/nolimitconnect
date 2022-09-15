#pragma once
//============================================================================
// Copyright (C) 2018 Brett R. Jones
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

#include "AppletPlayerBase.h"

#include "GuiPlayerCallback.h"
#include "ui_AppletPlayerPhoto.h"

class AppletPlayerPhoto : public AppletPlayerBase, public GuiPlayerCallback
{
	Q_OBJECT
public:
	AppletPlayerPhoto( AppCommon& app, QWidget * parent );
	virtual ~AppletPlayerPhoto() = default;

	virtual void				setAssetInfo( AssetInfo& assetInfo ) override;

    virtual void				callbackGuiPlayMotionVideoFrame( VxGUID& feedOnlineId, QImage& vidFrame, int motion0To100000 ) override;
	virtual void				callbackGuiPlayVideoFrame( VxGUID& feedOnlineId, QImage& vidFrame ) override;

	virtual void				showShredder( bool show );
	virtual void				showXferProgress( bool show );
	virtual void				setXferProgress( int sendProgress );

	virtual void				toGuiClientAssetAction( EAssetAction assetAction, VxGUID& assetId, int pos0to100000 ) override;

	virtual bool				playMedia( AssetBaseInfo& assetInfo, int pos0to100000 = 0 ) override;

protected slots:
	void						slotPlayButtonClicked( void );
	void						slotShredAsset( void );
	void						slotSliderPressed( void );
	void						slotSliderReleased( void );

	void						slotPlayProgress( int pos0to100000 );
	void						slotPlayEnd( void );
    void                        slotMenuItemSelected( int menuId, EMenuItemType menuItemType );

protected:
	void						initAppletPlayerPhoto( void );
	void						onAppletStop( void );

    void						showEvent( QShowEvent * ev ) override;
    void						hideEvent( QHideEvent * ev ) override;
    void						resizeEvent( QResizeEvent * ev ) override;

	void						setReadyForCallbacks( bool isReady );
	void						updateGuiPlayControls( bool isPlaying );
	void						startMediaPlay( int startPos );
	void						stopMediaIfPlaying( void );
    void                        setupBottomMenu( VxMenuButton * menuButton );

	//=== vars ===//
	bool						m_ActivityCallbacksEnabled;
	bool						m_IsPlaying;
	bool						m_SliderIsPressed;

	Ui::AppletPlayerPhotoUi		ui;
};



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

#include "AppletBase.h"

#include "ui_AppletKodi.h"

class AppletKodi : public AppletBase
{
	Q_OBJECT
public:
	AppletKodi( AppCommon& app, QWidget* parent );
	virtual ~AppletKodi() = default;

    RenderGlWidget *            getRenderConsumer( void );


//	virtual void				setAssetInfo( AssetInfo& assetInfo ) override;
//
//	virtual void				callbackGuiPlayMotionVideoFrame( VxGUID& feedOnlineId, QImage& vidFrame, int motion0To100000 );
//    virtual int	    			toGuiClientPlayVideoFrame( VxGUID& onlineId, uint8_t * picBuf, uint32_t picBufLen, int picWidth, int picHeight );
//
//	virtual void				showShredder( bool show );
//	virtual void				showXferProgress( bool show );
//	virtual void				setXferProgress( int sendProgress );
//
protected slots:
    void                        slotMenuItemSelected( int menuId, EMenuItemType menuItemType );
//	void						slotPlayButtonClicked( void );
//	void						slotShredAsset( void );
//	void						slotSliderPressed( void );
//	void						slotSliderReleased( void );
//
//	void						slotPlayProgress( int pos0to100000 );
//	void						slotPlayEnd( void );

protected:
    // called just before first show of applet.. override for special initialization needs
    virtual void                aboutToLaunchApplet( void ) override;

	void						initAppletKodi( void );
	void						onAppletStop( void );

//	void                        startKodiModule( void );
//    void                        stopKodiModule( void );

	// resizing window
	virtual void				onResizeBegin( QSize& newSize ) override;
	virtual void				onResizeEvent( QSize& newSize ) override;
	virtual void				onResizeEnd( QSize& newSize ) override;

    void                        setupBottomMenu( VxMenuButton * menuButton );

    void						showEvent( QShowEvent* ev ) override;
    void						hideEvent( QHideEvent* ev ) override;
    void						resizeEvent( QResizeEvent* ev ) override;

	void						setReadyForCallbacks( bool isReady );
	//void						updateGuiPlayControls( bool isPlaying );
	//void						startMediaPlay( int startPos );
	//void						stopMediaIfPlaying( void );

	////=== vars ===//
	bool						m_ActivityCallbacksEnabled;
	//bool						m_IsPlaying;
	//bool						m_SliderIsPressed;

	Ui::AppletKodiClass	        ui;
};

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

#include "ui_ActivityScanWebCams.h"

#include <GuiInterface/IDefs.h>
#include <CoreLib/VxDefs.h>

#include <QString>
#include <QDialog>
#include <QTimer>

class VxNetIdent;
class VxGUID;

class ActivityScanWebCams : public ActivityBase
{
	Q_OBJECT
public:
	ActivityScanWebCams(	AppCommon& app, 
							QWidget *		parent = NULL );
	virtual ~ActivityScanWebCams() override;

    // overrides required for dialogs with there own title bar and bottom bar widgets
    virtual TitleBarWidget *	getTitleBarWidget( void ) override { return ui.m_TitleBarWidget; }
    virtual BottomBarWidget *	getBottomBarWidget( void ) override { return ui.m_BottomBarWidget; }

public:
	void						setTitle( QString strTitle );

    virtual void				toGuiClientScanSearchComplete( EScanType eScanType ) override;
    virtual void				toGuiScanResultSuccess( EScanType eScanType, GuiUser * netIdent ) override;
    virtual void				toGuiClientPlayVideoFrame( VxGUID& onlineId, uint8_t * pu8Jpg, uint32_t u32JpgDataLen,int motion0To100000 ) override;
	//! handle audio
	void						playAudio( uint16_t * pu16PcmData, uint16_t u16PcmDataLen, VxGUID& onlineId );

signals:
	void						signalNewWebCamSession( GuiUser * netIdent );
	void						signalPlayVideoFrame( QImage oPicBitmap, int iRotate );
	void						signalPlayAudio( unsigned short * pu16PcmData, unsigned short u16PcmDataLen );

public slots:
	void						slotNewWebCamSession( GuiUser * netIdent );
	void						slotPlayVideoFrame( QImage oPicBitmap, int iRotate );
	void						slotPlayAudio(  unsigned short * pu16PcmData, unsigned short u16PcmDataLen );

    void						slotHomeButtonClicked( void ) override;

	void						slotStartScanClicked();
	void						slotPauseScanClicked();
	void						slotNextScanClicked( void );

	void						onCountdownTimer( void );
	void						slotIdentMenuClicked( void );

protected:
    void						showEvent( QShowEvent * ev ) override;
    void						hideEvent( QHideEvent * ev ) override;

	void						setScanStatusText( QString strMsg );
	void						setupIdentWidget( GuiUser * netIdent );

	void						startCountdown();
	void						updateCountdownGui();
	void						startStopScan( bool startScan );
	void						showNextCam( void ); 
	void						doCamConnect( GuiUser * netIdent );
	void						updateAvailableGui( void );
	void						startWebCamSession( VxGUID& onlineId, bool startSession );
	void						setCamViewToOfflineImage( void );


	//=== vars ===//
	Ui::ScanWebCamsDialog		ui;
    GuiUser *				    m_HisIdent;
	int							m_iCountdownCnt;
	bool						m_bPaused;
	QTimer *					m_CountdownTimer;
	EScanType					m_eScanType;
	bool						m_bIconConnected;
	bool						m_bSearchComplete;
	QVector<GuiUser *>		    m_ScanList;
	bool						m_ShowNextCam;
	bool						m_IsScanning;
	VxGUID						m_LclSessionId;
};


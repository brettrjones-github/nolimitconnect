#pragma once
//============================================================================
// Copyright (C) 2019 Brett R. Jones 
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

#include <QWidget> // must be declared first or linux Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include <CoreLib/AssetDefs.h>
#include <CoreLib/VxTimer.h>

#include <QListWidget>

class ImageListRow;
class ThumbnailViewWidget;

class AppCommon;
class P2PEngine;
class ThumbInfo;
class ThumbMgr;
class VxGUID;

class ImageListWidget : public QListWidget
{
	Q_OBJECT

public:
	ImageListWidget( QWidget* parent );

	AppCommon&					getMyApp( void ) { return m_MyApp; }

    void                        loadThumbAssets( void );
    void                        clearImages( void );
    void                        clearItems( void );

    void                        addAsset( ThumbInfo * asset );

signals:
	void						signalImageClicked( ThumbnailViewWidget * thumb );

private slots:
	void						slotItemClicked( QListWidgetItem* );
    void						slotImageClicked( ThumbnailViewWidget * thumb );

protected:
    void                        resizeEvent( QResizeEvent* ev );

    bool                        thumbExistsInList( VxGUID& assetId );
    ImageListRow *              getRowWithRoomForThumbnail();

	//=== vars ===//
	AppCommon&					m_MyApp;
	P2PEngine&					m_Engine;
    ThumbMgr&					m_ThumbMgr;

	VxTimer						m_ClickEventTimer; // avoid duplicate clicks
};


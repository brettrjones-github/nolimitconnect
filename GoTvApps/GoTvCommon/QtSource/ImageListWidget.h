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
// http://www.nolimitconnect.com
//============================================================================


#include "VxGuidQt.h"

#include <CoreLib/VxTimer.h>

#include <CoreLib/VxTimer.h>
#include <CoreLib/AssetDefs.h>

#include <QListWidget>

class ImageListRow;
class ThumbnailViewWidget;

class AssetMgr;
class AppCommon;
class AssetInfo;
class P2PEngine;

class ImageListWidget : public QListWidget
{
	Q_OBJECT

public:
	ImageListWidget( QWidget * parent );

	AppCommon&					getMyApp( void ) { return m_MyApp; }

    void                        addAsset( AssetInfo * asset );

signals:
	void						signalImageClicked( ThumbnailViewWidget * thumb );

private slots:
	void						slotItemClicked(QListWidgetItem *);
    void						slotImageClicked( ThumbnailViewWidget * thumb );

protected:
    void                        resizeEvent( QResizeEvent * ev );

    ImageListRow *              getRowWithRoomForThumbnail();

	//=== vars ===//
	AppCommon&					m_MyApp;
	P2PEngine&					m_Engine;
    AssetMgr&					m_AssetMgr;

	VxTimer						m_ClickEventTimer; // avoid duplicate clicks
};


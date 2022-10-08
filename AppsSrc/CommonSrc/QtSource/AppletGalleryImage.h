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

#include "AppletBase.h"
#include "ui_AppletGalleryImage.h"


class ImageListEntry;
class AssetMgr;

class AppletGalleryImage : public AppletBase
{
	Q_OBJECT
public:
	AppletGalleryImage( AppCommon& app, QWidget* parent = nullptr );
	virtual ~AppletGalleryImage() override = default;

signals:
    void                        signalImageSelected( ThumbnailViewWidget * imageEntry );

private slots:
	void						slotImageClicked( ThumbnailViewWidget * imageEntry );

protected:

	//=== vars ===//
	Ui::AppletGalleryImageUi	ui;
    AssetMgr&					m_AssetMgr;
};

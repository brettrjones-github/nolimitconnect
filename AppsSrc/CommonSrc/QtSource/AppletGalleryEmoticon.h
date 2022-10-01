#pragma once
//============================================================================
// Copyright (C) 2021 Brett R. Jones
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
#include "ui_AppletGalleryEmoticon.h"

class ThumbnailViewWidget;
class ThumbMgr;

class AppletGalleryEmoticon : public AppletBase
{
	Q_OBJECT
public:
	AppletGalleryEmoticon( AppCommon& app, QWidget* parent = NULL );
	virtual ~AppletGalleryEmoticon() override;

signals:
    void                        signalThumbSelected( AppletBase * applet, ThumbnailViewWidget * thumb );

private slots:
	void						slotImageClicked( ThumbnailViewWidget * thumb );

protected:
    void                        resizeEvent( QResizeEvent* ev ) override;
    void                        showEvent( QShowEvent* ev ) override;

    void                        loadAssets( void );

	//=== vars ===//
	Ui::AppletGalleryEmoticonUi	ui;
    ThumbMgr&					m_ThumbMgr;
    bool                        m_isShown = false;
    bool                        m_isLoaded = false;

};

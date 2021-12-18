#pragma once
//============================================================================
// Copyright (C) 2009 Brett R. Jones 
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

#include "config_apps.h"
#include "ui_ImageListRow.h"

#include <QListWidgetItem>
#include <QObject>
#include <QPushButton>

class ThumbnailViewWidget;
class QHBoxLayout;
class VxGUID;

class ImageListRow : public QWidget, public QListWidgetItem
{
	Q_OBJECT;
public:
	ImageListRow( QListWidget * parent = 0 );
	virtual ~ImageListRow() = default;

    void                        addThumbnail( ThumbnailViewWidget * thumbnail );
    bool                        hasRoomForThumbnail( int idx = 0 );
    void                        setRowNum( int rowNum );

    void                        clearImages( void );
    void                        recalculateSizeHint( int listWidth, float displayScale );
    bool                        thumbExistsInList( VxGUID& assetId );

signals:
    void                        signalImageClicked( ThumbnailViewWidget * thumbnail );

protected slots:
    void                        slotImageClicked( ThumbnailViewWidget * thumbnail );

protected:
    virtual QSize				sizeHint() const override;

    //=== vars ===//
    Ui::ImageListRowUi	        ui;
    QSize                       m_SizeHint;
    bool                        m_LayoutInited = false;
    int                         m_ThumbnailCnt = 0;
};





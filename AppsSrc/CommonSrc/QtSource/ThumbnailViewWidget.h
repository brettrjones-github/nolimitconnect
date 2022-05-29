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
#pragma once

#include <QWidget> // must be declared first or linux Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds
#include <QLabel>

#include <CoreLib/VxGUID.h>

class AppCommon;
class QPixmap;
class ThumbInfo;

class ThumbnailViewWidget : public QLabel
{
    Q_OBJECT
public:
    ThumbnailViewWidget( QWidget * parent );
    ~ThumbnailViewWidget() override = default;

    void                        setIsUserPickedImage( bool userPicked )     { m_bUserPickedImage = userPicked;  }
    bool                        getIsUserPickedImage( void )                { return m_bUserPickedImage; }

    void                        setThumnailIsCircular( bool isCircle )      { m_ThumbnailIsCircular = isCircle; }
    bool                        getThumbnailIsCircular( void )              { return m_ThumbnailIsCircular; }

    void                        setThumbnailImage( QPixmap pixmap )         { cropAndUpdateImage( pixmap ); }
#if QT_VERSION > QT_VERSION_CHECK(6,0,0)
    QPixmap                     getThumbnailImage( void )                   { return pixmap(); }
#else
    const QPixmap*              getThumbnailImage(void)                     { return pixmap(); }
#endif // QT_VERSION > QT_VERSION_CHECK(6,0,0)

    void                        setThumbnailId( VxGUID& id )                { m_ThumbnailId = id; }
    VxGUID&                     getThumbnailId( void )                      { return m_ThumbnailId; }

    bool                        loadFromAsset( ThumbInfo * asset );
    bool                        loadFromFile( QString fileName );

    bool                        saveToPngFile( QString pngFileName );
    void                        browseForImage( void );

    void                        updateAssetImage( ThumbInfo* thumbAsset );

signals:
    void                        clicked( void );
    void						signalImageClicked( ThumbnailViewWidget * widget );

protected slots:
    void                        slotJpgSnapshot( uint8_t* pu8JpgData, uint32_t u32DataLen, int iWidth, int iHeight );

protected:
    virtual void				mousePressEvent( QMouseEvent * ev ) override;
    void                        cropAndUpdateImage( QPixmap& pixmap );
    QPixmap                     makeCircleImage( QPixmap& pixmap );

    AppCommon&					m_MyApp;
    bool                        m_bUserPickedImage{ false };
    VxGUID                      m_ThumbnailId;
    bool                        m_ThumbnailIsCircular{ false };
    QPixmap                     m_ThumbPixmap;
};

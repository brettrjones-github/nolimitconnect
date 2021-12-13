#pragma once
//============================================================================
// Copyright (C) 2017 Brett R. Jones
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

#include <QFrame>

class VxLabel;

class VxAspectFrame : public QFrame
{
    Q_OBJECT

public:
    VxAspectFrame( QWidget *parent = 0 );

    void                        setPreviewAspectRatio( float aspectRatio );
    float                       getPreviewAspectRatio( void )                       { return m_PreviewAspectRatio; }
    void                        setVideoScreenAspectRatio( float aspectRatio );
    float                       getVideoScreenAspectRatio( void )                   { return m_ScreenAspectRatio; }
   void                         setThumbnailPreview( VxLabel * thumbnailPreview )   { m_ThumbnailPreview = thumbnailPreview; }
    VxLabel *                   getThumbnailPreview( void )                         { return m_ThumbnailPreview; }
    void                        setVideoScreen( VxLabel * videoScreen )             { m_VideoScreen = videoScreen; }
    VxLabel *                   getVideoScreen( void )                              { return m_VideoScreen; }

protected:
    virtual void				resizeEvent( QResizeEvent * ev );
    virtual void                updatePositions( void );
    void						positionWidget( QWidget * parentWidget, float aspectRatio, VxLabel * widget, bool centerWidget, float scale );

    float                       m_PreviewAspectRatio;
    float                       m_ScreenAspectRatio;
    VxLabel *					m_ThumbnailPreview;
    float                       m_ThumbnailScale;
    VxLabel *					m_VideoScreen;
};


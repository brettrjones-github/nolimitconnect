#pragma once
//============================================================================
// Copyright (C) 2015 Brett R. Jones
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
// http://www.gotvptop.net
//============================================================================

#include "config_gotvapps.h"
#include <QLabel>
#include <atomic>


class VxLabel : public QLabel 
{
	Q_OBJECT
public:
	VxLabel( QWidget *parent=0, Qt::WindowFlags f=0 );
	VxLabel( const QString &text, QWidget *parent=0, Qt::WindowFlags f=0 );
	virtual ~VxLabel();

	virtual void				setTextBreakAnywhere( QString text, int maxLines = 2 );

	void						setResourceImage( QString resourceUrl );
	void						setImageFromFile( QString fileName );

	void						playVideoFrame( unsigned char * pu8Jpg, unsigned long u32JpgLen, int motion0To100000 );
    int                         playVideoFrame( uint8_t * picBuf, uint32_t picBufLen, int picWidth, int picHeight );

	void						setVidImageRotation( int imageRotation )		{ m_VidImageRotation = imageRotation; }
	int							getVidImageRotation( void )						{ return m_VidImageRotation; }

	QImage						getLastVideoImage( void )						{ return m_PreResizeImage; }
	void						showScaledImage( QImage& picBitmap );

signals:
	void						clicked();
	void						signalPlayVideoFrame( QImage picBitmap, int iRotate );

protected slots:
	void						slotPlayVideoFrame( QImage picBitmap, int iRotate );

protected:
	void						resizeBitmapToFitScreen( QImage& picBitmap );
	void						loadImageFromFile( QString fileName );

	virtual void				mousePressEvent( QMouseEvent * ev );
	virtual void				resizeEvent( QResizeEvent * ev );

	//=== vars ===//
    std::atomic_char32_t        m_behindFrameCnt;
	QString						m_ImageFileName;
	QString						m_OrigText;
	int							m_MaxLines;
	int							m_VidImageRotation;
	QImage						m_PreResizeImage;
};


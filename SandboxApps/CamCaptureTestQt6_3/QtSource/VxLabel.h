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
// http://www.nolimitconnect.org
//============================================================================

#include <QLabel>
#include <atomic>

class VxLabel : public QLabel 
{
	Q_OBJECT
public:
    VxLabel( QWidget *parent=0, Qt::WindowFlags f=Qt::Widget );
    VxLabel( const QString &text, QWidget *parent=0, Qt::WindowFlags f=Qt::Widget );
    virtual ~VxLabel() = default;

	virtual void				setTextBreakAnywhere( QString text, int maxLines = 2 );

	void						setResourceImage( QString resourceUrl, bool scaleToLabelSize = true );
	void						setImageFromFile( QString fileName );

	void						playVideoFrame( unsigned char * pu8Jpg, unsigned long u32JpgLen, int motion0To100000 );
    int                         playVideoFrame( uint8_t * picBuf, uint32_t picBufLen, int picWidth, int picHeight );

	void						setVidImageRotation( int imageRotation )		{ m_VidImageRotation = imageRotation; }
	int							getVidImageRotation( void )						{ return m_VidImageRotation; }

	QImage						getLastVideoImage( void )						{ return m_PreResizeImage; }
	void						showScaledImage( QImage& picBitmap );

signals:
	void						clicked();
	// Qt 6.2.2 passing QImage through queued signal/slot makes the image white.. must use QPixmap instead
	void						signalPlayVideoFrame( QPixmap picBitmap, int iRotate );

protected slots:
	// Qt 6.2.2 passing QImage through queued signal/slot makes the image white.. must use QPixmap instead
	void						slotPlayVideoFrame( QPixmap picBitmap, int iRotate );

protected:
	void						resizeBitmapToFitScreen( QImage& picBitmap );
	void						loadImageFromFile( QString fileName );

	virtual void				mousePressEvent( QMouseEvent * ev ) override;
	virtual void				resizeEvent( QResizeEvent * ev ) override;

	//=== vars ===//
    std::atomic_char32_t        m_behindFrameCnt;
	QString						m_ImageFileName;
	QString						m_OrigText;
	int							m_MaxLines;
    int							m_VidImageRotation{ 0 };
	QImage						m_PreResizeImage;
};


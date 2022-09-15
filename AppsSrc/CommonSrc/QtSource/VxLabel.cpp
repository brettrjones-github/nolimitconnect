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

#include <QWidget> // must be declared first or Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include "VxLabel.h"

#include <CoreLib/VxDebug.h>

//============================================================================
VxLabel::VxLabel(QWidget *parent, Qt::WindowFlags f) 
: QLabel(parent,f) 
{
    setObjectName( "VxLabel" );
}

//============================================================================
VxLabel::VxLabel(const QString &text, QWidget *parent, Qt::WindowFlags f) 
: QLabel(text,parent,f) 
{
}

//============================================================================
void VxLabel::mousePressEvent(QMouseEvent * event)
{
	QWidget::mousePressEvent(event);
	emit clicked();
}

//============================================================================
void VxLabel::resizeEvent(QResizeEvent* resizeEvent)
{
	QLabel::resizeEvent(resizeEvent);
	if( false == m_ImageFileName.isEmpty() )
	{
		QImage	picBitmap;
		if( picBitmap.load( m_ImageFileName ) )
		{
			showScaledImage( picBitmap );
		}
	}

	if( !m_OrigText.isEmpty() 
		&& ( 1 < m_MaxLines ) )
	{
		setTextBreakAnywhere( m_OrigText, m_MaxLines );
	}
}

//============================================================================
void VxLabel::showScaledImage( QImage& picBitmap )
{
	if( picBitmap.isNull() )
	{
		LogMsg( LOG_ERROR, "VxLabel::showScaledImage image is null " );
		return;
	}

	float picWidth = picBitmap.width();
	float picHeight = picBitmap.height();
	float thisWidth = this->geometry().width() - 2;
	float thisHeight = this->geometry().height() - 2;
	if( ( thisWidth > 0 ) && ( thisHeight > 0 ) )
	{
		float scaleWidth = thisWidth / picWidth;
		float scaleHeight = thisHeight / picHeight;
		float picScale = ( scaleWidth < scaleHeight ) ? scaleWidth : scaleHeight;

		QSize newPicSize( picWidth * picScale, picHeight * picScale );
		QImage scaledBitmap = picBitmap.scaled( newPicSize, Qt::KeepAspectRatio, Qt::SmoothTransformation );
		if( ! scaledBitmap.isNull() )
		{
			setPixmap( QPixmap::fromImage( scaledBitmap ) );
			repaint();
		}
	}
}

//============================================================================
void VxLabel::setTextBreakAnywhere( QString text, int maxLines )
{
	//LogMsg( LOG_INFO, "VxLabel::setTextBreakAnywhere %s start\n", text.toUtf8().constData() );
	m_OrigText = text;
	m_MaxLines = maxLines;

	const QFont& font = this->font(); 
	QRect myRect = geometry();
	QString myText = "";
	QFontMetrics fm(font);
	QString remainingText = text;
	bool foundFit = false;
	for( int iLineNum = 0; iLineNum < maxLines; iLineNum++ )
	{
		// work backwards until fits
		QString testText;

		for( int textLen = remainingText.length(); textLen > 0; textLen-- )
		{
			testText = remainingText.left( textLen );
            #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                if (fm.horizontalAdvance(testText) <= myRect.width())
            #else
			    if( fm.width(testText) <= myRect.width() )
            #endif
			{
				// fits5
				foundFit = true;
				if( !myText.isEmpty() )
				{
					myText += " ";
				}

				myText += testText;
				remainingText = remainingText.right( remainingText.length() - textLen );
				
				break;
			}
		}

		if( false == foundFit )
		{
			// could not fit even a single char
			this->setText( text );
			return;
		}

		if( remainingText.isEmpty() )
		{
			break;
		}
	}

	if( !remainingText.isEmpty() )
	{
		myText += remainingText;
	}

	this->setText( myText );
	//LogMsg( LOG_INFO, "VxLabel::setTextBreakAnywhere %s done\n", text.toUtf8().constData() );
}

//============================================================================
void VxLabel::setResourceImage( QString resourceUrl, bool scaleToLabelSize )
{
	setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
	QImage picBitmap( resourceUrl ); 
    if( !picBitmap.isNull() )
    {
        if( scaleToLabelSize )
        {
            showScaledImage( picBitmap );
        }
        else
        {
            QPixmap pixmap = QPixmap::fromImage( picBitmap );
            if( !pixmap.isNull() )
            {
                setPixmap( pixmap );
                update();
            }
            else
            {
                LogMsg( LOG_ERROR, "VxLabel::setResourceImage NULL pixmap" );
            }
        }
    }
    else
    {
        LogMsg( LOG_ERROR, "VxLabel::setResourceImage %s failed", resourceUrl.toUtf8().constData() );
    }
}

//============================================================================
bool VxLabel::setImageFromFile( QString fileName )
{
	m_ImageFileName = fileName;
	return loadImageFromFile( m_ImageFileName );
}

//============================================================================
bool VxLabel::loadImageFromFile( QString fileName )
{
	QImage	picBitmap;
	if( picBitmap.load( fileName ) && !picBitmap.isNull() )
	{
		playMotionVideoFrame( picBitmap, 0 );
		return true;
	}
    else
    {
        LogMsg( LOG_DEBUG, "ERROR VxLabel::loadImageFromFile failed %s", fileName.toUtf8().constData() );
		return false;
    }
}

//============================================================================
void VxLabel::playMotionVideoFrame( QImage& vidFrame, int motion0To100000 )
{
	playRotatedVideoFrame( vidFrame, m_VidImageRotation );
}

//============================================================================
void VxLabel::playVideoFrame( QImage& vidFrame )
{
	playRotatedVideoFrame( vidFrame, m_VidImageRotation );
}

//============================================================================
void VxLabel::playRotatedVideoFrame( QImage& vidFrame, int iRotate )
{
    if( !vidFrame.isNull() && isVisible() )
    {
		m_PreResizeImage = vidFrame;  // for camera snapshot

        QSize screenSize( this->width(), this->height() );
        QImage resizedPicmap = m_PreResizeImage.scaled( screenSize, Qt::KeepAspectRatio );
        if( iRotate )
        {
            #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                QTransform rm;
            #else
                QMatrix rm; 
            #endif
            rm.rotate( iRotate );
            QPixmap pixmap = QPixmap::fromImage( resizedPicmap ).transformed( rm );
            if( !pixmap.isNull() )
            {
                setPixmap( pixmap );
                update();
            }
            else
            {
                LogMsg( LOG_DEBUG, " VxLabel::slotPlayVideoFrame NULL pixmap" );
            }
        }
        else
        {
            QPixmap pixmap = QPixmap::fromImage( resizedPicmap );
            if( !pixmap.isNull() )
            {
                setPixmap( pixmap );
                update();
            }
            else
            {
                LogMsg( LOG_DEBUG, " VxLabel::slotPlayVideoFrame NULL pixmap" );
            }
        }
    }
    else if( vidFrame.isNull() )
    {
        LogMsg( LOG_ERROR, " VxLabel::slotPlayVideoFrame NULL picBitmap" );
    }
}

//============================================================================
void VxLabel::resizeBitmapToFitScreen( QImage& picBitmap )
{
	QSize screenSize( this->width(), this->height() );
	picBitmap = picBitmap.scaled(screenSize, Qt::KeepAspectRatio, Qt::SmoothTransformation );
}

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


#include "VxFlatButton.h"
#include "AppCommon.h"
#include "VxAppTheme.h"
#include "MyIcons.h"

#include <CoreLib/VxDebug.h>

#include <QPainter>
#include <QMouseEvent>

//============================================================================
VxFlatButton::VxFlatButton( QWidget *parent ) 
: QPushButton( parent ) 
, m_MyApp( GetAppInstance() )
, m_MyIcon( eMyIconUnknown )
{
    setObjectName( "VxFlatButton" );
}

//============================================================================
MyIcons&  VxFlatButton::getMyIcons( void )
{
	return m_MyApp.getMyIcons();
}

//============================================================================
void VxFlatButton::paintEvent( QPaintEvent* ev )
{
	Q_UNUSED( ev );
#define IMAGE_PADDING  8
	QRect drawRect( IMAGE_PADDING, IMAGE_PADDING, this->width() - ( IMAGE_PADDING * 2 ), this->height() - ( IMAGE_PADDING * 2 ) );
	VxAppTheme& appTheme = getMyApp().getAppTheme();
	QColor iconColor = appTheme.getButtonColor( this, eColorLayerIcon );
	QString svgName = m_MyApp.getMyIcons().getIconFile( m_MyIcon );
	if( ( m_LastIconColor != iconColor ) 
		|| m_IconImage.isNull() 
		|| ( drawRect.size() != m_LastIconSize ) )
	{
		if( drawRect.width() < 8 )
		{
			LogMsg( LOG_ERROR, "VxFlatButton::paintEvent invalid drawRect.width()  %d ", drawRect.width() );
			return;
		}

		if( m_MyApp.getMyIcons().isSvgFile( svgName ) )
		{
			m_IconImage = appTheme.svgRenderer( svgName, drawRect.size(), iconColor );
			if( m_IconImage.isNull() )
			{
				LogMsg( LOG_ERROR, "VxFlatButton::paintEvent svg m_IconImage.isNull() %s", svgName.toUtf8().constData() );
				return;
			}
			else
			{
				m_LastIconColor = iconColor;
				m_LastIconSize = drawRect.size();
			}
		}
		else
		{
			QIcon& thisIcon = m_MyApp.getMyIcons().getIcon( m_MyIcon );
			if( thisIcon.isNull() )
			{
				LogMsg( LOG_ERROR, "VxFlatButton::paintEvent thisIcon.isNull() %s", svgName.toUtf8().constData() );
				return;
			}

			m_IconImage = thisIcon.pixmap( drawRect.size() );
		}
	}

	if( m_IconImage.isNull() )
	{
		LogMsg( LOG_ERROR, "VxFlatButton::paintEvent m_IconImage.isNull() file name is  %s", svgName.toUtf8().constData() );
		return;
	}
	else
	{
		QPainter painter( this );
		painter.save();
		appTheme.drawButton( this, painter );
		painter.restore();
		painter.setRenderHint( QPainter::Antialiasing, true );
		painter.setRenderHint( QPainter::TextAntialiasing, true );
		painter.setRenderHint( QPainter::SmoothPixmapTransform, true );
		painter.drawPixmap( drawRect, m_IconImage  );
		painter.end();
	}
}

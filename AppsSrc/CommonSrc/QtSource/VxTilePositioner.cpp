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
// http://www.nolimitconnect.org
//============================================================================


#include "VxTilePositioner.h"
#include "VxWidgetBase.h"
#include "GuiParams.h"

#include <CoreLib/VxDebug.h>

#include <QWidget>
#if defined(TARGET_OS_ANDROID) || defined(TARGET_OS_LINUX)
# include <math.h>
#endif // defined(TARGET_OS_ANDROID)

//============================================================================
VxTilePositioner::VxTilePositioner( AppCommon& myApp )
: m_MyApp( myApp )
{
}

// find optimal (largest) tile size for which
// at least N tiles fit in WxH rectangle
double optimal_size( double W, double H, int N )
{
	int i_min, j_min; // minimum values for which you get at least N tiles 
	for( int i = round( sqrt( N*W / H ) ); ; i++ ) {
		if( i*floor( H*i / W ) >= N ) {
			i_min = i;
			break;
		}
	}
	for( int j = round( sqrt( N*H / W ) ); ; j++ ) {
		if( floor( W*j / H )*j >= N ) {
			j_min = j;
			break;
		}
	}
	return std::max( W / i_min, H / j_min );
}

//============================================================================
void VxTilePositioner::repositionTiles( QVector<VxWidgetBase *>& widgetList, QWidget* parentWindow, int padding )
{
	if( !parentWindow 
		|| (  0 == widgetList.size() ) )
	{
		// nothing to do;
		return;
	}

	QSize windowSize = parentWindow->geometry().size();
	if( ( ( padding * 2 + 8 ) > windowSize.width() )
		|| ( ( padding * 2 + 8 ) > windowSize.height() ) )
	{
		LogMsg( LOG_ERROR, "VxTilePositioner::repositionTiles invalid Window size %d %d", windowSize.width(),  windowSize.height() );
		return;
	}


	int totalTiles = widgetList.size();

	int optimumTileSize = (int)optimal_size( windowSize.width()-(padding * 2), windowSize.height()-(padding * 2), totalTiles );
	if( optimumTileSize <= 0 )
	{
		LogMsg( LOG_ERROR, "VxTilePositioner::repositionTiles invalid tile size %d", optimumTileSize );
		return;
	}

	int columnCnt = windowSize.width() / optimumTileSize;
	int rowCnt = windowSize.height() / optimumTileSize;

	if( columnCnt * rowCnt < totalTiles )
	{
		// need plus totalTiles + 1 to account for rounding issues
		optimumTileSize = ( int )optimal_size( windowSize.width(), windowSize.height(), totalTiles + 1 );
	}

	// limit tile size to a reasonable large size
	if( optimumTileSize > GuiParams::getMaxTiledIconSize() )
	{
		optimumTileSize = GuiParams::getMaxTiledIconSize();
	}

	columnCnt = windowSize.width() / optimumTileSize;
	rowCnt = windowSize.height() / optimumTileSize;
	if( columnCnt * rowCnt < totalTiles )
	{
		LogMsg( LOG_ERROR, "VxTilePositioner::repositionTiles invalid tile size %d", optimumTileSize );
		return;
	}
	 
	int tileRowOffs = padding;
	int tileWidthAfterPadding =  optimumTileSize - ( padding * 2 );
	int tilesDisplayed = 0;
	int rowWidthPixels = windowSize.width();
	while( tilesDisplayed < totalTiles )
	{
		// fill as many tiles as we can in a row
		int iXPos = padding;
		while( ( rowWidthPixels >= ( iXPos + optimumTileSize ) )
			   && ( tilesDisplayed < totalTiles ) )
		{
			VxWidgetBase * widget = widgetList[ tilesDisplayed ];
            widget->setFixedSize( tileWidthAfterPadding, tileWidthAfterPadding );
            widget->move( iXPos, tileRowOffs );

#if defined(TARGET_OS_ANDROID)
            widget->updateGeometry();
            widget->update();
#endif // defined(TARGET_OS_ANDROID)

			tilesDisplayed++;
			iXPos += optimumTileSize;
		}

		tileRowOffs += optimumTileSize;
	}
}

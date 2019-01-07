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
// brett.jones@engineer.com
// http://www.gotvptop.net
//============================================================================

#include "VxTilePositioner.h"
#include "VxWidgetBase.h"

#include <CoreLib/VxDebug.h>

#include <QWidget>

//============================================================================
VxTilePositioner::VxTilePositioner( AppCommon& myApp )
: m_MyApp( myApp )
{
}

//============================================================================
VxTilePositioner::~VxTilePositioner()
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
void VxTilePositioner::repositionTiles( QVector<VxWidgetBase *>& widgetList, QWidget * parentWindow, int padding )
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
		LogMsg( LOG_ERROR, "VxTilePositioner::repositionTiles invalid Window size %d %d\n", windowSize.width(),  windowSize.height()  );
		return;
	}

	int totalTiles = widgetList.size();
	// need plus totalTiles + 1 to account for rounding issues
	int optimumTileSize = (int)optimal_size( windowSize.width()-(padding * 2), windowSize.height()-(padding * 2), totalTiles );
	if( optimumTileSize <= 0 )
	{
		LogMsg( LOG_ERROR, "VxTilePositioner::repositionTiles invalid tile size %d\n", optimumTileSize );
		return;
	}

	int columnCnt = windowSize.width() / optimumTileSize;
	int rowCnt = windowSize.height() / optimumTileSize;

	if( columnCnt * rowCnt < totalTiles )
	{
		optimumTileSize = ( int )optimal_size( windowSize.width(), windowSize.height(), totalTiles + 1 );
	}

	if( columnCnt * rowCnt < totalTiles )
	{
		LogMsg( LOG_ERROR, "VxTilePositioner::repositionTiles invalid tile size %d\n", optimumTileSize );
	}

	//LogMsg( LOG_DEBUG, "repositionTiles total width %d height %d tile width %d\n", 
	//		windowSize.width(),
	//		windowSize.height(), 
	//		totalTiles );
	 
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
			tilesDisplayed++;
			iXPos += optimumTileSize;
		}

		tileRowOffs += optimumTileSize;
	}

}

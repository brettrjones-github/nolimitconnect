//============================================================================
// Copyright (C) 2019 Brett R. Jones
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

#include <QWidget> // must be declared first or linux Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include "ImageListEntry.h"
#include "AppCommon.h"
#include "GuiHelpers.h"
#include "GuiParams.h"

#include <CoreLib/VxDebug.h>
#include <ptop_src/ptop_engine_src/AssetMgr/AssetInfo.h>
#include <ptop_src/ptop_engine_src/AssetMgr/AssetMgr.h>
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <VxVideoLib/VxVideoLib.h>

#include <QMessageBox>
#include <QPainter>

//============================================================================
ImageListEntry::ImageListEntry( QWidget * parent )
    : ThumbnailViewWidget( parent )
{
}

//============================================================================
bool ImageListEntry::loadFromAsset( AssetInfo * thumbAsset )
{
    bool loadOk = false;
    if( thumbAsset )
    {
        loadOk = loadFromFile( thumbAsset->getAssetName().c_str() );
    }

    return loadOk;
}

//============================================================================
void ImageListEntry::slotThumbGalleryClick( void )
{

}

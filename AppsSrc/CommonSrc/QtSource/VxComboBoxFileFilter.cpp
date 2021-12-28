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
// http://www.nolimitconnect.com
//============================================================================

#include "VxComboBoxFileFilter.h"

#include <CoreLib/VxFileInfo.h>

namespace
{
	QString g_QComboBoxFileFilterEntries[] =
	{
		QObject::tr("No filter (All File Types)"),
		QObject::tr("Photos"),
		QObject::tr("Music"),
		QObject::tr("Videos"),
		QObject::tr("Documents"),
		QObject::tr("Archives Or ISO"),
		QObject::tr("Other Files"),
        QObject::tr( "Unknown" ),
        QObject::tr( "Photos" ),
        QObject::tr( "Music" ),
        QObject::tr( "Videos" )
	};

	unsigned char g_QComboBoxFileMasks[] =
	{
		VXFILE_TYPE_ALLNOTEXE,
		VXFILE_TYPE_PHOTO,
		VXFILE_TYPE_AUDIO,
		VXFILE_TYPE_VIDEO,
		VXFILE_TYPE_DOC,
		VXFILE_TYPE_ARCHIVE_OR_CDIMAGE,
		VXFILE_TYPE_OTHER,
        VXFILE_TYPE_ALLNOTEXE,
        VXFILE_TYPE_PHOTO,
        VXFILE_TYPE_AUDIO,
        VXFILE_TYPE_VIDEO
	};
}

//============================================================================
VxComboBoxFileFilter::VxComboBoxFileFilter(QWidget *parent) 
: QComboBox(parent) 
, m_CurFileMask( VXFILE_TYPE_ALLNOTEXE )
, m_FileFilterType( eFileFilterAll )
{
    setObjectName( "VxComboBoxFileFilter" );
    setFileFilter( m_FileFilterType );

	connect( this, SIGNAL(currentIndexChanged(int)),	this,	SLOT(handleSelectionChanged(int)) );
}

//============================================================================
VxComboBoxFileFilter::~VxComboBoxFileFilter()
{
}

//============================================================================
unsigned char VxComboBoxFileFilter::getCurrentFileFilterMask( void )
{
    EFileFilterType filterType = getFileFilterTypeFromIndex( currentIndex () );
    return  getMaskFromFileFilterType( filterType );
}

//============================================================================
EFileFilterType VxComboBoxFileFilter::getCurrentFileFilterType( void )
{
    return  getFileFilterTypeFromIndex( currentIndex() );
}

//============================================================================
uint8_t VxComboBoxFileFilter::getMaskFromFileFilterType( EFileFilterType eFileFilterType )
{
    uint8_t filterMask = g_QComboBoxFileMasks[ eFileFilterAll ];
    if( eMaxFileFilterType > eFileFilterType )
    {
        filterMask = g_QComboBoxFileMasks[ eFileFilterType ];
    }

    return filterMask;
}

//============================================================================
EFileFilterType VxComboBoxFileFilter::getFileFilterTypeFromIndex( int selectionIdx )
{
    EFileFilterType filterType = eFileFilterAll;
    switch( m_FileFilterType )
    {

    case eFileFilterPhotoOnly:
        if( 0 != selectionIdx )
        {
            filterType = eFileFilterPhoto;
        }
        break;

    case eFileFilterAudioOnly:
        if( 0 != selectionIdx )
        {
            filterType = eFileFilterAudio;
        }
        break;

    case eFileFilterVideoOnly:
        if( 0 != selectionIdx )
        {
            filterType = eFileFilterVideo;
        }
        break;

    case eFileFilterAll:
    case eFileFilterPhoto:
    case eFileFilterAudio:
    case eFileFilterVideo:
    case eFileFilterDocuments:
    case eFileFilterArchive:
    case eFileFilterOther:
    default:
    {
        if( eFileFilterEnd > selectionIdx )
        {
            filterType = (EFileFilterType)selectionIdx;
        }
    }
    break;
    }

    return filterType;
}

//============================================================================
void VxComboBoxFileFilter::handleSelectionChanged( int idx )
{
    if( 0 <= idx )
    {
        EFileFilterType filterType = getFileFilterTypeFromIndex( idx );
        m_CurFileMask = getMaskFromFileFilterType( filterType );
        emit signalApplyFileFilter( m_CurFileMask );
        emit signalFileFilterSelectionChanged( filterType);
    }
}

//============================================================================
void VxComboBoxFileFilter::setFileFilter( EFileFilterType eFileFilter )
{
    m_FileFilterType = eFileFilter;
    clear();
    switch( eFileFilter )
    {

    case eFileFilterPhoto:
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterAll ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterPhoto ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterAudio ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterVideo ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterDocuments ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterArchive ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterOther ] );
        setCurrentIndex( getSelectionIndexFromFilterType( eFileFilterPhoto ) );
        break;

    case eFileFilterAudio:
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterAll ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterPhoto ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterAudio ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterVideo ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterDocuments ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterArchive ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterOther ] );
        setCurrentIndex( getSelectionIndexFromFilterType( eFileFilterAudio ) );
        break;

    case eFileFilterVideo:
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterAll ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterPhoto ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterAudio ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterVideo ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterDocuments ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterArchive ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterOther ] );
        setCurrentIndex( getSelectionIndexFromFilterType( eFileFilterVideo ) );
        break;

    case eFileFilterDocuments:
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterAll ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterPhoto ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterAudio ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterVideo ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterDocuments ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterArchive ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterOther ] );
        setCurrentIndex( getSelectionIndexFromFilterType( eFileFilterDocuments ) );
        break;

    case eFileFilterArchive:
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterAll ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterPhoto ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterAudio ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterVideo ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterDocuments ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterArchive ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterOther ] );
        setCurrentIndex( getSelectionIndexFromFilterType( eFileFilterArchive ) );
        break;

    case eFileFilterOther:
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterAll ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterPhoto ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterAudio ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterVideo ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterDocuments ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterArchive ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterOther ] );
        setCurrentIndex( getSelectionIndexFromFilterType( eFileFilterOther ) );
        break;

    case eFileFilterPhotoOnly:
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterAll ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterPhoto ] );
        setCurrentIndex( getSelectionIndexFromFilterType( eFileFilterPhoto ) );
        break;

    case eFileFilterAudioOnly:
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterAll ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterAudio ] );
        setCurrentIndex( getSelectionIndexFromFilterType( eFileFilterAudio ) );
        break;

    case eFileFilterVideoOnly:
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterAll ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterVideo ] );
        setCurrentIndex( getSelectionIndexFromFilterType( eFileFilterVideo ) );
        break;

    case eFileFilterAll:
    case eFileFilterEnd:
    default:
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterAll ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterPhoto ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterAudio ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterVideo ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterDocuments ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterArchive ] );
        addItem( g_QComboBoxFileFilterEntries[ eFileFilterOther ] );
        setCurrentIndex( getSelectionIndexFromFilterType( eFileFilterAll ) );
    }

    m_CurFileMask = getMaskFromFileFilterType( eFileFilter );
}

//============================================================================
int VxComboBoxFileFilter::getSelectionIndexFromFilterType( EFileFilterType eFileFilter )
{
    int idx = 0;
    switch( m_FileFilterType )
    {
    case eFileFilterAll:
    case eFileFilterPhoto:
    case eFileFilterAudio:
    case eFileFilterVideo:
    case eFileFilterDocuments:
    case eFileFilterArchive:
    case eFileFilterOther:
    {
        if( eFileFilterEnd > eFileFilter )
        {
            idx = (int)eFileFilter;
        }
    }
    break;

    case eFileFilterPhotoOnly:
    case eFileFilterAudioOnly:
    case eFileFilterVideoOnly:
        if( 0 == eFileFilter )
        {
            return 0;
        }
        else
        {
            return 1;
        }
    break;

    default:
        return 0;
    }

    return idx;
}



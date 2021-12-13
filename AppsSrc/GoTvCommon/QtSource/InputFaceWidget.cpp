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
#include <app_precompiled_hdr.h>
#include "InputFaceWidget.h"
#include "AppCommon.h"
#include "GuiParams.h"

#include <CoreLib/VxGlobals.h>

#include <QDebug>

#include <time.h>

//============================================================================
InputFaceWidget::InputFaceWidget( QWidget * parent )
: InputBaseWidget( GetAppInstance(), parent )
{
	m_AssetInfo.setAssetType( eAssetTypeChatFace );
	qDebug() << "InputFaceWidget::InputFaceWidget ";

	ui.setupUi( this );
    ui.m_CancelFaceButton->setSquareButtonSize( eButtonSizeTiny );

	connect( ui.m_FaceLabel1_1,		SIGNAL(clicked()),	this, SLOT(slotFace1LabelClicked()) );
	m_FaceList.push_back( ui.m_FaceLabel1_1 );
	connect( ui.m_FaceLabel1_2,		SIGNAL(clicked()),	this, SLOT(slotFace2LabelClicked()) );
	m_FaceList.push_back( ui.m_FaceLabel1_2 );
	connect( ui.m_FaceLabel1_3,		SIGNAL(clicked()),	this, SLOT(slotFace3LabelClicked()) );
	m_FaceList.push_back( ui.m_FaceLabel1_3 );
	connect( ui.m_FaceLabel1_4,		SIGNAL(clicked()),	this, SLOT(slotFace4LabelClicked()) );
	m_FaceList.push_back( ui.m_FaceLabel1_4 );
	connect( ui.m_FaceLabel1_5,		SIGNAL(clicked()),	this, SLOT(slotFace5LabelClicked()) );
	m_FaceList.push_back( ui.m_FaceLabel1_5 );
	connect( ui.m_FaceLabel1_6,		SIGNAL(clicked()),	this, SLOT(slotFace6LabelClicked()) );
	m_FaceList.push_back( ui.m_FaceLabel1_6 );
	connect( ui.m_FaceLabel1_7,		SIGNAL(clicked()),	this, SLOT(slotFace7LabelClicked()) );
	m_FaceList.push_back( ui.m_FaceLabel1_7 );
	connect( ui.m_FaceLabel1_8,		SIGNAL(clicked()),	this, SLOT(slotFace8LabelClicked()) );
	m_FaceList.push_back( ui.m_FaceLabel1_8 );
	connect( ui.m_FaceLabel1_9,		SIGNAL(clicked()),	this, SLOT(slotFace9LabelClicked()) );
	m_FaceList.push_back( ui.m_FaceLabel1_9 );
	connect( ui.m_FaceLabel1_10,		SIGNAL(clicked()),	this, SLOT(slotFace10LabelClicked()) );
	m_FaceList.push_back( ui.m_FaceLabel1_10 );
	connect( ui.m_FaceLabel1_11,		SIGNAL(clicked()),	this, SLOT(slotFace11LabelClicked()) );
	m_FaceList.push_back( ui.m_FaceLabel1_11 );
	connect( ui.m_FaceLabel1_12,		SIGNAL(clicked()),	this, SLOT(slotFace12LabelClicked()) );
	m_FaceList.push_back( ui.m_FaceLabel1_12 );
	connect( ui.m_FaceLabel1_13,		SIGNAL(clicked()),	this, SLOT(slotFace13LabelClicked()) );
	m_FaceList.push_back( ui.m_FaceLabel1_13 );
	connect( ui.m_FaceLabel1_14,		SIGNAL(clicked()),	this, SLOT(slotFace14LabelClicked()) );
	m_FaceList.push_back( ui.m_FaceLabel1_14 );
	connect( ui.m_FaceLabel1_15,		SIGNAL(clicked()),	this, SLOT(slotFace15LabelClicked()) );
	m_FaceList.push_back( ui.m_FaceLabel1_15 );
	connect( ui.m_FaceLabel1_16,		SIGNAL(clicked()),	this, SLOT(slotFace16LabelClicked()) );
	m_FaceList.push_back( ui.m_FaceLabel1_16 );
	connect( ui.m_FaceLabel1_17,		SIGNAL(clicked()),	this, SLOT(slotFace17LabelClicked()) );
	m_FaceList.push_back( ui.m_FaceLabel1_17 );
	connect( ui.m_FaceLabel1_18,		SIGNAL(clicked()),	this, SLOT(slotFace18LabelClicked()) );
	m_FaceList.push_back( ui.m_FaceLabel1_18 );
	connect( ui.m_FaceLabel1_19,		SIGNAL(clicked()),	this, SLOT(slotFace19LabelClicked()) );
	m_FaceList.push_back( ui.m_FaceLabel1_19 );
	connect( ui.m_FaceLabel1_20,		SIGNAL(clicked()),	this, SLOT(slotFace20LabelClicked()) );
	m_FaceList.push_back( ui.m_FaceLabel1_20 );
	connect( ui.m_FaceLabel1_21,		SIGNAL(clicked()),	this, SLOT(slotFace21LabelClicked()) );
	m_FaceList.push_back( ui.m_FaceLabel1_21 );
	connect( ui.m_FaceLabel1_22,		SIGNAL(clicked()),	this, SLOT(slotFace22LabelClicked()) );
	m_FaceList.push_back( ui.m_FaceLabel1_22 );
	connect( ui.m_FaceLabel1_23,		SIGNAL(clicked()),	this, SLOT(slotFace23LabelClicked()) );
	m_FaceList.push_back( ui.m_FaceLabel1_23 );
	connect( ui.m_FaceLabel1_24,		SIGNAL(clicked()),	this, SLOT(slotFace24LabelClicked()) );
	m_FaceList.push_back( ui.m_FaceLabel1_24 );
	connect( ui.m_FaceLabel1_25,		SIGNAL(clicked()),	this, SLOT(slotFace25LabelClicked()) );
	m_FaceList.push_back( ui.m_FaceLabel1_25 );
	connect( ui.m_FaceLabel1_26,		SIGNAL(clicked()),	this, SLOT(slotFace26LabelClicked()) );
	m_FaceList.push_back( ui.m_FaceLabel1_26 );
	connect( ui.m_FaceLabel1_27,		SIGNAL(clicked()),	this, SLOT(slotFace27LabelClicked()) );
	m_FaceList.push_back( ui.m_FaceLabel1_27 );
	connect( ui.m_FaceLabel1_28,		SIGNAL(clicked()),	this, SLOT(slotFace28LabelClicked()) );
	m_FaceList.push_back( ui.m_FaceLabel1_28 );
	connect( ui.m_FaceLabel1_29,		SIGNAL(clicked()),	this, SLOT(slotFace29LabelClicked()) );
	m_FaceList.push_back( ui.m_FaceLabel1_29 );
	connect( ui.m_FaceLabel1_30,		SIGNAL(clicked()),	this, SLOT(slotFace30LabelClicked()) );
	m_FaceList.push_back( ui.m_FaceLabel1_30 );
	connect( ui.m_FaceLabel1_31,		SIGNAL(clicked()),	this, SLOT(slotFace31LabelClicked()) );
	m_FaceList.push_back( ui.m_FaceLabel1_31 );
	connect( ui.m_FaceLabel1_32,		SIGNAL(clicked()),	this, SLOT(slotFace32LabelClicked()) );
	m_FaceList.push_back( ui.m_FaceLabel1_32 );

	char resBuf[128];
	QSize imageSize( 40, 40 );
	for( int i = 0; i < m_FaceList.size(); i++ )
	{
		VxLabel * faceLabel = m_FaceList[i];
        if( i + 1 > 9 )
        {
            sprintf( resBuf, ":/AppRes/Resources/emoj%d.svg", i + 1 );
        }
        else
        {
            sprintf( resBuf, ":/AppRes/Resources/emoj0%d.svg", i + 1 );
        }

		QPixmap faceImage( resBuf );
		QPixmap picPixmap = faceImage.scaled(imageSize, Qt::KeepAspectRatio, Qt::SmoothTransformation );
		faceLabel->setPixmap( picPixmap );
	}

	ui.m_CancelFaceButton->setIcons( eMyIconCancelNormal );
	connect( ui.m_CancelFaceButton,		SIGNAL(clicked()),							this, SLOT(slotCancelFaceButtonClicked()) );
}

//============================================================================
QSize InputFaceWidget::sizeHint() const
{
	QSize thisSizeHint( ( int )( GuiParams::getGuiScale() * 228 ), 
                        ( int )( 347 * GuiParams::getGuiScale() ) );
	return thisSizeHint;
}

//============================================================================
void InputFaceWidget::slotCancelFaceButtonClicked( void )
{
	emit signalInputCompleted();
}

//============================================================================
void InputFaceWidget::faceLabelClicked( int faceNum )
{
	char assetBuf[128];
    if( faceNum > 9 )
    {
        sprintf( assetBuf, "emoj%d", faceNum );
    }
    else
    {
        sprintf( assetBuf, "emoj0%d", faceNum );
    }

	m_AssetInfo.setAssetName( assetBuf );
	m_AssetInfo.generateNewUniqueId();
	m_AssetInfo.setAssetLength( 0 );
	m_TimeRecCurrent	= GetTimeStampMs();
	m_AssetInfo.setCreationTime( m_TimeRecCurrent );
	m_MyApp.getEngine().fromGuiAssetAction( m_IsPersonalRecorder ? eAssetActionAddToAssetMgr : eAssetActionAddAssetAndSend,  m_AssetInfo );

	emit signalInputCompleted();
}

//============================================================================
void InputFaceWidget::slotFace1LabelClicked( void ) { faceLabelClicked( 1 ); }
void InputFaceWidget::slotFace2LabelClicked( void ) { faceLabelClicked( 2 ); }
void InputFaceWidget::slotFace3LabelClicked( void ) { faceLabelClicked( 3 ); }
void InputFaceWidget::slotFace4LabelClicked( void ) { faceLabelClicked( 4 ); }
void InputFaceWidget::slotFace5LabelClicked( void ) { faceLabelClicked( 5 ); }
void InputFaceWidget::slotFace6LabelClicked( void ) { faceLabelClicked( 6 ); }
void InputFaceWidget::slotFace7LabelClicked( void ) { faceLabelClicked( 7 ); }
void InputFaceWidget::slotFace8LabelClicked( void ) { faceLabelClicked( 8 ); }
void InputFaceWidget::slotFace9LabelClicked( void ) { faceLabelClicked( 9 ); }
void InputFaceWidget::slotFace10LabelClicked( void ) { faceLabelClicked( 10 ); }
void InputFaceWidget::slotFace11LabelClicked( void ) { faceLabelClicked( 11 ); }
void InputFaceWidget::slotFace12LabelClicked( void ) { faceLabelClicked( 12 ); }
void InputFaceWidget::slotFace13LabelClicked( void ) { faceLabelClicked( 13 ); }
void InputFaceWidget::slotFace14LabelClicked( void ) { faceLabelClicked( 14 ); }
void InputFaceWidget::slotFace15LabelClicked( void ) { faceLabelClicked( 15 ); }
void InputFaceWidget::slotFace16LabelClicked( void ) { faceLabelClicked( 16 ); }
void InputFaceWidget::slotFace17LabelClicked( void ) { faceLabelClicked( 17 ); }
void InputFaceWidget::slotFace18LabelClicked( void ) { faceLabelClicked( 18 ); }
void InputFaceWidget::slotFace19LabelClicked( void ) { faceLabelClicked( 19 ); }
void InputFaceWidget::slotFace20LabelClicked( void ) { faceLabelClicked( 20 ); }
void InputFaceWidget::slotFace21LabelClicked( void ) { faceLabelClicked( 21 ); }
void InputFaceWidget::slotFace22LabelClicked( void ) { faceLabelClicked( 22 ); }
void InputFaceWidget::slotFace23LabelClicked( void ) { faceLabelClicked( 23 ); }
void InputFaceWidget::slotFace24LabelClicked( void ) { faceLabelClicked( 24 ); }
void InputFaceWidget::slotFace25LabelClicked( void ) { faceLabelClicked( 25 ); }
void InputFaceWidget::slotFace26LabelClicked( void ) { faceLabelClicked( 26 ); }
void InputFaceWidget::slotFace27LabelClicked( void ) { faceLabelClicked( 27 ); }
void InputFaceWidget::slotFace28LabelClicked( void ) { faceLabelClicked( 28 ); }
void InputFaceWidget::slotFace29LabelClicked( void ) { faceLabelClicked( 29 ); }
void InputFaceWidget::slotFace30LabelClicked( void ) { faceLabelClicked( 30 ); }
void InputFaceWidget::slotFace31LabelClicked( void ) { faceLabelClicked( 31 ); }
void InputFaceWidget::slotFace32LabelClicked( void ) { faceLabelClicked( 32 ); }

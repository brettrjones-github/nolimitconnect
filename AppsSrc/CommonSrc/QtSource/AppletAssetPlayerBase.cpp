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

#include "AppletAssetPlayerBase.h"
#include "AppCommon.h"

#include <CoreLib/VxGlobals.h>
#include <ptop_src/ptop_engine_src/AssetMgr/AssetMgr.h>

#include <QProgressBar>

//============================================================================
AppletAssetPlayerBase::AppletAssetPlayerBase( const char* ObjName, AppCommon& appCommon, QWidget *parent)
: AppletBase( ObjName, appCommon, parent )
, m_Engine( appCommon.getEngine() )
{
	connect( this, SIGNAL(signalToGuiAssetAction(EAssetAction, int)), this, SLOT(slotToGuiAssetAction(EAssetAction, int)) );
}

//============================================================================
AppletAssetPlayerBase::~AppletAssetPlayerBase()
{
	if( m_CallbacksRequested && ( false == VxIsAppShuttingDown() ) )
	{
		m_MyApp.wantToGuiActivityCallbacks( this, this, false );
		m_CallbacksRequested = false;
	}
}

//============================================================================
MyIcons& AppletAssetPlayerBase::getMyIcons( void )
{
	return m_MyApp.getMyIcons();
}

//============================================================================
void AppletAssetPlayerBase::setXferBar( QProgressBar * xferProgressBar )	
{ 
	m_XferProgressBar = xferProgressBar; 
	if( 0 != m_XferProgressBar )
	{
		m_XferProgressBar->setVisible( m_ProgressBarShouldBeVisible );
		m_ProgressBarIsVisible = m_ProgressBarShouldBeVisible;
	}
}

//============================================================================
void AppletAssetPlayerBase::showEvent(QShowEvent * showEvent)
{
	AppletBase::showEvent(showEvent);
	updateWantCallbacks( true );
}

//============================================================================
void AppletAssetPlayerBase::updateWantCallbacks( bool wantCallbacks )
{
	if( wantCallbacks )
	{
		if( (false == VxIsAppShuttingDown())
			&& isAssetInfoSet()
			&& m_AssetInfo.isValid()
			&& (false == m_CallbacksRequested) )
		{
			m_CallbacksRequested = true;
			m_MyApp.wantToGuiActivityCallbacks( this, this, true );
		}

		updateProgressBarVisibility();
	}
	else
	{
		if( m_CallbacksRequested && (false == VxIsAppShuttingDown()) )
		{
			m_MyApp.wantToGuiActivityCallbacks( this, this, false );
			m_CallbacksRequested = false;
		}
	}
}

//============================================================================
void AppletAssetPlayerBase::hideEvent( QHideEvent * ev )
{
	updateWantCallbacks( false );

	AppletBase::hideEvent( ev );
}

//============================================================================
void AppletAssetPlayerBase::toGuiClientAssetAction( void * userData, EAssetAction assetAction, VxGUID& assetId, int pos0to100000 )
{
	if( assetId == m_AssetInfo.getAssetUniqueId() )
	{
		if( eAssetActionTxError == assetAction )
		{
			LogMsg( LOG_ERROR, "toGuiClientAssetAction txError %s\n ", m_AssetInfo.getAssetUniqueId().toHexString().c_str() );
		}

		emit signalToGuiAssetAction( assetAction, pos0to100000 );
	}
}

//============================================================================
void AppletAssetPlayerBase::slotToGuiAssetAction( EAssetAction assetAction, int pos0to100000 )
{
	bool needUpdate = false;
	switch( assetAction )
	{
	case eAssetActionTxProgress:
		if( eAssetSendStateTxProgress != m_AssetInfo.getAssetSendState() )
		{
			m_AssetInfo.setAssetSendState( eAssetSendStateTxProgress );
			needUpdate = true;
		}

		m_ProgressBarShouldBeVisible = true;
		if( m_AssetSendProgress != pos0to100000 )
		{
			m_AssetSendProgress = pos0to100000;
			needUpdate = true;
		}

		break;

	case eAssetActionRxProgress:
		if( eAssetSendStateRxProgress != m_AssetInfo.getAssetSendState() )
		{
			m_AssetInfo.setAssetSendState( eAssetSendStateRxProgress );
			needUpdate = true;
		}

		m_ProgressBarShouldBeVisible = true;
		if( m_AssetSendProgress != pos0to100000 )
		{
			m_AssetSendProgress = pos0to100000;
			needUpdate = true;
		}

		break;

	case eAssetActionTxBegin:
		m_ProgressBarShouldBeVisible = true;
		//if( eAssetSendStateTxProgress != m_AssetInfo.getAssetSendState() )
		{
			m_AssetSendProgress = 0;
			m_AssetInfo.setAssetSendState( eAssetSendStateTxProgress );
			needUpdate = true;
		}

		break;

	case eAssetActionRxBegin:
		m_ProgressBarShouldBeVisible = true;
		if( eAssetSendStateRxProgress != m_AssetInfo.getAssetSendState() )
		{
			m_AssetInfo.setAssetSendState(  eAssetSendStateRxProgress );
			needUpdate = true;
		}
		break;

	case eAssetActionTxSuccess:
		m_ProgressBarShouldBeVisible = false;
		m_AssetSendProgress = 100;
		if( eAssetSendStateTxSuccess != m_AssetInfo.getAssetSendState() )
		{
			m_AssetInfo.setAssetSendState(  eAssetSendStateTxSuccess );
			needUpdate = true;
		}

		break;

	case eAssetActionRxSuccess:
		m_ProgressBarShouldBeVisible = false;
		if( eAssetSendStateRxSuccess != m_AssetInfo.getAssetSendState() )
		{
			m_AssetInfo.setAssetSendState(  eAssetSendStateRxSuccess );
			needUpdate = true;
		}

		break;

	case eAssetActionTxError:
	case eAssetActionTxCancel:
		m_ProgressBarShouldBeVisible = true;
		if( eAssetSendStateTxFail != m_AssetInfo.getAssetSendState() )
		{
			m_AssetInfo.setAssetSendState( eAssetSendStateTxFail );
			needUpdate = true;
		}
		break;

	case eAssetActionRxError:
	case eAssetActionRxCancel:
		m_ProgressBarShouldBeVisible = true;
		if( eAssetSendStateRxFail != m_AssetInfo.getAssetSendState() )
		{
			m_AssetInfo.setAssetSendState( eAssetSendStateRxFail );
			needUpdate = true;
		}
		break;		

	default:
		m_ProgressBarShouldBeVisible = false;
		//LogMsg( LOG_ERROR, "Unknown Asset action %d\n", assetAction );
		break;
	}				

	if( needUpdate )
	{
		updateFromAssetInfo();
	}

	updateProgressBarVisibility();
}

//============================================================================
void AppletAssetPlayerBase::updateProgressBarVisibility( void )
{
	if( 0 != m_XferProgressBar )
	{
		if( ( m_ProgressBarShouldBeVisible != m_ProgressBarIsVisible )
			&& ( 0 != m_XferProgressBar ) )
		{
			m_XferProgressBar->setVisible( m_ProgressBarShouldBeVisible );
			m_ProgressBarIsVisible = m_ProgressBarShouldBeVisible;
		}
	}
}

//============================================================================
void AppletAssetPlayerBase::slotShredAsset( void )
{
	// don't do this.. history list will call it
	//this->m_Engine.fromGuiAssetAction( eAssetActionShreadFile, m_AssetInfo.getAssetUniqueId(), 0 );
	emit signalShreddingAsset( this );
}

//============================================================================
void AppletAssetPlayerBase::slotResendAsset( void )
{
	m_Engine.fromGuiAssetAction( eAssetActionAssetResend, m_AssetInfo.getAssetUniqueId(), 0 );
	m_MyApp.playSound( eSndDefButtonClick );
}

//============================================================================
void AppletAssetPlayerBase::doShowProgress( bool showProgress )
{
	m_ProgressBarShouldBeVisible = showProgress;
	updateProgressBarVisibility();
	showXferProgress( showProgress );
}

//============================================================================
void AppletAssetPlayerBase::doSetProgress(  int xferProgress )
{
	if( 0 != m_XferProgressBar )
	{
		m_XferProgressBar->setValue( xferProgress );
	}

	setXferProgress( xferProgress );
}


//============================================================================
void AppletAssetPlayerBase::updateFromAssetInfo( void )
{
	switch( m_AssetInfo.getAssetSendState() )
	{
	case eAssetSendStateTxProgress:
	case eAssetSendStateRxProgress:
		showSendFail( false );
		showResendButton( false );
		doShowProgress( true );
		showShredder( false );
		doSetProgress( m_AssetSendProgress );
		break;

	case eAssetSendStateTxSuccess:
	case eAssetSendStateRxSuccess:
		showSendFail( false );
		showResendButton( false );
		doShowProgress( false );
		showShredder( true );
		break;

	case eAssetSendStateTxFail:
		showSendFail( true );
		showResendButton( true );
		doShowProgress( false );
		showShredder( true );
		break;

	case eAssetSendStateTxPermissionErr:
		showSendFail( true, true );
		showResendButton( false );
		doShowProgress( true );
		showShredder( true );
		break;

	case eAssetSendStateRxPermissionErr:
		showSendFail( true, true );
		showResendButton( false );
		doShowProgress( false );
		showShredder( true );
		break;

	case eAssetSendStateRxFail:
		showSendFail( true );
		showResendButton( false );
		doShowProgress( false );
		showShredder( true );
		break;

	case eAssetSendStateNone:
	default:
		showSendFail( false );
		showResendButton( false );
		doShowProgress( false );
		showShredder( true );
		break;

	}
}


//============================================================================
void AppletAssetPlayerBase::setPlayerAssetId( VxGUID& feedId )
{
	if( feedId.isVxGUIDValid() )
	{
		AssetBaseInfo* baseInfo = m_Engine.getAssetMgr().findAsset( feedId );
		if( baseInfo )
		{
			setAssetInfo( *baseInfo );
			updateWantCallbacks( true );
		}
		else
		{
			LogMsg( LOG_ERROR, "AppletAssetPlayerBase::setPlayerAssetId asset Not Found %s ", feedId.toHexString().c_str() );
		}
	}
	else
	{
		LogMsg( LOG_ERROR, "AppletAssetPlayerBase::setPlayerAssetId invalid feed id " );
	}
}
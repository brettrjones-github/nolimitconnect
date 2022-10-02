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

#include "InputBaseWidget.h"
#include "AppCommon.h"

#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxTime.h>
#include <CoreLib/VxTimeUtil.h>

#include <MediaToolsLib/OpusAudioDecoder.h>

#include <QTimer>

//============================================================================
InputBaseWidget::InputBaseWidget( AppCommon& app, QWidget* parent )
: QWidget( parent )
, m_MyApp( app )
, m_ChatEntryWidget( (ChatEntryWidget *)parent )
, m_MyIdent( 0 )
, m_HisIdent( 0 )
, m_TimeRecStart( 0 )
, m_TimeRecCurrent( 0 )
, m_ElapseTimer( new QTimer( this ) )
, m_IsPersonalRecorder( false )
{
	connect( this,			SIGNAL(signalChatMessage(QString)), this, SLOT(slotChatMessage(QString)) );
	connect( m_ElapseTimer, SIGNAL(timeout()),					this, SLOT(slotElapsedTimerTimeout()) );
}

//============================================================================
MyIcons&  InputBaseWidget::getMyIcons( void )
{
	return m_MyApp.getMyIcons();
}

//============================================================================
void InputBaseWidget::setIdents( GuiUser* myIdent, GuiUser* hisIdent )		
{ 
	m_MyIdent = myIdent; 
	m_HisIdent = hisIdent; 
	m_AssetInfo.setCreatorId( myIdent->getMyOnlineId() );
	m_AssetInfo.setHistoryId( hisIdent->getMyOnlineId() );
}

//============================================================================
void InputBaseWidget::setIsPersonalRecorder( bool isPersonal )
{
	m_IsPersonalRecorder = isPersonal;
	m_AssetInfo.setIsPersonalRecord( isPersonal );
}

//============================================================================
void InputBaseWidget::setIsChatRoom( bool isChatRoom )
{
    m_IsPersonalRecorder = false;
    m_AssetInfo.setIsChatRoomRecord( isChatRoom );
}

//============================================================================
void InputBaseWidget::slotChatMessage( QString chatMsg )
{
}

//============================================================================
bool InputBaseWidget::voiceRecord( EAssetAction action )
{
	EAssetType assetType = eAssetTypeAudio;
	bool actionResult = true;
    switch( action )
    {
    case eAssetActionRecordBegin:
        m_AssetInfo.generateNewUniqueId();
        if( generateFileName( assetType, m_AssetInfo.getAssetUniqueId() ) )
        {
            m_AssetInfo.setAssetName( m_FileName );
            actionResult = m_MyApp.getEngine().fromGuiSndRecord( eSndRecordStateStartRecording, m_AssetInfo.getCreatorId(), m_FileName.c_str() );
            if( true == actionResult )
            {
                m_TimeRecStart = GetTimeStampMs();
                m_TimeRecCurrent = m_TimeRecStart;
                m_AssetInfo.setCreationTime( m_TimeRecCurrent );
                m_ElapseTimer->start( 400 );
            }
            else
            {
                LogMsg( LOG_ERROR, "Could Not start Audio Record\n" );
            }
        }

		break;

	case eAssetActionRecordEnd:
		m_ElapseTimer->stop();
		updateElapsedTime();
		if( 0 == ( m_TimeRecCurrent - m_TimeRecStart ) )
		{
			// if only pressed for a second then ignore
			m_MyApp.getEngine().fromGuiSndRecord( eSndRecordStateCancelRecording, m_AssetInfo.getCreatorId(), m_FileName.c_str() );
			actionResult = false;
		}
		else
		{
			m_MyApp.getEngine().fromGuiSndRecord( eSndRecordStateStopRecording, m_AssetInfo.getCreatorId(), m_FileName.c_str() );
			uint64_t fileLen = VxFileUtil::getFileLen( m_FileName.c_str() );
			m_AssetInfo.setAssetLength( fileLen );
			if( MIN_OPUS_FILE_LEN < m_AssetInfo.getAssetLength() )
			{
				m_MyApp.getEngine().fromGuiAssetAction( m_IsPersonalRecorder ? eAssetActionAddToAssetMgr : eAssetActionAddAssetAndSend,  m_AssetInfo );
				actionResult = true;
			}
			else
			{
				LogMsg( LOG_ERROR, "InputBaseWidget::voiceRecord file len %d too short %s\n", (uint32_t)m_AssetInfo.getAssetLength(), m_FileName.c_str() );
				m_MyApp.toGuiUserMessage( "Voice Record file len %d too short %s\n", (uint32_t)m_AssetInfo.getAssetLength(), m_FileName.c_str() );
				VxFileUtil::deleteFile( m_FileName.c_str() );
				actionResult = false;
			}
		}

		break;

	case eAssetActionRecordCancel:
		m_ElapseTimer->stop();
		m_MyApp.getEngine().fromGuiSndRecord( eSndRecordStateCancelRecording, m_AssetInfo.getCreatorId(), m_FileName.c_str() );	
		actionResult = true;
		break;

    default:
        break;
	}

	return actionResult;
}

//============================================================================
bool InputBaseWidget::videoRecord( EAssetAction action )
{
	EAssetType assetType = eAssetTypeVideo;
	bool actionResult = true;
	switch( action )
	{
	case eAssetActionRecordBegin:
		m_AssetInfo.generateNewUniqueId();
        if( generateFileName( assetType, m_AssetInfo.getAssetUniqueId() ) )
        {
		    m_AssetInfo.setAssetName( m_FileName );
		    actionResult = m_MyApp.getEngine().fromGuiVideoRecord( eVideoRecordStateStartRecording, m_AssetInfo.getCreatorId(), m_FileName.c_str() );
		    if( true == actionResult )
		    {
			    m_TimeRecStart		= GetTimeStampMs();
			    m_TimeRecCurrent	= m_TimeRecStart;
			    m_AssetInfo.setCreationTime( m_TimeRecCurrent );
			    m_ElapseTimer->start( 400 );
		    }
		    else
		    {
			    LogMsg( LOG_ERROR, "Could Not start Video Record\n" );
		    }
        }

		break;

	case eAssetActionRecordEnd:
		{
			m_ElapseTimer->stop();
			updateElapsedTime();
			if( 0 == ( m_TimeRecCurrent - m_TimeRecStart ) )
			{
				// if only pressed for a second then ignore
				m_MyApp.getEngine().fromGuiVideoRecord( eVideoRecordStateCancelRecording, m_AssetInfo.getCreatorId(), m_FileName.c_str() );	
				actionResult = false;
			}
			else
			{
				m_MyApp.getEngine().fromGuiVideoRecord( eVideoRecordStateStopRecording, m_AssetInfo.getCreatorId(), m_FileName.c_str() );
				uint64_t fileLen = VxFileUtil::getFileLen( m_FileName.c_str() );
				if( 5000 > fileLen )
				{
					// not long enough to be a recording
					VxFileUtil::deleteFile( m_FileName.c_str() );
					LogMsg( LOG_ERROR, "InputBaseWidget::videoRecord file %s has to short len %" PRId64 "", m_FileName.c_str(), fileLen );
					actionResult = false;
				}
				else
				{
					m_AssetInfo.setAssetLength( fileLen );
					m_MyApp.getEngine().fromGuiAssetAction( m_IsPersonalRecorder ? eAssetActionAddToAssetMgr : eAssetActionAddAssetAndSend,  m_AssetInfo );
					actionResult = true;
				}
			}
		}

		break;

	case eAssetActionRecordCancel:
		m_ElapseTimer->stop();
		m_MyApp.getEngine().fromGuiVideoRecord( eVideoRecordStateCancelRecording, m_AssetInfo.getCreatorId(), m_FileName.c_str() );	
		actionResult = true;
		break;

    default:
        break;
	}

	return actionResult;
}

//============================================================================
void InputBaseWidget::slotElapsedTimerTimeout( void )
{
	updateElapsedTime();
}

//============================================================================
void InputBaseWidget::updateElapsedTime( void )
{
	m_TimeRecCurrent = GetTimeStampMs();
	time_t timeDif = m_TimeRecCurrent - m_TimeRecStart;
	std::string elapsedTime = VxTimeUtil::formatTimeDiffIntoMinutesAndSeconds( timeDif );
	emit signalElapsedRecTime( elapsedTime.c_str() );
}

//============================================================================
bool InputBaseWidget::generateFileName( EAssetType assetType, VxGUID& uniqueId )
{
	//std::string hisOnlineId;
	//m_HisIdent->getMyOnlineId().toHexString( hisOnlineId );
	//m_FileName = VxGetUserXferDirectory() + hisOnlineId + "/" + AssetInfo::getSubDirectoryName( assetType );
    bool result = false;
    if( m_HisIdent )
    {
        std::string mediaSubDir = "contacts/";
        if( m_HisIdent->getMyOnlineId() == m_MyIdent->getMyOnlineId() )
        {
            mediaSubDir = "me/";
        }

        m_FileName = VxGetUserXferDirectory() + mediaSubDir;
        VxFileUtil::makeDirectory( m_FileName );
        //m_FileName += AssetInfo::getSubDirectoryName( assetType );
        //VxFileUtil::makeDirectory( m_FileName );
        m_FileName += m_MyIdent->getOnlineName();
        m_FileName += "_";
        m_FileName += VxTimeUtil::getFileNameCompatibleDateAndTime( GetLocalTimeMs() );
        m_FileName += "#";
        m_FileName += uniqueId.toHexString();
        m_FileName += AssetInfo::getDefaultFileExtension( assetType );
        result = true;
    }
    else
    {
        LogMsg( LOG_ERROR, "InputBaseWidget::generateFileName m_HisIdent is null" );
    }

    return result;
}

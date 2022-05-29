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

#include <QWidget> // must be declared first or linux Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include "TodGameDefs.h"
#include <PktLib/VxCommon.h>

class AppCommon;
class QFrame;
class VxPushButton;
class QLabel;
class TodGameWidget;
class VidWidget;
class EngineSettings;
class P2PEngine;

class TodGameLogic : public QWidget
{
	Q_OBJECT

public:
	TodGameLogic( AppCommon& myApp, P2PEngine& engine, EPluginType pluginType, QWidget * parent );

	void						setGuiWidgets(	GuiUser *	hisIdent,
												TodGameWidget *	todGameWidget );
	virtual void				setVisible( bool visible );

	bool						beginGame( bool isChallenger );
	void						endGame( void );

	void						setGameValueVar( long s32VarId, long s32VarValue );
	void						setGameActionVar( long s32VarId, long s32VarValue );

	void						showChallengeStatus( bool showChallenge );
	void						setChallengeStatus( QString statusIcon, QString statusText );

	void						setGameStatus( EGameStatus eGameStatus );

signals:
	void						signalUserWentOffline( void );

protected slots:
	void						slotToGuiSetGameValueVar( long s32VarId, long s32VarValue );
	void						slotToGuiSetGameActionVar( long s32ActionId, long s32VarValue );

	void						slotTruthButtonClicked( void );
	void						slotDareButtonClicked( void );

protected:
	bool						sendGameStats( void );
	void						enableGameButton( EGameButton eButton, bool bEnable );
	void						setGameButtonText( EGameButton eButton, QString strText );

	void						loadMyGameStats( void );
	void						saveMyGameStats( void );
	void						updateMyStats( void );
	void						updateFriendStats( void );
	void						setTodStatusMsg( QString strStatus );
	bool						fromGuiSetGameValueVar(	int32_t	s32VarId, int32_t s32VarValue );
	bool						fromGuiSetGameActionVar(int32_t	s32VarId, int32_t s32VarValue );
	void						handleUserWentOffline( void );

	AppCommon& 				    m_MyApp;
	P2PEngine&					m_Engine; 
	EPluginType					m_ePluginType;
	EngineSettings&				m_MySettings;
    GuiUser *				    m_HisIdent;
	TodGameWidget *				m_TodGameWidget;
	TodGameStats				m_TodGameStats;
	EGameStatus					m_eGameStatus;
	bool						m_IsChallenger;
};

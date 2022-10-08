#pragma once
//============================================================================
// Copyright (C) 2021 Brett R. Jones
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

#include "AppletBase.h"
#include "ui_AppletHostSelect.h"

class GuiHostJoin;
class GuiHostJoinMgr;
class GroupieId;

class AppletHostSelect : public AppletBase
{
	Q_OBJECT
public:
	AppletHostSelect( AppCommon& app, QWidget* parent = nullptr );
	virtual ~AppletHostSelect() override;

signals:
	void                        signalGroupHostSelected( QString hostUrl );

protected slots:
	void				        slotHostJoinRequested( GuiHostJoin* guiHostJoin );
	void				        slotlHostJoinUpdated( GuiHostJoin* guiHostJoin );
	void				        slotHostJoinRemoved( GroupieId& groupieId );
	void                        slotHostJoinOfferStateChange( GroupieId& groupieId, EJoinState hostOfferState );
	void                        slotHostJoinOnlineStatus( GuiHostJoin* user, bool isOnline );

	void                        slotAcceptButtonClicked( GuiHostJoinSession* joinSession, HostJoinRequestListItem* joinItem );
	void                        slotRejectButtonClicked( GuiHostJoinSession* joinSession, HostJoinRequestListItem* joinItem );

	void                        slotJoinComboBoxSelectionChange( int comboIdx );
	void                        slotCreateInviteButtonClicked( void );

protected:
	void						updateJoinList( void );
	void						updateHostJoinRequest( GuiHostJoin* guiHostJoin );

	//=== vars ===//
	Ui::AppletHostSelectUi		ui;
	GuiHostJoinMgr&				m_HostJoinMgr;
	EHostType					m_HostType{ eHostTypeGroup };
	EJoinState					m_JoinState{ eJoinStateJoinRequested };
};

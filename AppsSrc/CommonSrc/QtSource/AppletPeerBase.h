#pragma once
//============================================================================
// Copyright (C) 2013 Brett R. Jones
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

#include "AppletBase.h"

class GuiOfferSession;
class GuiUser;
class VidWidget;
class VxLabel;
class VxPushButton;

class AppletPeerBase : public AppletBase
{
	Q_OBJECT
public:
	AppletPeerBase( const char * objName, AppCommon& app, QWidget * parent );
	virtual ~AppletPeerBase();

    virtual void				setPluginType( EPluginType ePluginType ) override   { m_ePluginType = ePluginType; m_OfferSessionLogic.setPluginType( ePluginType ); }
    virtual void                setGuiOfferSession( GuiOfferSession* offerSession ) { m_OfferSessionLogic.setGuiOfferSession( offerSession ); }

	void						setupBaseWidgets(	IdentWidget *		friendIdentWidget = 0, 
													VxPushButton *		permissionButton = 0, 
													QLabel *			permissionLabel = 0 );

	void						setVidCamWidget( VidWidget * camWidget )			{ m_VidCamWidget = camWidget; }
	VidWidget *					getVidCamWidget( void )								{ return m_VidCamWidget; }

	virtual bool				fromGuiSetGameValueVar(	int32_t varId, int32_t varValue );
	virtual bool				fromGuiSetGameActionVar( int32_t actionId, int32_t actionValue );

    virtual void				onActivityFinish( void ) override;

protected slots:
    void				        slotUserAdded( GuiUser* user ); 
    void				        slotUserRemoved( VxGUID onlineId ); 
    void                        slotUserUpdated( GuiUser* user );
    void                        slotUserOnlineStatus( GuiUser* user );

private:
	void						setupAppletPeerBase( void );

protected:
    virtual void				toGuiRxedPluginOffer( GuiOfferSession * offer );
    virtual void				toGuiRxedOfferReply( GuiOfferSession * offerReply );
    virtual void				toGuiPluginSessionEnded( GuiOfferSession * offer ) override;
	virtual void				toGuiClientPlayVideoFrame(	VxGUID&			onlineId, 
                                                            uint8_t *		pu8Jpg,
                                                            uint32_t		u32JpgDataLen,
                                                            int				motion0To100000 ) override;

    virtual void				toGuiSetGameValueVar(	EPluginType     ePluginType,
                                                        VxGUID&         onlineId,
														int32_t			s32VarId, 
                                                        int32_t			s32VarValue ) override;

    virtual void				toGuiSetGameActionVar(	EPluginType     ePluginType,
                                                        VxGUID&         onlineId,
														int32_t			s32VarId, 
                                                        int32_t			s32VarValue ) override;

    virtual void				showEvent( QShowEvent * ev ) override;
    virtual void				hideEvent( QHideEvent * ev ) override;
    virtual void				closeEvent( QCloseEvent * ev ) override;

	//=== vars ===//
    bool						m_bFirstMsg{false};
    VidWidget *					m_VidCamWidget{nullptr};
	OfferSessionLogic			m_OfferSessionLogic;
};

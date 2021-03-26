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

signals:
	void						signalToGuiSetGameValueVar( long varId, long varValue );
	void						signalToGuiSetGameActionVar( long actionId, long actionValue );
	void						signalToGuiRxedPluginOffer( GuiOfferSession * offerSession );
	void						signalToGuiRxedOfferReply( GuiOfferSession * offerSession );
	void						signalToGuiPluginSessionEnded( GuiOfferSession * offerSession );
	void						signalToGuiContactOnline( VxNetIdent * hisIdent, bool newContact );
	void						signalToGuiContactOffline( VxNetIdent * hisIdent );

protected slots:
	void						slotToGuiRxedPluginOffer( GuiOfferSession * offerSession );
	void						slotToGuiRxedOfferReply( GuiOfferSession * offerSession );
	void						slotToGuiPluginSessionEnded( GuiOfferSession * offerSession );
	void						slotToGuiContactOffline( VxNetIdent * hisIdent );

private:
	void						setupAppletPeerBase( void );

protected:
    virtual void				toGuiRxedPluginOffer( void * callbackData, GuiOfferSession * offer );
    virtual void				toGuiRxedOfferReply( void * callbackData, GuiOfferSession * offerReply );
    virtual void				toGuiPluginSessionEnded(void * callbackData, GuiOfferSession * offer ) override;
    virtual void				toGuiContactOnline( void * callbackData, VxNetIdent * friendIdent, bool newContact ) override;
    virtual void				toGuiContactOffline( void * callbackData, VxNetIdent * friendIdent ) override;
	virtual void				toGuiClientPlayVideoFrame(	void *			userData, 
															VxGUID&			onlineId, 
                                                            uint8_t *		pu8Jpg,
                                                            uint32_t		u32JpgDataLen,
                                                            int				motion0To100000 ) override;

    virtual void				toGuiSetGameValueVar(	void *          userData,
                                                        EPluginType     ePluginType,
                                                        VxGUID&         onlineId,
														int32_t			s32VarId, 
                                                        int32_t			s32VarValue ) override;

    virtual void				toGuiSetGameActionVar(	void *          userData,
                                                        EPluginType     ePluginType,
                                                        VxGUID&         onlineId,
														int32_t			s32VarId, 
                                                        int32_t			s32VarValue ) override;

    virtual void				showEvent( QShowEvent * ev ) override;
    virtual void				hideEvent( QHideEvent * ev ) override;
    virtual void				closeEvent( QCloseEvent * ev ) override;

	QString						describePluginOffer( EPluginType ePluginType );

	//=== vars ===//
    bool						m_bFirstMsg{false};
    VidWidget *					m_VidCamWidget{nullptr};
	OfferSessionLogic			m_OfferSessionLogic;
};

#pragma once
//============================================================================
// Copyright (C) 2018 Brett R. Jones
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
#include "GuiWebPageCallback.h"

#include "ui_AppletAboutMeClient.h"

class AppletAboutMeClient : public AppletBase, public GuiWebPageCallback
{
	Q_OBJECT
public:
	AppletAboutMeClient( AppCommon& app, QWidget* parent );
	virtual ~AppletAboutMeClient();

    void                        setIdentity( GuiUser* guiUser );

protected:
    void                        loadRichTextFile( QString fileName );
    virtual void				toGuiPluginMsg( EPluginType pluginType, VxGUID& onlineId, EPluginMsgType msgType, QString& paramValue ) override;

    //=== vars ===//
    Ui::AppletAboutMeClientUi	ui;
    VxNetIdent *				m_MyIdent = nullptr;
    QString                     m_strOrigOnlineName;
    QString                     m_strOrigMoodMessage;

    std::string					m_strDefaultPicPath;
    std::string					m_strUserSepecificDataDir;
    bool						m_bUserPickedImage = false;
    bool						m_bUsingDefaultImage = true;
    bool 					    m_CameraSourceAvail{ false };
    VxGUID                      m_HisOnlineId;
};



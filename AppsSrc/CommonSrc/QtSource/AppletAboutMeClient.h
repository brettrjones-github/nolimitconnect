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
// http://www.nolimitconnect.com
//============================================================================

#include "AppletBase.h"
#include "UserProfile.h"

#include "ui_AppletAboutMeClient.h"

class AppletAboutMeClient : public AppletBase
{
	Q_OBJECT
public:
	AppletAboutMeClient( AppCommon& app, QWidget * parent );
	virtual ~AppletAboutMeClient();

public slots:
     //! browse for picture of me
    void						onBrowseButClick( void );


protected:
    void                        loadRichTextFile( QString fileName );

    //=== constants ===//
    static const int SELECT_IMAGE = 0; // selector for image gallery call
    static const int CAMERA_SNAPSHOT = 1; // selector for image gallery call

    //=== vars ===//
    Ui::AppletAboutMeClientUi	ui;
    UserProfile 				m_UserProfile;
    VxNetIdent *				m_MyIdent = nullptr;
    QString                     m_strOrigOnlineName;
    QString                     m_strOrigMoodMessage;

    std::string					m_strDefaultPicPath;
    std::string					m_strUserSepecificDataDir;
    bool						m_bUserPickedImage = false;
    bool						m_bUsingDefaultImage = true;
    bool 					    m_CameraSourceAvail{ false };
};



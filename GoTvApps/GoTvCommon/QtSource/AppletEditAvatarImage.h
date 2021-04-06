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

#include "ui_AppletEditAvatarImage.h"

class AssetMgr;

class AppletEditAvatarImage : public AppletBase
{
	Q_OBJECT
public:
    AppletEditAvatarImage( AppCommon& app, QWidget * parent );
	virtual ~AppletEditAvatarImage() = default;

public slots:
    void						onApplyButClick( void );

protected:

    //=== constants ===//
    static const int SELECT_IMAGE = 0; // selector for image gallery call
    static const int CAMERA_SNAPSHOT = 1; // selector for image gallery call

    //=== vars ===//
    Ui::AppletEditAvatarImageUi	ui;
    ThumbMgr&                   m_ThumbMgr;
    UserProfile 				m_UserProfile;
    VxNetIdent *				m_MyIdent = nullptr;
    QString                     m_strOrigOnlineName;
    QString                     m_strOrigMoodMessage;
};



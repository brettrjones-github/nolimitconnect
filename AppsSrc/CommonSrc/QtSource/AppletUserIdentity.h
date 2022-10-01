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
#include "UserProfile.h"

#include "ui_AppletUserIdentity.h"

class ThumbInfo;
class AppletUserIdentity : public AppletBase
{
	Q_OBJECT
public:
	AppletUserIdentity( AppCommon& app, QWidget* parent );
	virtual ~AppletUserIdentity() = default;

public slots:
    void						onApplyMoodMsgClick( void );
    void						onApplyAgeClick( void );
    void						onApplyContentClick( void );
    void						onApplyGenderClick( void );

    void						slotEditAvatarButClick( void );
    void						slotEditAboutMeButClick( void );
    void						slotViewAboutMeButClick( void );
    void                        slotEditStoryboardButClick( void );
    void                        slotViewStoryboardButClick( void );

    void						slotCreateNewAccount( void );
    void						slotDeleteAccount( void );

    void                        slotAccountSelectionChanged( int idx );
    void                        slotAvatarImageChanged( ThumbInfo* avatarThumb );
    void                        slotAvatarImageRemoved( void );

protected:

    void                        loadIdentity( VxNetIdent& ident );
    void                        saveIdentity( VxNetIdent& ident );
    void                        updateAvatarImage( VxGUID& thumbId, int64_t thumbModifiedTime );

    //! validate user input
    QString						validateString( QString charSeq );
    bool                        validateMoodMessage( void );
    void                        resetComboIdxToOriginalAccount( void );

    //=== constants ===//
    static const int SELECT_IMAGE = 0; // selector for image gallery call
    static const int CAMERA_SNAPSHOT = 1; // selector for image gallery call

    //=== vars ===//
    Ui::AppletUserIdentityUi	ui;
    std::vector<VxNetIdent>     m_AccountList;

    UserProfile 				m_UserProfile;
    VxNetIdent *				m_CurIdent = nullptr;
    QString                     m_strOrigOnlineName;
    QString                     m_strOrigMoodMessage;

    std::string					m_strDefaultPicPath;
    std::string					m_strUserSepecificDataDir;
    bool						m_bUserPickedImage = false;
    bool						m_bUsingDefaultImage = true;
};



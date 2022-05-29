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

#include "ui_AppletEditStoryBoard.h"
#include "ui_StoryWidget.h"

class AppletEditStoryboard : public AppletBase
{
	Q_OBJECT
public:
    AppletEditStoryboard( AppCommon& app, QWidget * parent );
	virtual ~AppletEditStoryboard() = default;

private slots:
    void						slotStoryBoardSavedModified();

protected:
    //=== vars ===//
    Ui::AppletEditStoryboardUi	ui;
    std::string					m_strSavedCwd;
    std::string					m_strStoryBoardDir;
    std::string					m_strStoryBoardFile;
};



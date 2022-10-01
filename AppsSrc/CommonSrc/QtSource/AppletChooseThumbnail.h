#pragma once
//============================================================================
// Copyright (C) 2020 Brett R. Jones
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

#include "ui_AppletChooseThumbnail.h"

class AppletChooseThumbnail : public AppletBase
{
	Q_OBJECT
public:
    AppletChooseThumbnail( AppCommon& app, QWidget* parent );
	virtual ~AppletChooseThumbnail();

signals:
    void						signalThumbSelected( AppletBase * baseApplet, ThumbnailEditWidget * widget );

private slots:
    void						slotImageChanged();

protected:
    //=== vars ===//
    Ui::AppletChooseThumbnailUi	ui;
    std::string					m_strSavedCwd;
    std::string					m_strStoryBoardDir;
    std::string					m_strStoryBoardFile;
};



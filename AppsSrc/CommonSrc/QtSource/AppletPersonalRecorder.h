#pragma once
//============================================================================
// Copyright (C) 2019 Brett R. Jones
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

#include "ui_AppletPersonalRecorder.h"

#include "AppletBase.h"

class AppletPersonalRecorder : public AppletBase
{
	Q_OBJECT
public:
	AppletPersonalRecorder( AppCommon& app, QWidget * parent );
	virtual ~AppletPersonalRecorder();


    virtual void				toGuiClientPlayVideoFrame( VxGUID&          onlineId,
                                                           uint8_t *		pu8Jpg,
                                                           uint32_t		    u32JpgDataLen,
                                                           int				motion0To100000 ) override;

protected:
    void						setupMultiSessionActivity( void );

    Ui::PersonalRecorderUi	    ui;
};



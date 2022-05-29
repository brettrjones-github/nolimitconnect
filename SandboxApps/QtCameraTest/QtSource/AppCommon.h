#pragma once
//============================================================================
// Copyright (C) 2020 Brett R. Jones 
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

#include "config_gotvapps.h"
#include "AppDefs.h"

#include "CamLogic.h"
#include "VxAppTheme.h"
#include "MyIcons.h"
#include "SoundDefs.h"

#include <GuiInterface/IToGui.h>

#include <QWidget>

class QApplication;
class AppSettings;
class IGoTv;
class P2PEngine;


class AppCommon : public QWidget, public IToGui
{
    Q_OBJECT

public:
    AppCommon( QApplication& myQApp, AppSettings& appSettings );
    virtual ~AppCommon() = default;

    AppSettings&				getAppSettings( void )						{ return m_AppSettings; }
    VxAppTheme&					getAppTheme( void )							{ return m_AppTheme; }
    MyIcons&					getMyIcons( void )							{ return m_MyIcons; }
    CamLogic&					getCamLogic( void )							{ return m_CamLogic; }
    P2PEngine&					getEngine( void )							{ return m_Engine; }

    void						playSound( ESndDef sndDef ){};

    virtual void				toGuiWantVideoCapture( bool wantVidCapture ) override;
    virtual void				toGuiPlayVideoFrame( VxGUID& onlineId, uint8_t * pu8Jpg, uint32_t u32JpgDataLen, int motion0To100000 ) override;
    virtual int				    toGuiPlayVideoFrame( VxGUID& onlineId, uint8_t * picBuf, uint32_t picBufLen, int picWidth, int picHeight ) override;

signals:
    void                        signalEnableVideoCapture( bool wantVidCapture );

protected slots:
    void                        slotEnableVideoCapture( bool enableVidCapture );

protected:
    QApplication&               m_QApp;
    AppSettings&				m_AppSettings;
    MyIcons					    m_MyIcons;
    VxAppTheme					m_AppTheme;
    CamLogic                    m_CamLogic;

    P2PEngine&					m_Engine;

private:
    AppCommon( const AppCommon& rhs ) = delete;
};

AppCommon& CreateAppCommonInstance( QApplication& myApp );

AppCommon& GetAppInstance( void );

void DestroyAppCommonInstance();

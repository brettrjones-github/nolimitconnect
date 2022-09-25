#pragma once
//============================================================================
// Copyright (C) 2022 Brett R. Jones 
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

#include <QWidget> // must be declared first or linux Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value �53� is outside the bounds

#include <GuiInterface/IDefs.h>

class AppCommon;

class GuiPluginMgr : public QObject
{
    Q_OBJECT
public:
    GuiPluginMgr() = delete;
    GuiPluginMgr( AppCommon& app );
    GuiPluginMgr( const GuiPluginMgr& rhs ) = delete;
    virtual ~GuiPluginMgr() = default;


    void						setPluginVisible( EPluginType ePluginType, bool isVisible );
    bool						getIsPluginVisible( EPluginType ePluginType );

    void						setIsCamServerEnabled( bool camServerIsEnabled )        { m_CamServerIsEnabled = camServerIsEnabled; }
    int						    getIsCamServerEnabled( void )                           { return m_CamServerIsEnabled; }
    void						setCamServerClientCount( int camServerClientCount )     { m_CamServerClientCount = camServerClientCount; }
    int						    getCamServerClientCount( void )                         { return m_CamServerClientCount; }

protected:
    AppCommon&                  m_MyApp;

    QVector<EPluginType>		m_VisiblePluginsList;

    bool                        m_CamServerIsEnabled{ false };
    int                         m_CamServerClientCount{ -1 };
};

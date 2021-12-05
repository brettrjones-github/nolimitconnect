#pragma once
//============================================================================
// Copyright (C) 2021 Brett R. Jones
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

#include <PktLib/VxCommon.h>
#include <CoreLib/VxGUID.h>

#include <QWidget>

class AppCommon;
class GuiThumbMgr;
class ThumbInfo;
class QImage;

class GuiThumb : public QWidget
{
public:
    GuiThumb() = delete;
    GuiThumb( AppCommon& app );
    GuiThumb( AppCommon& app, ThumbInfo* thumbInfo, VxGUID& sessionId );
    GuiThumb( const GuiThumb& rhs );
	virtual ~GuiThumb() = default;

    bool						operator == ( const GuiThumb& rhs ) const;
    bool						operator == ( GuiThumb& rhs );

    bool                        isEqualTo( GuiThumb* guiThumb );

    void                        setThumbInfo( ThumbInfo* thumbInfo );
    ThumbInfo*                  getThumbInfo( void )                    { return m_ThumbInfo; }
    bool                        isThumbValid( void )                    { return m_ThumbInfo && m_ThumbId.isVxGUIDValid(); }
    void                        setSessionId( VxGUID& sessionId )       { m_SessionId = sessionId; }
    VxGUID&                     getSessionId( void )                    { return m_SessionId; }

    VxGUID&                     getCreatorId( void )                    { return m_CreatorId; }
    VxGUID&                     getThumbId( void )                      { return m_ThumbId; }

    bool                        createImage( QImage& retAvatarImage );

protected:
    void                        updateThumbInfoIds( void );

    AppCommon&                  m_MyApp;
    GuiThumbMgr&                m_GuiThumbMgr;

    ThumbInfo*                  m_ThumbInfo{ nullptr };
    VxGUID                      m_SessionId;
    VxGUID                      m_ThumbId;
    VxGUID                      m_CreatorId;
};

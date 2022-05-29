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

#include <stdint.h>

class AppSettings
{
public:
    AppSettings();
    virtual ~AppSettings() = default;

    void						setCamSourceId( uint32_t camId )                        { m_CamId = camId; }
    uint32_t					getCamSourceId( void )                                  { return m_CamId; }

    void						setCamShowPreview( bool showPreview )                   { m_ShowPreview = showPreview; };
    bool						getCamShowPreview( void )                               { return m_ShowPreview; }

    void						setCamRotation( uint32_t camId, uint32_t camRotation )  { m_CamRotation = camRotation; };
    uint32_t					getCamRotation( uint32_t camId )                        { return m_CamRotation; }

    void						setVidFeedRotation( uint32_t feedRotation )             { m_FeedRotation = feedRotation; };
    uint32_t					getVidFeedRotation( void )                              { return m_FeedRotation; }

    uint32_t                    m_CamId{ 0 };
    bool                        m_ShowPreview;
    uint32_t                    m_CamRotation{ 0 };
    uint32_t                    m_FeedRotation{ 0 };
};



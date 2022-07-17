#pragma once

#include <vector>
#include <stdint.h>

#define MAX_YUV_PLANES 3
#define MAX_FRAME_PLANES 4


class INlc;

class NlcRenderFrame
{
public:
    NlcRenderFrame( ) = default;

    bool                        m_Inited; 

    unsigned int                m_Width; // frame width 
    unsigned int                m_Height; // frame height

    unsigned int                m_PlaneCount; 

    std::vector<uint8_t>        m_PlaneData[ MAX_FRAME_PLANES ]; 

    int                         m_Pitch[ MAX_FRAME_PLANES ]; 
    int                         m_VisiblePitch[ MAX_FRAME_PLANES ];
    int                         m_Lines[ MAX_FRAME_PLANES ];
    int                         m_VisibleLines[ MAX_FRAME_PLANES ];
};

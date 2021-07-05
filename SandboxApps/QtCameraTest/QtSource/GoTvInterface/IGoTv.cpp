#include "IGoTv.h"
#include "AppCommon.h"
#include "AppCommon.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>


//============================================================================
IGoTv::IGoTv()
{
}

//============================================================================
IGoTv::~IGoTv()
{
}

//============================================================================
IGoTv& IGoTv::getIGoTv()
{
    static IGoTv g_IGoTv;
    return g_IGoTv;
}

//============================================================================
IToGui& IToGui::getToGui()
{
    return IGoTv::getIGoTv();
}

//============================================================================
AppCommon& IGoTv::getAppCommon()
{
    return GetAppInstance();
}

//============================================================================
P2PEngine& IGoTv::getPtoP()
{
    return GetP2PEngineInstance();
}

//============================================================================
void IGoTv::toGuiWantVideoCapture( bool wantVidCapture )
{
    getAppCommon().toGuiWantVideoCapture( wantVidCapture );
}

//============================================================================
void IGoTv::toGuiPlayVideoFrame( VxGUID& onlineId, uint8_t * pu8Jpg, uint32_t u32JpgDataLen, int motion0to100000 )
{
    if( false == getPtoP().isAppPaused() )
    {
        getAppCommon().toGuiPlayVideoFrame( onlineId, pu8Jpg, u32JpgDataLen, motion0to100000 );
    }
}
//============================================================================
//! show jpeg for video playback
int IGoTv::toGuiPlayVideoFrame( VxGUID& onlineId, uint8_t * picBuf, uint32_t picBufLen, int picWidth, int picHeight )
{
    if( false == getPtoP().isAppPaused() )
    {
        return getAppCommon().toGuiPlayVideoFrame( onlineId, picBuf, picBufLen, picWidth, picHeight );
    }

    return 0;
}

#pragma once

#include "GuiInterface/IToGui.h"
#include <GuiInterface/IFromGui.h>

class AppCommon;
class P2PEngine;

class IGoTv : public IToGui
{
public:
    IGoTv();
    virtual  ~IGoTv();

    static IGoTv&               getIGoTv();
    AppCommon&                  getAppCommon();
    P2PEngine&                  getPtoP();


    virtual void				toGuiWantVideoCapture( bool wantVidCapture ) override;
    virtual void				toGuiPlayVideoFrame( VxGUID& onlineId, uint8_t * pu8Jpg, uint32_t u32JpgDataLen, int motion0To100000 ) override;
    virtual int				    toGuiPlayVideoFrame( VxGUID& onlineId, uint8_t * picBuf, uint32_t picBufLen, int picWidth, int picHeight ) override;

};

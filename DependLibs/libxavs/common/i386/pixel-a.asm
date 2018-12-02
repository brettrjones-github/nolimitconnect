;*****************************************************************************
;* pixel.asm: h264 encoder library
;*****************************************************************************
;* Copyright (C) 2003 xavs project
;* $Id: pixel.asm,v 1.1 2004/06/03 19:27:07 fenrir Exp $
;*
;* Authors: Laurent Aimar <fenrir@via.ecp.fr>
;*
;* This program is free software; you can redistribute it and/or modify
;* it under the terms of the GNU General Public License as published by
;* the Free Software Foundation; either version 2 of the License, or
;* (at your option) any later version.
;*
;* This program is distributed in the hope that it will be useful,
;* but WITHOUT ANY WARRANTY; without even the implied warranty of
;* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;* GNU General Public License for more details.
;*
;* You should have received a copy of the GNU General Public License
;* along with this program; if not, write to the Free Software
;* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111, USA.
;*****************************************************************************

BITS 32

;=============================================================================
; Macros and other preprocessor constants
;=============================================================================

%include "i386inc.asm"

; sad

%macro SAD_INC_2x16P 0
    movq    mm1,    [eax]
    movq    mm2,    [eax+8]
    movq    mm3,    [eax+ebx]
    movq    mm4,    [eax+ebx+8]
    psadbw  mm1,    [ecx]
    psadbw  mm2,    [ecx+8]
    psadbw  mm3,    [ecx+edx]
    psadbw  mm4,    [ecx+edx+8]
    lea     eax,    [eax+2*ebx]
    paddw   mm1,    mm2
    paddw   mm3,    mm4
    lea     ecx,    [ecx+2*edx]
    paddw   mm0,    mm1
    paddw   mm0,    mm3
%endmacro

%macro SAD_INC_2x8P 0
    movq    mm1,    [eax]
    movq    mm2,    [eax+ebx]
    psadbw  mm1,    [ecx]
    psadbw  mm2,    [ecx+edx]
    lea     eax,    [eax+2*ebx]
    paddw   mm0,    mm1
    paddw   mm0,    mm2
    lea     ecx,    [ecx+2*edx]
%endmacro

%macro SAD_INC_2x4P 0
    movd    mm1,    [eax]
    movd    mm2,    [ecx]
    movd    mm3,    [eax+ebx]
    movd    mm4,    [ecx+edx]

    psadbw  mm1,    mm2
    psadbw  mm3,    mm4
    paddw   mm0,    mm1
    paddw   mm0,    mm3

    lea     eax,    [eax+2*ebx]
    lea     ecx,    [ecx+2*edx]
%endmacro

; sad x3 / x4

%macro SAD_X3_START_1x8P 1
    push    edi
    push    esi
    mov     edi,    [esp+12]
    mov     eax,    [esp+16]
    mov     ecx,    [esp+20]
    mov     edx,    [esp+24]
    mov     esi,    [esp+28]
    mov%1   mm3,    [edi]
    mov%1   mm0,    [eax]
    mov%1   mm1,    [ecx]
    mov%1   mm2,    [edx]
    psadbw  mm0,    mm3
    psadbw  mm1,    mm3
    psadbw  mm2,    mm3
%endmacro

%macro SAD_X3_1x8P 3
    mov%1   mm3,    [edi+%2]
    mov%1   mm4,    [eax+%3]
    mov%1   mm5,    [ecx+%3]
    mov%1   mm6,    [edx+%3]
    psadbw  mm4,    mm3
    psadbw  mm5,    mm3
    psadbw  mm6,    mm3
    paddw   mm0,    mm4
    paddw   mm1,    mm5
    paddw   mm2,    mm6
%endmacro

%macro SAD_X3_2x16P 1
%if %1
    SAD_X3_START_1x8P q
%else
    SAD_X3_1x8P q, 0, 0
%endif
    SAD_X3_1x8P q, 8, 8
    SAD_X3_1x8P q, FENC_STRIDE, esi
    SAD_X3_1x8P q, FENC_STRIDE+8, esi+8
    add     edi, 2*FENC_STRIDE
    lea     eax, [eax+2*esi]
    lea     ecx, [ecx+2*esi]
    lea     edx, [edx+2*esi]
%endmacro

%macro SAD_X3_2x8P 1
%if %1
    SAD_X3_START_1x8P q
%else
    SAD_X3_1x8P q, 0, 0
%endif
    SAD_X3_1x8P q, FENC_STRIDE, esi
    add     edi, 2*FENC_STRIDE
    lea     eax, [eax+2*esi]
    lea     ecx, [ecx+2*esi]
    lea     edx, [edx+2*esi]
%endmacro

%macro SAD_X3_2x4P 1
%if %1
    SAD_X3_START_1x8P d
%else
    SAD_X3_1x8P d, 0, 0
%endif
    SAD_X3_1x8P d, FENC_STRIDE, esi
    add     edi, 2*FENC_STRIDE
    lea     eax, [eax+2*esi]
    lea     ecx, [ecx+2*esi]
    lea     edx, [edx+2*esi]
%endmacro

%macro SAD_X4_START_1x8P 1
    push    edi
    push    esi
    push    ebx
    mov     edi,    [esp+16]
    mov     eax,    [esp+20]
    mov     ebx,    [esp+24]
    mov     ecx,    [esp+28]
    mov     edx,    [esp+32]
    mov     esi,    [esp+36]
    mov%1   mm7,    [edi]
    mov%1   mm0,    [eax]
    mov%1   mm1,    [ebx]
    mov%1   mm2,    [ecx]
    mov%1   mm3,    [edx]
    psadbw  mm0,    mm7
    psadbw  mm1,    mm7
    psadbw  mm2,    mm7
    psadbw  mm3,    mm7
%endmacro

%macro SAD_X4_1x8P 2
    movq    mm7,    [edi+%1]
    movq    mm4,    [eax+%2]
    movq    mm5,    [ebx+%2]
    movq    mm6,    [ecx+%2]
    psadbw  mm4,    mm7
    psadbw  mm5,    mm7
    psadbw  mm6,    mm7
    psadbw  mm7,    [edx+%2]
    paddw   mm0,    mm4
    paddw   mm1,    mm5
    paddw   mm2,    mm6
    paddw   mm3,    mm7
%endmacro

%macro SAD_X4_1x4P 2
    movd    mm7,    [edi+%1]
    movd    mm4,    [eax+%2]
    movd    mm5,    [ebx+%2]
    movd    mm6,    [ecx+%2]
    psadbw  mm4,    mm7
    psadbw  mm5,    mm7
    paddw   mm0,    mm4
    psadbw  mm6,    mm7
    movd    mm4,    [edx+%2]
    paddw   mm1,    mm5
    psadbw  mm4,    mm7
    paddw   mm2,    mm6
    paddw   mm3,    mm4
%endmacro

%macro SAD_X4_2x16P 1
%if %1
    SAD_X4_START_1x8P q
%else
    SAD_X4_1x8P 0, 0
%endif
    SAD_X4_1x8P 8, 8
    SAD_X4_1x8P FENC_STRIDE, esi
    SAD_X4_1x8P FENC_STRIDE+8, esi+8
    add     edi, 2*FENC_STRIDE
    lea     eax, [eax+2*esi]
    lea     ebx, [ebx+2*esi]
    lea     ecx, [ecx+2*esi]
    lea     edx, [edx+2*esi]
%endmacro

%macro SAD_X4_2x8P 1
%if %1
    SAD_X4_START_1x8P q
%else
    SAD_X4_1x8P 0, 0
%endif
    SAD_X4_1x8P FENC_STRIDE, esi
    add     edi, 2*FENC_STRIDE
    lea     eax, [eax+2*esi]
    lea     ebx, [ebx+2*esi]
    lea     ecx, [ecx+2*esi]
    lea     edx, [edx+2*esi]
%endmacro

%macro SAD_X4_2x4P 1
%if %1
    SAD_X4_START_1x8P d
%else
    SAD_X4_1x4P 0, 0
%endif
    SAD_X4_1x4P FENC_STRIDE, esi
    add     edi, 2*FENC_STRIDE
    lea     eax, [eax+2*esi]
    lea     ebx, [ebx+2*esi]
    lea     ecx, [ecx+2*esi]
    lea     edx, [edx+2*esi]
%endmacro

%macro SAD_X3_END 0
    mov     eax,  [esp+32]
    movd    [eax+0], mm0
    movd    [eax+4], mm1  
    movd    [eax+8], mm2
    pop     esi
    pop     edi
    ret
%endmacro

%macro SAD_X4_END 0
    mov     eax,  [esp+40]
    movd    [eax+0], mm0
    movd    [eax+4], mm1  
    movd    [eax+8], mm2
    movd    [eax+12], mm3
    pop     ebx
    pop     esi
    pop     edi
    ret
%endmacro

; ssd

%macro SSD_INC_1x16P 0
    movq    mm1,    [eax]
    movq    mm2,    [ecx]
    movq    mm3,    [eax+8]
    movq    mm4,    [ecx+8]

    movq    mm5,    mm2
    movq    mm6,    mm4
    psubusb mm2,    mm1
    psubusb mm4,    mm3
    psubusb mm1,    mm5
    psubusb mm3,    mm6
    por     mm1,    mm2
    por     mm3,    mm4

    movq    mm2,    mm1
    movq    mm4,    mm3
    punpcklbw mm1,  mm7
    punpcklbw mm3,  mm7
    punpckhbw mm2,  mm7
    punpckhbw mm4,  mm7
    pmaddwd mm1,    mm1
    pmaddwd mm2,    mm2
    pmaddwd mm3,    mm3
    pmaddwd mm4,    mm4

    add     eax,    ebx
    add     ecx,    edx
    paddd   mm0,    mm1
    paddd   mm0,    mm2
    paddd   mm0,    mm3
    paddd   mm0,    mm4
%endmacro

%macro SSD_INC_1x8P 0
    movq    mm1,    [eax]
    movq    mm2,    [ecx]

    movq    mm5,    mm2
    psubusb mm2,    mm1
    psubusb mm1,    mm5
    por     mm1,    mm2         ; mm1 = 8bit abs diff

    movq    mm2,    mm1
    punpcklbw mm1,  mm7
    punpckhbw mm2,  mm7         ; (mm1,mm2) = 16bit abs diff
    pmaddwd mm1,    mm1
    pmaddwd mm2,    mm2

    add     eax,    ebx
    add     ecx,    edx
    paddd   mm0,    mm1
    paddd   mm0,    mm2
%endmacro

%macro SSD_INC_1x4P 0
    movd    mm1,    [eax]
    movd    mm2,    [ecx]

    movq    mm5,    mm2
    psubusb mm2,    mm1
    psubusb mm1,    mm5
    por     mm1,    mm2
    punpcklbw mm1,  mm7
    pmaddwd mm1,    mm1

    add     eax,    ebx
    add     ecx,    edx
    paddd   mm0,    mm1
%endmacro

; satd

%macro LOAD_DIFF_4P 4  ; MMP, MMT, [pix1], [pix2]
    movd        %1, %3
    movd        %2, %4
    punpcklbw   %1, %2
    punpcklbw   %2, %2
    psubw       %1, %2
%endmacro

; in: eax=pix1, ebx=stride1, ecx=pix2, edx=stride2, %1=horizontal offset
; out: mm4..mm7= 16bit diffs
; clobber: mm3
%macro LOAD_DIFF_INC_4x4 1
    LOAD_DIFF_4P mm4, mm3, [eax+%1],     [ecx+%1]
    LOAD_DIFF_4P mm5, mm3, [eax+ebx+%1], [ecx+edx+%1]
    lea eax, [eax+2*ebx]
    lea ecx, [ecx+2*edx]
    LOAD_DIFF_4P mm6, mm3, [eax+%1],     [ecx+%1]
    LOAD_DIFF_4P mm7, mm3, [eax+ebx+%1], [ecx+edx+%1]
    lea eax, [eax+2*ebx]
    lea ecx, [ecx+2*edx]
%endmacro

%macro LOAD_DIFF_4x4 1
    LOAD_DIFF_4P mm4, mm3, [eax+%1],       [ecx+%1]
    LOAD_DIFF_4P mm6, mm3, [eax+2*ebx+%1], [ecx+2*edx+%1]
    add eax, ebx
    add ecx, edx
    LOAD_DIFF_4P mm5, mm3, [eax+%1],       [ecx+%1]
    LOAD_DIFF_4P mm7, mm3, [eax+2*ebx+%1], [ecx+2*edx+%1]
%endmacro

%macro HADAMARD4_SUB_BADC 4
    paddw %1,   %2
    paddw %3,   %4
    paddw %2,   %2
    paddw %4,   %4
    psubw %2,   %1
    psubw %4,   %3
%endmacro

%macro HADAMARD4x4 4
    HADAMARD4_SUB_BADC %1, %2, %3, %4
    HADAMARD4_SUB_BADC %1, %3, %2, %4
%endmacro

%macro SBUTTERFLYwd 3
    movq        %3, %1
    punpcklwd   %1, %2
    punpckhwd   %3, %2
%endmacro

%macro SBUTTERFLYdq 3
    movq        %3, %1
    punpckldq   %1, %2
    punpckhdq   %3, %2
%endmacro

%macro TRANSPOSE4x4 5   ; abcd-t -> adtc
    SBUTTERFLYwd %1, %2, %5
    SBUTTERFLYwd %3, %4, %2
    SBUTTERFLYdq %1, %3, %4
    SBUTTERFLYdq %5, %2, %3
%endmacro

%macro MMX_ABS 2        ; mma, mmt
    pxor    %2, %2
    psubw   %2, %1
    pmaxsw  %1, %2
%endmacro

%macro HADAMARD4x4_SUM 1    ; %1 - dest (row sum of one block)
    HADAMARD4x4 mm4, mm5, mm6, mm7
    TRANSPOSE4x4 mm4, mm5, mm6, mm7, %1
    HADAMARD4x4 mm4, mm7, %1, mm6
    MMX_ABS     mm4, mm5
    MMX_ABS     mm7, mm5
    MMX_ABS     %1,  mm5
    MMX_ABS     mm6, mm5
    paddw       %1,  mm4
    paddw       mm6, mm7
    pavgw       %1,  mm6
%endmacro

; in: %2 = horizontal offset
; in: %3 = whether we need to increment pix1 and pix2
; clobber: mm3..mm7
; out: %1 = satd
%macro LOAD_DIFF_HADAMARD_SUM 3
%if %3
    LOAD_DIFF_INC_4x4 %2
%else
    LOAD_DIFF_4x4 %2
%endif
    HADAMARD4x4_SUM %1
%endmacro

;=============================================================================
; Code
;=============================================================================

SECTION .text

cglobal xavs_pixel_sad_16x16_mmxext
cglobal xavs_pixel_sad_16x8_mmxext
cglobal xavs_pixel_sad_8x16_mmxext
cglobal xavs_pixel_sad_8x8_mmxext
cglobal xavs_pixel_sad_8x4_mmxext
cglobal xavs_pixel_sad_4x8_mmxext
cglobal xavs_pixel_sad_4x4_mmxext

cglobal xavs_pixel_sad_x3_16x16_mmxext
cglobal xavs_pixel_sad_x3_16x8_mmxext
cglobal xavs_pixel_sad_x3_8x16_mmxext
cglobal xavs_pixel_sad_x3_8x8_mmxext
cglobal xavs_pixel_sad_x3_8x4_mmxext
cglobal xavs_pixel_sad_x3_4x8_mmxext
cglobal xavs_pixel_sad_x3_4x4_mmxext

cglobal xavs_pixel_sad_x4_16x16_mmxext
cglobal xavs_pixel_sad_x4_16x8_mmxext
cglobal xavs_pixel_sad_x4_8x16_mmxext
cglobal xavs_pixel_sad_x4_8x8_mmxext
cglobal xavs_pixel_sad_x4_8x4_mmxext
cglobal xavs_pixel_sad_x4_4x8_mmxext
cglobal xavs_pixel_sad_x4_4x4_mmxext

cglobal xavs_pixel_sad_pde_16x16_mmxext
cglobal xavs_pixel_sad_pde_16x8_mmxext
cglobal xavs_pixel_sad_pde_8x16_mmxext

cglobal xavs_pixel_ssd_16x16_mmx
cglobal xavs_pixel_ssd_16x8_mmx
cglobal xavs_pixel_ssd_8x16_mmx
cglobal xavs_pixel_ssd_8x8_mmx
cglobal xavs_pixel_ssd_8x4_mmx
cglobal xavs_pixel_ssd_4x8_mmx
cglobal xavs_pixel_ssd_4x4_mmx

cglobal xavs_pixel_satd_4x4_mmxext
cglobal xavs_pixel_satd_4x8_mmxext
cglobal xavs_pixel_satd_8x4_mmxext
cglobal xavs_pixel_satd_8x8_mmxext
cglobal xavs_pixel_satd_16x8_mmxext
cglobal xavs_pixel_satd_8x16_mmxext
cglobal xavs_pixel_satd_16x16_mmxext

%macro SAD_START 0
    push    ebx

    mov     eax,    [esp+ 8]    ; pix1
    mov     ebx,    [esp+12]    ; stride1
    mov     ecx,    [esp+16]    ; pix2
    mov     edx,    [esp+20]    ; stride2

    pxor    mm0,    mm0
%endmacro
%macro SAD_END 0
    movd eax,    mm0

    pop ebx
    ret
%endmacro

;-----------------------------------------------------------------------------
;   int __cdecl xavs_pixel_sad_16x16_mmxext (uint8_t *, int, uint8_t *, int )
;-----------------------------------------------------------------------------
%macro SAD 2
ALIGN 16
xavs_pixel_sad_%1x%2_mmxext:
    SAD_START
%rep %2/2
    SAD_INC_2x%1P
%endrep
    SAD_END
%endmacro

SAD 16, 16
SAD 16,  8
SAD  8, 16
SAD  8,  8
SAD  8,  4
SAD  4,  8
SAD  4,  4

;-----------------------------------------------------------------------------
;  void xavs_pixel_sad_x3_16x16_mmxext( uint8_t *fenc, uint8_t *pix0, uint8_t *pix1,
;                                       uint8_t *pix2, int i_stride, int scores[3] )
;-----------------------------------------------------------------------------
%macro SAD_X 3
ALIGN 16
xavs_pixel_sad_x%1_%2x%3_mmxext:
    SAD_X%1_2x%2P 1
%rep %3/2-1
    SAD_X%1_2x%2P 0
%endrep
    SAD_X%1_END
%endmacro

SAD_X 3, 16, 16
SAD_X 3, 16,  8
SAD_X 3,  8, 16
SAD_X 3,  8,  8
SAD_X 3,  8,  4
SAD_X 3,  4,  8
SAD_X 3,  4,  4
SAD_X 4, 16, 16
SAD_X 4, 16,  8
SAD_X 4,  8, 16
SAD_X 4,  8,  8
SAD_X 4,  8,  4
SAD_X 4,  4,  8
SAD_X 4,  4,  4


;-----------------------------------------------------------------------------
;   int __cdecl xavs_pixel_sad_pde_16x16_mmxext (uint8_t *, int, uint8_t *, int, int )
;-----------------------------------------------------------------------------
%macro SAD_PDE 2
ALIGN 16
xavs_pixel_sad_pde_%1x%2_mmxext:
    SAD_START
%rep %2/4
    SAD_INC_2x%1P
%endrep

    movd ebx, mm0
    cmp  ebx, [esp+24] ; prev_score
    jl   .continue
    pop  ebx
    mov  eax, 0xffff
    ret
ALIGN 4
.continue:
    mov  ebx, [esp+12]

%rep %2/4
    SAD_INC_2x%1P
%endrep
    SAD_END
%endmacro

SAD_PDE 16, 16
SAD_PDE 16 , 8
SAD_PDE  8, 16



%macro SSD_START 0
    push    ebx

    mov     eax,    [esp+ 8]    ; pix1
    mov     ebx,    [esp+12]    ; stride1
    mov     ecx,    [esp+16]    ; pix2
    mov     edx,    [esp+20]    ; stride2

    pxor    mm7,    mm7         ; zero
    pxor    mm0,    mm0         ; mm0 holds the sum
%endmacro

%macro SSD_END 0
    movq    mm1,    mm0
    psrlq   mm1,    32
    paddd   mm0,    mm1
    movd    eax,    mm0

    pop ebx
    ret
%endmacro

;-----------------------------------------------------------------------------
;   int __cdecl xavs_pixel_ssd_16x16_mmx (uint8_t *, int, uint8_t *, int )
;-----------------------------------------------------------------------------
%macro SSD 2
ALIGN 16
xavs_pixel_ssd_%1x%2_mmx:
    SSD_START
%rep %2
    SSD_INC_1x%1P
%endrep
    SSD_END
%endmacro

SSD 16, 16
SSD 16,  8
SSD  8, 16
SSD  8,  8
SSD  8,  4
SSD  4,  8
SSD  4,  4



%macro SATD_START 0
    push        ebx

    mov         eax, [esp+ 8]       ; pix1
    mov         ebx, [esp+12]       ; stride1
    mov         ecx, [esp+16]       ; pix2
    mov         edx, [esp+20]       ; stride2
%endmacro

%macro SATD_END 0
    pshufw      mm1, mm0, 01001110b
    paddw       mm0, mm1
    pshufw      mm1, mm0, 10110001b
    paddw       mm0, mm1
    movd        eax, mm0
    and         eax, 0xffff
    pop         ebx
    ret
%endmacro

ALIGN 16
;-----------------------------------------------------------------------------
;   int __cdecl xavs_pixel_satd_4x4_mmxext (uint8_t *, int, uint8_t *, int )
;-----------------------------------------------------------------------------
xavs_pixel_satd_4x4_mmxext:
    SATD_START
    LOAD_DIFF_HADAMARD_SUM mm0, 0, 0
    SATD_END

ALIGN 16
;-----------------------------------------------------------------------------
;   int __cdecl xavs_pixel_satd_4x8_mmxext (uint8_t *, int, uint8_t *, int )
;-----------------------------------------------------------------------------
xavs_pixel_satd_4x8_mmxext:
    SATD_START
    LOAD_DIFF_HADAMARD_SUM mm0, 0, 1
    LOAD_DIFF_HADAMARD_SUM mm1, 0, 0
    paddw       mm0, mm1
    SATD_END

ALIGN 16
;-----------------------------------------------------------------------------
;   int __cdecl xavs_pixel_satd_8x4_mmxext (uint8_t *, int, uint8_t *, int )
;-----------------------------------------------------------------------------
xavs_pixel_satd_8x4_mmxext:
    SATD_START
    LOAD_DIFF_HADAMARD_SUM mm0, 0, 0
    sub         eax, ebx
    sub         ecx, edx
    LOAD_DIFF_HADAMARD_SUM mm1, 4, 0
    paddw       mm0, mm1
    SATD_END

ALIGN 16
;-----------------------------------------------------------------------------
;   int __cdecl xavs_pixel_satd_8x8_mmxext (uint8_t *, int, uint8_t *, int )
;-----------------------------------------------------------------------------
xavs_pixel_satd_8x8_mmxext:
    SATD_START
    LOAD_DIFF_HADAMARD_SUM mm0, 0, 1
    LOAD_DIFF_HADAMARD_SUM mm1, 0, 0

    mov         eax, [esp+ 8]       ; pix1
    mov         ecx, [esp+16]       ; pix2
    LOAD_DIFF_HADAMARD_SUM mm2, 4, 1
    LOAD_DIFF_HADAMARD_SUM mm3, 4, 0
    paddw       mm0, mm1
    paddw       mm2, mm3
    paddw       mm0, mm2
    SATD_END

ALIGN 16
;-----------------------------------------------------------------------------
;   int __cdecl xavs_pixel_satd_16x8_mmxext (uint8_t *, int, uint8_t *, int )
;-----------------------------------------------------------------------------
xavs_pixel_satd_16x8_mmxext:
    SATD_START
    LOAD_DIFF_HADAMARD_SUM mm0, 0, 1
    LOAD_DIFF_HADAMARD_SUM mm1, 0, 0

    mov         eax, [esp+ 8]       ; pix1
    mov         ecx, [esp+16]       ; pix2
    LOAD_DIFF_HADAMARD_SUM mm2, 4, 1
    LOAD_DIFF_HADAMARD_SUM mm3, 4, 0
    paddw       mm0, mm1
    paddw       mm2, mm3
    paddw       mm0, mm2

    mov         eax, [esp+ 8]       ; pix1
    mov         ecx, [esp+16]       ; pix2
    LOAD_DIFF_HADAMARD_SUM mm1, 8, 1
    LOAD_DIFF_HADAMARD_SUM mm2, 8, 0
    paddw       mm1, mm2

    mov         eax, [esp+ 8]       ; pix1
    mov         ecx, [esp+16]       ; pix2
    LOAD_DIFF_HADAMARD_SUM mm2, 12, 1
    LOAD_DIFF_HADAMARD_SUM mm3, 12, 0
    paddw       mm0, mm1
    paddw       mm2, mm3
    paddw       mm0, mm2
    SATD_END

ALIGN 16
;-----------------------------------------------------------------------------
;   int __cdecl xavs_pixel_satd_8x16_mmxext (uint8_t *, int, uint8_t *, int )
;-----------------------------------------------------------------------------
xavs_pixel_satd_8x16_mmxext:
    SATD_START
    LOAD_DIFF_HADAMARD_SUM mm0, 0, 1
    LOAD_DIFF_HADAMARD_SUM mm1, 0, 1
    LOAD_DIFF_HADAMARD_SUM mm2, 0, 1
    LOAD_DIFF_HADAMARD_SUM mm3, 0, 0
    paddw       mm0, mm1
    paddw       mm2, mm3
    paddw       mm0, mm2

    mov         eax, [esp+ 8]       ; pix1
    mov         ecx, [esp+16]       ; pix2
    LOAD_DIFF_HADAMARD_SUM mm1, 4, 1
    LOAD_DIFF_HADAMARD_SUM mm2, 4, 1
    paddw       mm1, mm2
    LOAD_DIFF_HADAMARD_SUM mm2, 4, 1
    LOAD_DIFF_HADAMARD_SUM mm3, 4, 0
    paddw       mm0, mm1
    paddw       mm2, mm3
    paddw       mm0, mm2
    SATD_END

ALIGN 16
;-----------------------------------------------------------------------------
;   int __cdecl xavs_pixel_satd_16x16_mmxext (uint8_t *, int, uint8_t *, int )
;-----------------------------------------------------------------------------
xavs_pixel_satd_16x16_mmxext:
    SATD_START
    LOAD_DIFF_HADAMARD_SUM mm0, 0, 1
    LOAD_DIFF_HADAMARD_SUM mm1, 0, 1
    LOAD_DIFF_HADAMARD_SUM mm2, 0, 1
    LOAD_DIFF_HADAMARD_SUM mm3, 0, 0
    paddw       mm0, mm1
    paddw       mm2, mm3
    paddw       mm0, mm2

    mov         eax, [esp+ 8]       ; pix1
    mov         ecx, [esp+16]       ; pix2
    LOAD_DIFF_HADAMARD_SUM mm1, 4, 1
    LOAD_DIFF_HADAMARD_SUM mm2, 4, 1
    paddw       mm1, mm2
    LOAD_DIFF_HADAMARD_SUM mm2, 4, 1
    LOAD_DIFF_HADAMARD_SUM mm3, 4, 0
    paddw       mm0, mm1
    paddw       mm2, mm3
    paddw       mm0, mm2

    mov         eax, [esp+ 8]       ; pix1
    mov         ecx, [esp+16]       ; pix2
    LOAD_DIFF_HADAMARD_SUM mm1, 8, 1
    LOAD_DIFF_HADAMARD_SUM mm2, 8, 1
    paddw       mm1, mm2
    LOAD_DIFF_HADAMARD_SUM mm2, 8, 1
    LOAD_DIFF_HADAMARD_SUM mm3, 8, 0
    paddw       mm0, mm1
    paddw       mm2, mm3
    paddw       mm0, mm2

    mov         eax, [esp+ 8]       ; pix1
    mov         ecx, [esp+16]       ; pix2
    LOAD_DIFF_HADAMARD_SUM mm1, 12, 1
    LOAD_DIFF_HADAMARD_SUM mm2, 12, 1
    paddw       mm1, mm2
    LOAD_DIFF_HADAMARD_SUM mm2, 12, 1
    LOAD_DIFF_HADAMARD_SUM mm3, 12, 0
    paddw       mm0, mm1
    paddw       mm2, mm3
    paddw       mm0, mm2

    pxor        mm3, mm3
    pshufw      mm1, mm0, 01001110b
    paddw       mm0, mm1
    punpcklwd   mm0, mm3
    pshufw      mm1, mm0, 01001110b
    paddd       mm0, mm1
    movd        eax, mm0
    pop         ebx
    ret


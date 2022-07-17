/*****************************************************************************
 * gotvptop_fourcc.h: Definition of various FOURCC and helpers
 *****************************************************************************
 * Copyright (C) 2009 Laurent Aimar
 * $Id: 02e65f37b4be8b7150fa7ea5e74bb587754f3352 $
 *
 * Authors: Laurent Aimar <fenrir _AT_ videolan _DOT_ com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifndef GOTV_FOURCC_H
#define GOTV_FOURCC_H 1

#define GOTV_CODEC_UNKNOWN         GOTV_FOURCC('u','n','d','f')

/* Video codec */
#define GOTV_CODEC_MPGV            GOTV_FOURCC('m','p','g','v')
#define GOTV_CODEC_MP4V            GOTV_FOURCC('m','p','4','v')
#define GOTV_CODEC_DIV1            GOTV_FOURCC('D','I','V','1')
#define GOTV_CODEC_DIV2            GOTV_FOURCC('D','I','V','2')
#define GOTV_CODEC_DIV3            GOTV_FOURCC('D','I','V','3')
#define GOTV_CODEC_SVQ1            GOTV_FOURCC('S','V','Q','1')
#define GOTV_CODEC_SVQ3            GOTV_FOURCC('S','V','Q','3')
#define GOTV_CODEC_H264            GOTV_FOURCC('h','2','6','4')
#define GOTV_CODEC_H263            GOTV_FOURCC('h','2','6','3')
#define GOTV_CODEC_H263I           GOTV_FOURCC('I','2','6','3')
#define GOTV_CODEC_H263P           GOTV_FOURCC('I','L','V','R')
#define GOTV_CODEC_FLV1            GOTV_FOURCC('F','L','V','1')
#define GOTV_CODEC_H261            GOTV_FOURCC('h','2','6','1')
#define GOTV_CODEC_MJPG            GOTV_FOURCC('M','J','P','G')
#define GOTV_CODEC_MJPGB           GOTV_FOURCC('m','j','p','b')
#define GOTV_CODEC_LJPG            GOTV_FOURCC('L','J','P','G')
#define GOTV_CODEC_WMV1            GOTV_FOURCC('W','M','V','1')
#define GOTV_CODEC_WMV2            GOTV_FOURCC('W','M','V','2')
#define GOTV_CODEC_WMV3            GOTV_FOURCC('W','M','V','3')
#define GOTV_CODEC_WMVA            GOTV_FOURCC('W','M','V','A')
#define GOTV_CODEC_WMVP            GOTV_FOURCC('W','M','V','P')
#define GOTV_CODEC_WMVP2           GOTV_FOURCC('W','V','P','2')
#define GOTV_CODEC_VC1             GOTV_FOURCC('V','C','-','1')
#define GOTV_CODEC_DAALA           GOTV_FOURCC('d','a','a','l')
#define GOTV_CODEC_THEORA          GOTV_FOURCC('t','h','e','o')
#define GOTV_CODEC_TARKIN          GOTV_FOURCC('t','a','r','k')
#define GOTV_CODEC_DIRAC           GOTV_FOURCC('d','r','a','c')
#define GOTV_CODEC_OGGSPOTS        GOTV_FOURCC('S','P','O','T')
#define GOTV_CODEC_CAVS            GOTV_FOURCC('C','A','V','S')
#define GOTV_CODEC_NUV             GOTV_FOURCC('N','J','P','G')
#define GOTV_CODEC_RV10            GOTV_FOURCC('R','V','1','0')
#define GOTV_CODEC_RV13            GOTV_FOURCC('R','V','1','3')
#define GOTV_CODEC_RV20            GOTV_FOURCC('R','V','2','0')
#define GOTV_CODEC_RV30            GOTV_FOURCC('R','V','3','0')
#define GOTV_CODEC_RV40            GOTV_FOURCC('R','V','4','0')
#define GOTV_CODEC_VP3             GOTV_FOURCC('V','P','3',' ')
#define GOTV_CODEC_VP5             GOTV_FOURCC('V','P','5',' ')
#define GOTV_CODEC_VP6             GOTV_FOURCC('V','P','6','2')
#define GOTV_CODEC_VP6F            GOTV_FOURCC('V','P','6','F')
#define GOTV_CODEC_VP6A            GOTV_FOURCC('V','P','6','A')
#define GOTV_CODEC_MSVIDEO1        GOTV_FOURCC('M','S','V','C')
#define GOTV_CODEC_FLIC            GOTV_FOURCC('F','L','I','C')
#define GOTV_CODEC_SP5X            GOTV_FOURCC('S','P','5','X')
#define GOTV_CODEC_DV              GOTV_FOURCC('d','v',' ',' ')
#define GOTV_CODEC_MSRLE           GOTV_FOURCC('m','r','l','e')
#define GOTV_CODEC_HUFFYUV         GOTV_FOURCC('H','F','Y','U')
#define GOTV_CODEC_FFVHUFF         GOTV_FOURCC('F','F','V','H')
#define GOTV_CODEC_ASV1            GOTV_FOURCC('A','S','V','1')
#define GOTV_CODEC_ASV2            GOTV_FOURCC('A','S','V','2')
#define GOTV_CODEC_FFV1            GOTV_FOURCC('F','F','V','1')
#define GOTV_CODEC_VCR1            GOTV_FOURCC('V','C','R','1')
#define GOTV_CODEC_CLJR            GOTV_FOURCC('C','L','J','R')
#define GOTV_CODEC_RPZA            GOTV_FOURCC('r','p','z','a')
#define GOTV_CODEC_SMC             GOTV_FOURCC('s','m','c',' ')
#define GOTV_CODEC_CINEPAK         GOTV_FOURCC('C','V','I','D')
#define GOTV_CODEC_TSCC            GOTV_FOURCC('T','S','C','C')
#define GOTV_CODEC_CSCD            GOTV_FOURCC('C','S','C','D')
#define GOTV_CODEC_ZMBV            GOTV_FOURCC('Z','M','B','V')
#define GOTV_CODEC_VMNC            GOTV_FOURCC('V','M','n','c')
#define GOTV_CODEC_FMVC            GOTV_FOURCC('F','M','V','C')
#define GOTV_CODEC_FRAPS           GOTV_FOURCC('F','P','S','1')
#define GOTV_CODEC_TRUEMOTION1     GOTV_FOURCC('D','U','C','K')
#define GOTV_CODEC_TRUEMOTION2     GOTV_FOURCC('T','M','2','0')
#define GOTV_CODEC_QTRLE           GOTV_FOURCC('r','l','e',' ')
#define GOTV_CODEC_QDRAW           GOTV_FOURCC('q','d','r','w')
#define GOTV_CODEC_QPEG            GOTV_FOURCC('Q','P','E','G')
#define GOTV_CODEC_ULTI            GOTV_FOURCC('U','L','T','I')
#define GOTV_CODEC_VIXL            GOTV_FOURCC('V','I','X','L')
#define GOTV_CODEC_LOCO            GOTV_FOURCC('L','O','C','O')
#define GOTV_CODEC_WNV1            GOTV_FOURCC('W','N','V','1')
#define GOTV_CODEC_AASC            GOTV_FOURCC('A','A','S','C')
#define GOTV_CODEC_INDEO2          GOTV_FOURCC('I','V','2','0')
#define GOTV_CODEC_INDEO3          GOTV_FOURCC('I','V','3','1')
#define GOTV_CODEC_INDEO4          GOTV_FOURCC('I','V','4','1')
#define GOTV_CODEC_INDEO5          GOTV_FOURCC('I','V','5','0')
#define GOTV_CODEC_FLASHSV         GOTV_FOURCC('F','S','V','1')
#define GOTV_CODEC_KMVC            GOTV_FOURCC('K','M','V','C')
#define GOTV_CODEC_SMACKVIDEO      GOTV_FOURCC('S','M','K','2')
#define GOTV_CODEC_DNXHD           GOTV_FOURCC('A','V','d','n')
#define GOTV_CODEC_8BPS            GOTV_FOURCC('8','B','P','S')
#define GOTV_CODEC_MIMIC           GOTV_FOURCC('M','L','2','O')
#define GOTV_CODEC_INTERPLAY       GOTV_FOURCC('i','m','v','e')
#define GOTV_CODEC_IDCIN           GOTV_FOURCC('I','D','C','I')
#define GOTV_CODEC_4XM             GOTV_FOURCC('4','X','M','V')
#define GOTV_CODEC_ROQ             GOTV_FOURCC('R','o','Q','v')
#define GOTV_CODEC_MDEC            GOTV_FOURCC('M','D','E','C')
#define GOTV_CODEC_VMDVIDEO        GOTV_FOURCC('V','M','D','V')
#define GOTV_CODEC_CDG             GOTV_FOURCC('C','D','G',' ')
#define GOTV_CODEC_FRWU            GOTV_FOURCC('F','R','W','U')
#define GOTV_CODEC_AMV             GOTV_FOURCC('A','M','V',' ')
#define GOTV_CODEC_VP7             GOTV_FOURCC('V','P','7','0')
#define GOTV_CODEC_VP8             GOTV_FOURCC('V','P','8','0')
#define GOTV_CODEC_VP9             GOTV_FOURCC('V','P','9','0')
#define GOTV_CODEC_VP10            GOTV_FOURCC('V','P',':','0')
#define GOTV_CODEC_AV1             GOTV_FOURCC('a','v','0','1')
#define GOTV_CODEC_JPEG2000        GOTV_FOURCC('J','P','2','K')
#define GOTV_CODEC_LAGARITH        GOTV_FOURCC('L','A','G','S')
#define GOTV_CODEC_FLASHSV2        GOTV_FOURCC('F','S','V','2')
#define GOTV_CODEC_PRORES          GOTV_FOURCC('a','p','c','n')
#define GOTV_CODEC_MXPEG           GOTV_FOURCC('M','X','P','G')
#define GOTV_CODEC_CDXL            GOTV_FOURCC('C','D','X','L')
#define GOTV_CODEC_BMVVIDEO        GOTV_FOURCC('B','M','V','V')
#define GOTV_CODEC_UTVIDEO         GOTV_FOURCC('U','L','R','A')
#define GOTV_CODEC_VBLE            GOTV_FOURCC('V','B','L','E')
#define GOTV_CODEC_DXTORY          GOTV_FOURCC('x','t','o','r')
#define GOTV_CODEC_MSS1            GOTV_FOURCC('M','S','S','1')
#define GOTV_CODEC_MSS2            GOTV_FOURCC('M','S','S','2')
#define GOTV_CODEC_MSA1            GOTV_FOURCC('M','S','A','1')
#define GOTV_CODEC_TSC2            GOTV_FOURCC('T','S','C','2')
#define GOTV_CODEC_MTS2            GOTV_FOURCC('M','T','S','2')
#define GOTV_CODEC_HEVC            GOTV_FOURCC('h','e','v','c')
#define GOTV_CODEC_ICOD            GOTV_FOURCC('i','c','o','d')
#define GOTV_CODEC_G2M2            GOTV_FOURCC('G','2','M','2')
#define GOTV_CODEC_G2M3            GOTV_FOURCC('G','2','M','3')
#define GOTV_CODEC_G2M4            GOTV_FOURCC('G','2','M','4')
#define GOTV_CODEC_BINKVIDEO       GOTV_FOURCC('B','I','K','f')
#define GOTV_CODEC_BINKAUDIO_DCT   GOTV_FOURCC('B','A','U','1')
#define GOTV_CODEC_BINKAUDIO_RDFT  GOTV_FOURCC('B','A','U','2')
#define GOTV_CODEC_XAN_WC4         GOTV_FOURCC('X','x','a','n')
#define GOTV_CODEC_LCL_MSZH        GOTV_FOURCC('M','S','Z','H')
#define GOTV_CODEC_LCL_ZLIB        GOTV_FOURCC('Z','L','I','B')
#define GOTV_CODEC_THP             GOTV_FOURCC('T','H','P','0')
#define GOTV_CODEC_ESCAPE124       GOTV_FOURCC('E','1','2','4')
#define GOTV_CODEC_KGV1            GOTV_FOURCC('K','G','V','1')
#define GOTV_CODEC_CLLC            GOTV_FOURCC('C','L','L','C')
#define GOTV_CODEC_AURA            GOTV_FOURCC('A','U','R','A')
#define GOTV_CODEC_FIC             GOTV_FOURCC('F','I','C','V')
#define GOTV_CODEC_TMV             GOTV_FOURCC('T','M','A','V')
#define GOTV_CODEC_XAN_WC3         GOTV_FOURCC('X','A','N','3')
#define GOTV_CODEC_WS_VQA          GOTV_FOURCC('W','V','Q','A')
#define GOTV_CODEC_MMVIDEO         GOTV_FOURCC('M','M','V','I')
#define GOTV_CODEC_AVS             GOTV_FOURCC('A','V','S','V')
#define GOTV_CODEC_DSICINVIDEO     GOTV_FOURCC('D','C','I','V')
#define GOTV_CODEC_TIERTEXSEQVIDEO GOTV_FOURCC('T','S','E','Q')
#define GOTV_CODEC_DXA             GOTV_FOURCC('D','E','X','A')
#define GOTV_CODEC_C93             GOTV_FOURCC('I','C','9','3')
#define GOTV_CODEC_BETHSOFTVID     GOTV_FOURCC('B','V','I','D')
#define GOTV_CODEC_VB              GOTV_FOURCC('V','B','V','1')
#define GOTV_CODEC_RL2             GOTV_FOURCC('R','L','V','2')
#define GOTV_CODEC_BFI             GOTV_FOURCC('B','F','&','I')
#define GOTV_CODEC_CMV             GOTV_FOURCC('E','C','M','V')
#define GOTV_CODEC_MOTIONPIXELS    GOTV_FOURCC('M','P','I','X')
#define GOTV_CODEC_TGV             GOTV_FOURCC('T','G','V','V')
#define GOTV_CODEC_TGQ             GOTV_FOURCC('T','G','Q','V')
#define GOTV_CODEC_TQI             GOTV_FOURCC('T','Q','I','V')
#define GOTV_CODEC_MAD             GOTV_FOURCC('M','A','D','V')
#define GOTV_CODEC_ANM             GOTV_FOURCC('A','N','I','M')
#define GOTV_CODEC_YOP             GOTV_FOURCC('Y','O','P','V')
#define GOTV_CODEC_JV              GOTV_FOURCC('J','V','0','0')
#define GOTV_CODEC_DFA             GOTV_FOURCC('D','F','I','A')
#define GOTV_CODEC_HNM4_VIDEO      GOTV_FOURCC('H','N','M','4')
#define GOTV_CODEC_TDSC            GOTV_FOURCC('T','D','S','C')
#define GOTV_CODEC_HQX             GOTV_FOURCC('C','H','Q','X')
#define GOTV_CODEC_HQ_HQA          GOTV_FOURCC('C','U','V','C')
#define GOTV_CODEC_HAP             GOTV_FOURCC('H','A','P','1')
#define GOTV_CODEC_DXV             GOTV_FOURCC('D','X','D','3')
#define GOTV_CODEC_CINEFORM        GOTV_FOURCC('C','F','H','D')
#define GOTV_CODEC_SPEEDHQ         GOTV_FOURCC('S','H','Q','2')
#define GOTV_CODEC_PIXLET          GOTV_FOURCC('p','x','l','t')

/***********
 * Chromas
 ***********/

/* Planar YUV */

/* Planar YUV 4:1:0 Y:V:U */
#define GOTV_CODEC_YV9             GOTV_FOURCC('Y','V','U','9')
/* Planar YUV 4:1:0 Y:U:V */
#define GOTV_CODEC_I410            GOTV_FOURCC('I','4','1','0')
/* Planar YUV 4:1:1 Y:U:V */
#define GOTV_CODEC_I411            GOTV_FOURCC('I','4','1','1')

/* Planar YUV 4:2:0 Y:V:U */
#define GOTV_CODEC_YV12            GOTV_FOURCC('Y','V','1','2')
/* Planar YUV 4:2:0 Y:U:V 8-bit */
#define GOTV_CODEC_I420            GOTV_FOURCC('I','4','2','0')
/* Planar YUV 4:2:0 Y:U:V  9-bit stored on 16 bits */
#define GOTV_CODEC_I420_9L         GOTV_FOURCC('I','0','9','L')
#define GOTV_CODEC_I420_9B         GOTV_FOURCC('I','0','9','B')
/* Planar YUV 4:2:0 Y:U:V 10-bit stored on 16 bits LSB */
#define GOTV_CODEC_I420_10L        GOTV_FOURCC('I','0','A','L')
#define GOTV_CODEC_I420_10B        GOTV_FOURCC('I','0','A','B')
/* Planar YUV 4:2:0 Y:U:V 12-bit stored on 16 bits */
#define GOTV_CODEC_I420_12L        GOTV_FOURCC('I','0','C','L')
#define GOTV_CODEC_I420_12B        GOTV_FOURCC('I','0','C','B')

/* Planar YUV 4:2:0 Y:U:V 16-bit stored on 16 bits */
#define GOTV_CODEC_I420_16L        GOTV_FOURCC('I','0','F','L')
#define GOTV_CODEC_I420_16B        GOTV_FOURCC('I','0','F','B')

/* Planar YUV 4:2:2 Y:U:V 8-bit */
#define GOTV_CODEC_I422            GOTV_FOURCC('I','4','2','2')
/* Planar YUV 4:2:2 Y:U:V  9-bit stored on 16 bits */
#define GOTV_CODEC_I422_9L         GOTV_FOURCC('I','2','9','L')
#define GOTV_CODEC_I422_9B         GOTV_FOURCC('I','2','9','B')
/* Planar YUV 4:2:2 Y:U:V 10-bit stored on 16 bits */
#define GOTV_CODEC_I422_10L        GOTV_FOURCC('I','2','A','L')
#define GOTV_CODEC_I422_10B        GOTV_FOURCC('I','2','A','B')
/* Planar YUV 4:2:2 Y:U:V 12-bit stored on 16 bits */
#define GOTV_CODEC_I422_12L        GOTV_FOURCC('I','2','C','L')
#define GOTV_CODEC_I422_12B        GOTV_FOURCC('I','2','C','B')

/* Planar YUV 4:4:0 Y:U:V */
#define GOTV_CODEC_I440            GOTV_FOURCC('I','4','4','0')
/* Planar YUV 4:4:4 Y:U:V 8-bit */
#define GOTV_CODEC_I444            GOTV_FOURCC('I','4','4','4')
/* Planar YUV 4:4:4 Y:U:V  9-bit stored on 16 bits */
#define GOTV_CODEC_I444_9L         GOTV_FOURCC('I','4','9','L')
#define GOTV_CODEC_I444_9B         GOTV_FOURCC('I','4','9','B')
/* Planar YUV 4:4:4 Y:U:V 10-bit stored on 16 bits */
#define GOTV_CODEC_I444_10L        GOTV_FOURCC('I','4','A','L')
#define GOTV_CODEC_I444_10B        GOTV_FOURCC('I','4','A','B')
/* Planar YUV 4:4:4 Y:U:V 12-bit stored on 16 bits */
#define GOTV_CODEC_I444_12L        GOTV_FOURCC('I','4','C','L')
#define GOTV_CODEC_I444_12B        GOTV_FOURCC('I','4','C','B')
/* Planar YUV 4:4:4 Y:U:V 16-bit */
#define GOTV_CODEC_I444_16L        GOTV_FOURCC('I','4','F','L')
#define GOTV_CODEC_I444_16B        GOTV_FOURCC('I','4','F','B')

/* Planar YUV 4:2:0 Y:U:V full scale */
#define GOTV_CODEC_J420            GOTV_FOURCC('J','4','2','0')
/* Planar YUV 4:2:2 Y:U:V full scale */
#define GOTV_CODEC_J422            GOTV_FOURCC('J','4','2','2')
/* Planar YUV 4:4:0 Y:U:V full scale */
#define GOTV_CODEC_J440            GOTV_FOURCC('J','4','4','0')
/* Planar YUV 4:4:4 Y:U:V full scale */
#define GOTV_CODEC_J444            GOTV_FOURCC('J','4','4','4')
/* Palettized YUV with palette element Y:U:V:A */
#define GOTV_CODEC_YUVP            GOTV_FOURCC('Y','U','V','P')

/* Planar YUV 4:4:4 Y:U:V:A */
#define GOTV_CODEC_YUVA            GOTV_FOURCC('Y','U','V','A')
/* Planar YUV 4:2:2 Y:U:V:A */
#define GOTV_CODEC_YUV422A         GOTV_FOURCC('I','4','2','A')
/* Planar YUV 4:2:0 Y:U:V:A */
#define GOTV_CODEC_YUV420A         GOTV_FOURCC('I','4','0','A')

/* Planar Y:U:V:A 4:4:4 10bits */
#define GOTV_CODEC_YUVA_444_10L    GOTV_FOURCC('Y','A','0','L')
#define GOTV_CODEC_YUVA_444_10B    GOTV_FOURCC('Y','A','0','B')

/* Semi-planar Y/UV */

/* 2 planes Y/UV 4:2:0 */
#define GOTV_CODEC_NV12            GOTV_FOURCC('N','V','1','2')
/* 2 planes Y/VU 4:2:0 */
#define GOTV_CODEC_NV21            GOTV_FOURCC('N','V','2','1')
/* 2 planes Y/UV 4:2:2 */
#define GOTV_CODEC_NV16            GOTV_FOURCC('N','V','1','6')
/* 2 planes Y/VU 4:2:2 */
#define GOTV_CODEC_NV61            GOTV_FOURCC('N','V','6','1')
/* 2 planes Y/UV 4:4:4 */
#define GOTV_CODEC_NV24            GOTV_FOURCC('N','V','2','4')
/* 2 planes Y/VU 4:4:4 */
#define GOTV_CODEC_NV42            GOTV_FOURCC('N','V','4','2')
/* 2 planes Y/UV 4:2:0 10-bit */
#define GOTV_CODEC_P010            GOTV_FOURCC('P','0','1','0')

/* Packed YUV */

/* Packed YUV 4:2:2, U:Y:V:Y */
#define GOTV_CODEC_UYVY            GOTV_FOURCC('U','Y','V','Y')
/* Packed YUV 4:2:2, V:Y:U:Y */
#define GOTV_CODEC_VYUY            GOTV_FOURCC('V','Y','U','Y')
/* Packed YUV 4:2:2, Y:U:Y:V */
#define GOTV_CODEC_YUYV            GOTV_FOURCC('Y','U','Y','2')
/* Packed YUV 4:2:2, Y:V:Y:U */
#define GOTV_CODEC_YVYU            GOTV_FOURCC('Y','V','Y','U')
/* Packed YUV 2:1:1, Y:U:Y:V */
#define GOTV_CODEC_Y211            GOTV_FOURCC('Y','2','1','1')
/* Packed YUV 4:2:2, U:Y:V:Y, reverted */
#define GOTV_CODEC_CYUV            GOTV_FOURCC('c','y','u','v')
/* 10-bit 4:2:2 Component YCbCr */
#define GOTV_CODEC_V210            GOTV_FOURCC('v','2','1','0')
/* I420 packed for RTP (RFC 4175) */
#define GOTV_CODEC_R420            GOTV_FOURCC('r','4','2','0')


/* RGB */

/* Palettized RGB with palette element R:G:B */
#define GOTV_CODEC_RGBP            GOTV_FOURCC('R','G','B','P')
/* 8 bits RGB */
#define GOTV_CODEC_RGB8            GOTV_FOURCC('R','G','B','8')
/* 12 bits RGB padded to 16 bits */
#define GOTV_CODEC_RGB12           GOTV_FOURCC('R','V','1','2')
/* 15 bits RGB padded to 16 bits */
#define GOTV_CODEC_RGB15           GOTV_FOURCC('R','V','1','5')
/* 16 bits RGB */
#define GOTV_CODEC_RGB16           GOTV_FOURCC('R','V','1','6')
/* 24 bits RGB */
#define GOTV_CODEC_RGB24           GOTV_FOURCC('R','V','2','4')
/* 24 bits RGB padded to 32 bits */
#define GOTV_CODEC_RGB32           GOTV_FOURCC('R','V','3','2')
/* 32 bits RGBA */
#define GOTV_CODEC_RGBA            GOTV_FOURCC('R','G','B','A')
/* 32 bits ARGB */
#define GOTV_CODEC_ARGB            GOTV_FOURCC('A','R','G','B')
/* 32 bits BGRA */
#define GOTV_CODEC_BGRA            GOTV_FOURCC('B','G','R','A')

/* Planar GBR 4:4:4 8 bits */
#define GOTV_CODEC_GBR_PLANAR      GOTV_FOURCC('G','B','R','8')
#define GOTV_CODEC_GBR_PLANAR_9B   GOTV_FOURCC('G','B','9','B')
#define GOTV_CODEC_GBR_PLANAR_9L   GOTV_FOURCC('G','B','9','L')
#define GOTV_CODEC_GBR_PLANAR_10B  GOTV_FOURCC('G','B','A','B')
#define GOTV_CODEC_GBR_PLANAR_10L  GOTV_FOURCC('G','B','A','L')
#define GOTV_CODEC_GBR_PLANAR_16L  GOTV_FOURCC('G','B','F','L')
#define GOTV_CODEC_GBR_PLANAR_16B  GOTV_FOURCC('G','B','F','B')

/* 8 bits grey */
#define GOTV_CODEC_GREY            GOTV_FOURCC('G','R','E','Y')

/* VDPAU video surface YCbCr 4:2:0 */
#define GOTV_CODEC_VDPAU_VIDEO_420 GOTV_FOURCC('V','D','V','0')
/* VDPAU video surface YCbCr 4:2:2 */
#define GOTV_CODEC_VDPAU_VIDEO_422 GOTV_FOURCC('V','D','V','2')
/* VDPAU video surface YCbCr 4:4:4 */
#define GOTV_CODEC_VDPAU_VIDEO_444 GOTV_FOURCC('V','D','V','4')
/* VDPAU output surface RGBA */
#define GOTV_CODEC_VDPAU_OUTPUT    GOTV_FOURCC('V','D','O','R')

/* VAAPI opaque surface */
#define GOTV_CODEC_VAAPI_420 GOTV_FOURCC('V','A','O','P') /* 4:2:0  8 bpc */
#define GOTV_CODEC_VAAPI_420_10BPP GOTV_FOURCC('V','A','O','0') /* 4:2:0 10 bpc */

/* MediaCodec/IOMX opaque buffer type */
#define GOTV_CODEC_ANDROID_OPAQUE  GOTV_FOURCC('A','N','O','P')

/* Broadcom MMAL opaque buffer type */
#define GOTV_CODEC_MMAL_OPAQUE     GOTV_FOURCC('M','M','A','L')

/* DXVA2 opaque video surface for use with D3D9 */
#define GOTV_CODEC_D3D9_OPAQUE     GOTV_FOURCC('D','X','A','9') /* 4:2:0  8 bpc */
#define GOTV_CODEC_D3D9_OPAQUE_10B GOTV_FOURCC('D','X','A','0') /* 4:2:0 10 bpc */

/* D3D11VA opaque video surface for use with D3D11 */
#define GOTV_CODEC_D3D11_OPAQUE          GOTV_FOURCC('D','X','1','1') /* 4:2:0  8 bpc */
#define GOTV_CODEC_D3D11_OPAQUE_10B      GOTV_FOURCC('D','X','1','0') /* 4:2:0 10 bpc */

/* CVPixelBuffer opaque buffer type */
#define GOTV_CODEC_CVPX_NV12       GOTV_FOURCC('C','V','P','N')
#define GOTV_CODEC_CVPX_UYVY       GOTV_FOURCC('C','V','P','Y')
#define GOTV_CODEC_CVPX_I420       GOTV_FOURCC('C','V','P','I')
#define GOTV_CODEC_CVPX_BGRA       GOTV_FOURCC('C','V','P','B')

/* Image codec (video) */
#define GOTV_CODEC_PNG             GOTV_FOURCC('p','n','g',' ')
#define GOTV_CODEC_PPM             GOTV_FOURCC('p','p','m',' ')
#define GOTV_CODEC_PGM             GOTV_FOURCC('p','g','m',' ')
#define GOTV_CODEC_PGMYUV          GOTV_FOURCC('p','g','m','y')
#define GOTV_CODEC_PAM             GOTV_FOURCC('p','a','m',' ')
#define GOTV_CODEC_JPEG            GOTV_FOURCC('j','p','e','g')
#define GOTV_CODEC_BPG             GOTV_FOURCC('B','P','G',0xFB)
#define GOTV_CODEC_JPEGLS          GOTV_FOURCC('M','J','L','S')
#define GOTV_CODEC_BMP             GOTV_FOURCC('b','m','p',' ')
#define GOTV_CODEC_TIFF            GOTV_FOURCC('t','i','f','f')
#define GOTV_CODEC_GIF             GOTV_FOURCC('g','i','f',' ')
#define GOTV_CODEC_TARGA           GOTV_FOURCC('t','g','a',' ')
#define GOTV_CODEC_SVG             GOTV_FOURCC('s','v','g',' ')
#define GOTV_CODEC_SGI             GOTV_FOURCC('s','g','i',' ')
#define GOTV_CODEC_PNM             GOTV_FOURCC('p','n','m',' ')
#define GOTV_CODEC_PCX             GOTV_FOURCC('p','c','x',' ')
#define GOTV_CODEC_XWD             GOTV_FOURCC('X','W','D',' ')
#define GOTV_CODEC_TXD             GOTV_FOURCC('T','X','D',' ')


/* Audio codec */
#define GOTV_CODEC_MPGA                       GOTV_FOURCC('m','p','g','a')
#define GOTV_CODEC_MP4A                       GOTV_FOURCC('m','p','4','a')
#define GOTV_CODEC_ALS                        GOTV_FOURCC('a','l','s',' ')
#define GOTV_CODEC_A52                        GOTV_FOURCC('a','5','2',' ')
#define GOTV_CODEC_EAC3                       GOTV_FOURCC('e','a','c','3')
#define GOTV_CODEC_DTS                        GOTV_FOURCC('d','t','s',' ')
#define GOTV_CODEC_WMA1                       GOTV_FOURCC('W','M','A','1')
#define GOTV_CODEC_WMA2                       GOTV_FOURCC('W','M','A','2')
#define GOTV_CODEC_WMAP                       GOTV_FOURCC('W','M','A','P')
#define GOTV_CODEC_WMAL                       GOTV_FOURCC('W','M','A','L')
#define GOTV_CODEC_WMAS                       GOTV_FOURCC('W','M','A','S')
#define GOTV_CODEC_FLAC                       GOTV_FOURCC('f','l','a','c')
#define GOTV_CODEC_MLP                        GOTV_FOURCC('m','l','p',' ')
#define GOTV_CODEC_TRUEHD                     GOTV_FOURCC('t','r','h','d')
#define GOTV_CODEC_DVAUDIO                    GOTV_FOURCC('d','v','a','u')
#define GOTV_CODEC_SPEEX                      GOTV_FOURCC('s','p','x',' ')
#define GOTV_CODEC_OPUS                       GOTV_FOURCC('O','p','u','s')
#define GOTV_CODEC_VORBIS                     GOTV_FOURCC('v','o','r','b')
#define GOTV_CODEC_MACE3                      GOTV_FOURCC('M','A','C','3')
#define GOTV_CODEC_MACE6                      GOTV_FOURCC('M','A','C','6')
#define GOTV_CODEC_MUSEPACK7                  GOTV_FOURCC('M','P','C',' ')
#define GOTV_CODEC_MUSEPACK8                  GOTV_FOURCC('M','P','C','K')
#define GOTV_CODEC_RA_144                     GOTV_FOURCC('1','4','_','4')
#define GOTV_CODEC_RA_288                     GOTV_FOURCC('2','8','_','8')
#define GOTV_CODEC_INTERPLAY_DPCM             GOTV_FOURCC('i','d','p','c')
#define GOTV_CODEC_ROQ_DPCM                   GOTV_FOURCC('R','o','Q','a')
#define GOTV_CODEC_DSICINAUDIO                GOTV_FOURCC('D','C','I','A')
#define GOTV_CODEC_ADPCM_4XM                  GOTV_FOURCC('4','x','m','a')
#define GOTV_CODEC_ADPCM_EA                   GOTV_FOURCC('A','D','E','A')
#define GOTV_CODEC_ADPCM_XA                   GOTV_FOURCC('x','a',' ',' ')
#define GOTV_CODEC_ADPCM_ADX                  GOTV_FOURCC('a','d','x',' ')
#define GOTV_CODEC_ADPCM_IMA_WS               GOTV_FOURCC('A','I','W','S')
#define GOTV_CODEC_ADPCM_G722                 GOTV_FOURCC('g','7','2','2')
#define GOTV_CODEC_ADPCM_G726                 GOTV_FOURCC('g','7','2','6')
#define GOTV_CODEC_ADPCM_SWF                  GOTV_FOURCC('S','W','F','a')
#define GOTV_CODEC_ADPCM_MS                   GOTV_FOURCC('m','s',0x00,0x02)
#define GOTV_CODEC_ADPCM_IMA_WAV              GOTV_FOURCC('m','s',0x00,0x11)
#define GOTV_CODEC_ADPCM_IMA_AMV              GOTV_FOURCC('i','m','a','v')
#define GOTV_CODEC_ADPCM_IMA_QT               GOTV_FOURCC('i','m','a','4')
#define GOTV_CODEC_ADPCM_YAMAHA               GOTV_FOURCC('m','s',0x00,0x20)
#define GOTV_CODEC_ADPCM_DK3                  GOTV_FOURCC('m','s',0x00,0x62)
#define GOTV_CODEC_ADPCM_DK4                  GOTV_FOURCC('m','s',0x00,0x61)
#define GOTV_CODEC_ADPCM_CREATIVE             GOTV_FOURCC('m','s',0x00,0xC0)
#define GOTV_CODEC_ADPCM_SBPRO_2              GOTV_FOURCC('m','s',0x00,0xC2)
#define GOTV_CODEC_ADPCM_SBPRO_3              GOTV_FOURCC('m','s',0x00,0xC3)
#define GOTV_CODEC_ADPCM_SBPRO_4              GOTV_FOURCC('m','s',0x00,0xC4)
#define GOTV_CODEC_ADPCM_THP                  GOTV_FOURCC('T','H','P','A')
#define GOTV_CODEC_ADPCM_XA_EA                GOTV_FOURCC('X','A','J', 0)
#define GOTV_CODEC_G723_1                     GOTV_FOURCC('g','7','2', 0x31)
#define GOTV_CODEC_G729                       GOTV_FOURCC('g','7','2','9')
#define GOTV_CODEC_VMDAUDIO                   GOTV_FOURCC('v','m','d','a')
#define GOTV_CODEC_AMR_NB                     GOTV_FOURCC('s','a','m','r')
#define GOTV_CODEC_AMR_WB                     GOTV_FOURCC('s','a','w','b')
#define GOTV_CODEC_ALAC                       GOTV_FOURCC('a','l','a','c')
#define GOTV_CODEC_QDM2                       GOTV_FOURCC('Q','D','M','2')
#define GOTV_CODEC_QDMC                       GOTV_FOURCC('Q','D','M','C')
#define GOTV_CODEC_COOK                       GOTV_FOURCC('c','o','o','k')
#define GOTV_CODEC_SIPR                       GOTV_FOURCC('s','i','p','r')
#define GOTV_CODEC_TTA                        GOTV_FOURCC('T','T','A','1')
#define GOTV_CODEC_SHORTEN                    GOTV_FOURCC('s','h','n',' ')
#define GOTV_CODEC_WAVPACK                    GOTV_FOURCC('W','V','P','K')
#define GOTV_CODEC_GSM                        GOTV_FOURCC('g','s','m',' ')
#define GOTV_CODEC_GSM_MS                     GOTV_FOURCC('a','g','s','m')
#define GOTV_CODEC_ATRAC1                     GOTV_FOURCC('a','t','r','1')
#define GOTV_CODEC_ATRAC3                     GOTV_FOURCC('a','t','r','c')
#define GOTV_CODEC_ATRAC3P                    GOTV_FOURCC('a','t','r','p')
#define GOTV_CODEC_IMC                        GOTV_FOURCC(0x1,0x4,0x0,0x0)
#define GOTV_CODEC_TRUESPEECH                 GOTV_FOURCC(0x22,0x0,0x0,0x0)
#define GOTV_CODEC_NELLYMOSER                 GOTV_FOURCC('N','E','L','L')
#define GOTV_CODEC_APE                        GOTV_FOURCC('A','P','E',' ')
#define GOTV_CODEC_QCELP                      GOTV_FOURCC('Q','c','l','p')
#define GOTV_CODEC_302M                       GOTV_FOURCC('3','0','2','m')
#define GOTV_CODEC_DVD_LPCM                   GOTV_FOURCC('l','p','c','m')
#define GOTV_CODEC_DVDA_LPCM                  GOTV_FOURCC('a','p','c','m')
#define GOTV_CODEC_BD_LPCM                    GOTV_FOURCC('b','p','c','m')
#define GOTV_CODEC_WIDI_LPCM                  GOTV_FOURCC('w','p','c','m')
#define GOTV_CODEC_SDDS                       GOTV_FOURCC('s','d','d','s')
#define GOTV_CODEC_MIDI                       GOTV_FOURCC('M','I','D','I')
#define GOTV_CODEC_RALF                       GOTV_FOURCC('R','A','L','F')

#define GOTV_CODEC_S8                         GOTV_FOURCC('s','8',' ',' ')
#define GOTV_CODEC_U8                         GOTV_FOURCC('u','8',' ',' ')
#define GOTV_CODEC_S16L                       GOTV_FOURCC('s','1','6','l')
#define GOTV_CODEC_S16L_PLANAR                GOTV_FOURCC('s','1','l','p')
#define GOTV_CODEC_S16B                       GOTV_FOURCC('s','1','6','b')
#define GOTV_CODEC_U16L                       GOTV_FOURCC('u','1','6','l')
#define GOTV_CODEC_U16B                       GOTV_FOURCC('u','1','6','b')
#define GOTV_CODEC_S20B                       GOTV_FOURCC('s','2','0','b')
#define GOTV_CODEC_S24L                       GOTV_FOURCC('s','2','4','l')
#define GOTV_CODEC_S24B                       GOTV_FOURCC('s','2','4','b')
#define GOTV_CODEC_U24L                       GOTV_FOURCC('u','2','4','l')
#define GOTV_CODEC_U24B                       GOTV_FOURCC('u','2','4','b')
#define GOTV_CODEC_S24L32                     GOTV_FOURCC('s','2','4','4')
#define GOTV_CODEC_S24B32                     GOTV_FOURCC('S','2','4','4')
#define GOTV_CODEC_S32L                       GOTV_FOURCC('s','3','2','l')
#define GOTV_CODEC_S32B                       GOTV_FOURCC('s','3','2','b')
#define GOTV_CODEC_U32L                       GOTV_FOURCC('u','3','2','l')
#define GOTV_CODEC_U32B                       GOTV_FOURCC('u','3','2','b')
#define GOTV_CODEC_F32L                       GOTV_FOURCC('f','3','2','l')
#define GOTV_CODEC_F32B                       GOTV_FOURCC('f','3','2','b')
#define GOTV_CODEC_F64L                       GOTV_FOURCC('f','6','4','l')
#define GOTV_CODEC_F64B                       GOTV_FOURCC('f','6','4','b')

#define GOTV_CODEC_ALAW                       GOTV_FOURCC('a','l','a','w')
#define GOTV_CODEC_MULAW                      GOTV_FOURCC('m','l','a','w')
#define GOTV_CODEC_DAT12                      GOTV_FOURCC('L','P','1','2')
#define GOTV_CODEC_S24DAUD                    GOTV_FOURCC('d','a','u','d')
#define GOTV_CODEC_TWINVQ                     GOTV_FOURCC('T','W','I','N')
#define GOTV_CODEC_BMVAUDIO                   GOTV_FOURCC('B','M','V','A')
#define GOTV_CODEC_ULEAD_DV_AUDIO_NTSC        GOTV_FOURCC('m','s',0x02,0x15)
#define GOTV_CODEC_ULEAD_DV_AUDIO_PAL         GOTV_FOURCC('m','s',0x02,0x16)
#define GOTV_CODEC_INDEO_AUDIO                GOTV_FOURCC('m','s',0x04,0x02)
#define GOTV_CODEC_METASOUND                  GOTV_FOURCC('m','s',0x00,0x75)
#define GOTV_CODEC_ON2AVC                     GOTV_FOURCC('m','s',0x05,0x00)
#define GOTV_CODEC_TAK                        GOTV_FOURCC('t','a','k',' ')
#define GOTV_CODEC_SMACKAUDIO                 GOTV_FOURCC('S','M','K','A')
#define GOTV_CODEC_ADPCM_IMA_EA_SEAD          GOTV_FOURCC('S','E','A','D')
#define GOTV_CODEC_ADPCM_EA_R1                GOTV_FOURCC('E','A','R','1')
#define GOTV_CODEC_ADPCM_IMA_APC              GOTV_FOURCC('A','I','P','C')

/* Subtitle */
#define GOTV_CODEC_SPU       GOTV_FOURCC('s','p','u',' ')
#define GOTV_CODEC_DVBS      GOTV_FOURCC('d','v','b','s')
#define GOTV_CODEC_SUBT      GOTV_FOURCC('s','u','b','t')
#define GOTV_CODEC_XSUB      GOTV_FOURCC('X','S','U','B')
#define GOTV_CODEC_SSA       GOTV_FOURCC('s','s','a',' ')
#define GOTV_CODEC_TEXT      GOTV_FOURCC('T','E','X','T')
#define GOTV_CODEC_TELETEXT  GOTV_FOURCC('t','e','l','x')
#define GOTV_CODEC_KATE      GOTV_FOURCC('k','a','t','e')
#define GOTV_CODEC_CMML      GOTV_FOURCC('c','m','m','l')
#define GOTV_CODEC_ITU_T140  GOTV_FOURCC('t','1','4','0')
#define GOTV_CODEC_USF       GOTV_FOURCC('u','s','f',' ')
#define GOTV_CODEC_OGT       GOTV_FOURCC('o','g','t',' ')
#define GOTV_CODEC_CVD       GOTV_FOURCC('c','v','d',' ')
#define GOTV_CODEC_TX3G      GOTV_FOURCC('t','x','3','g')
#define GOTV_CODEC_ARIB_A    GOTV_FOURCC('a','r','b','a')
#define GOTV_CODEC_ARIB_C    GOTV_FOURCC('a','r','b','c')
/* Blu-ray Presentation Graphics */
#define GOTV_CODEC_BD_PG     GOTV_FOURCC('b','d','p','g')
#define GOTV_CODEC_BD_TEXT   GOTV_FOURCC('b','d','t','x')
/* EBU STL (TECH. 3264-E) */
#define GOTV_CODEC_EBU_STL   GOTV_FOURCC('S','T','L',' ')
#define GOTV_CODEC_SCTE_18   GOTV_FOURCC('S','C','1','8')
#define GOTV_CODEC_SCTE_27   GOTV_FOURCC('S','C','2','7')
/* EIA/CEA-608/708 */
#define GOTV_CODEC_CEA608    GOTV_FOURCC('c','6','0','8')
#define GOTV_CODEC_CEA708    GOTV_FOURCC('c','7','0','8')
#define GOTV_CODEC_TTML      GOTV_FOURCC('T','T','M','L')
#define GOTV_CODEC_WEBVTT    GOTV_FOURCC('w','v','t','t')

/* XYZ colorspace 12 bits packed in 16 bits, organisation |XXX0|YYY0|ZZZ0| */
#define GOTV_CODEC_XYZ12     GOTV_FOURCC('X','Y','1','2')


/* Special endian dependent values
 * The suffic N means Native
 * The suffix I means Inverted (ie non native) */
#ifdef WORDS_BIGENDIAN
#   define GOTV_CODEC_S16N GOTV_CODEC_S16B
#   define GOTV_CODEC_U16N GOTV_CODEC_U16B
#   define GOTV_CODEC_S24N GOTV_CODEC_S24B
#   define GOTV_CODEC_U24N GOTV_CODEC_U24B
#   define GOTV_CODEC_S32N GOTV_CODEC_S32B
#   define GOTV_CODEC_U32N GOTV_CODEC_U32B
#   define GOTV_CODEC_FL32 GOTV_CODEC_F32B
#   define GOTV_CODEC_FL64 GOTV_CODEC_F64B

#   define GOTV_CODEC_S16I GOTV_CODEC_S16L
#   define GOTV_CODEC_U16I GOTV_CODEC_U16L
#   define GOTV_CODEC_S24I GOTV_CODEC_S24L
#   define GOTV_CODEC_U24I GOTV_CODEC_U24L
#   define GOTV_CODEC_S32I GOTV_CODEC_S32L
#   define GOTV_CODEC_U32I GOTV_CODEC_U32L

#else
#   define GOTV_CODEC_S16N GOTV_CODEC_S16L
#   define GOTV_CODEC_U16N GOTV_CODEC_U16L
#   define GOTV_CODEC_S24N GOTV_CODEC_S24L
#   define GOTV_CODEC_U24N GOTV_CODEC_U24L
#   define GOTV_CODEC_S32N GOTV_CODEC_S32L
#   define GOTV_CODEC_U32N GOTV_CODEC_U32L
#   define GOTV_CODEC_FL32 GOTV_CODEC_F32L
#   define GOTV_CODEC_FL64 GOTV_CODEC_F64L

#   define GOTV_CODEC_S16I GOTV_CODEC_S16B
#   define GOTV_CODEC_U16I GOTV_CODEC_U16B
#   define GOTV_CODEC_S24I GOTV_CODEC_S24B
#   define GOTV_CODEC_U24I GOTV_CODEC_U24B
#   define GOTV_CODEC_S32I GOTV_CODEC_S32B
#   define GOTV_CODEC_U32I GOTV_CODEC_U32B
#endif

/* Non official codecs, used to force a profile in an encoder */
/* MPEG-1 video */
#define GOTV_CODEC_MP1V      GOTV_FOURCC('m','p','1','v')
/* MPEG-2 video */
#define GOTV_CODEC_MP2V      GOTV_FOURCC('m','p','2','v')
/* MPEG-I/II layer 2 audio */
#define GOTV_CODEC_MP2       GOTV_FOURCC('m','p','2',' ')
/* MPEG-I/II layer 3 audio */
#define GOTV_CODEC_MP3       GOTV_FOURCC('m','p','3',' ')

/**
 * It returns the codec associated to a fourcc within an ES category.
 *
 * If not found, it will return the given fourcc.
 * If found, it will always be one of the GOTV_CODEC_ defined above.
 *
 * You may use UNKNOWN_ES for the ES category if you don't have the information.
 */
NLC_API gotvptop_fourcc_t gotvptop_fourcc_GetCodec( int i_cat, gotvptop_fourcc_t i_fourcc );

/**
 * It returns the codec associated to a fourcc stored in a zero terminated
 * string.
 *
 * If the string is NULL or does not have exactly 4 characters, it will
 * return 0, otherwise it behaves like gotvptop_fourcc_GetCodec.
 *
 * Provided for convenience.
 */
NLC_API gotvptop_fourcc_t gotvptop_fourcc_GetCodecFromString( int i_cat, const char * );

/**
 * It converts the given fourcc to an audio codec when possible.
 *
 * The fourccs converted are aflt, araw/pcm , twos, sowt. When an incompatible i_bits
 * is detected, 0 is returned.
 * The other fourccs go through gotvptop_fourcc_GetCodec and i_bits is not checked.
 */
NLC_API gotvptop_fourcc_t gotvptop_fourcc_GetCodecAudio( gotvptop_fourcc_t i_fourcc, int i_bits );

/**
 * It returns the description of the given fourcc or NULL if not found.
 *
 * You may use UNKNOWN_ES for the ES category if you don't have the information.
 */
NLC_API const char * gotvptop_fourcc_GetDescription( int i_cat, gotvptop_fourcc_t i_fourcc );

/**
 * It returns a list (terminated with the value 0) of YUV fourccs in
 * decreasing priority order for the given chroma.
 *
 * It will always return a non NULL pointer that must not be freed.
 */
NLC_API const gotvptop_fourcc_t * gotvptop_fourcc_GetYUVFallback( gotvptop_fourcc_t );

/**
 * It returns a list (terminated with the value 0) of RGB fourccs in
 * decreasing priority order for the given chroma.
 *
 * It will always return a non NULL pointer that must not be freed.
 */
NLC_API const gotvptop_fourcc_t * gotvptop_fourcc_GetRGBFallback( gotvptop_fourcc_t );

/**
 * It returns true if the given fourcc is YUV and false otherwise.
 */
NLC_API bool gotvptop_fourcc_IsYUV( gotvptop_fourcc_t );

/**
 * It returns true if the two fourccs are equivalent if their U&V planes are
 * swapped.
 */
NLC_API bool gotvptop_fourcc_AreUVPlanesSwapped(gotvptop_fourcc_t , gotvptop_fourcc_t );

/**
 * Chroma related information.
 */
typedef struct {
    unsigned plane_count;
    struct {
        gotvptop_rational_t w;
        gotvptop_rational_t h;
    } p[4];
    unsigned pixel_size;        /* Number of bytes per pixel for a plane */
    unsigned pixel_bits;        /* Number of bits actually used bits per pixel for a plane */
} gotvptop_chroma_description_t;

/**
 * It returns a gotvptop_chroma_description_t describing the requested fourcc or NULL
 * if not found.
 */
NLC_API const gotvptop_chroma_description_t * gotvptop_fourcc_GetChromaDescription( gotvptop_fourcc_t fourcc ) GOTV_USED;

#endif /* _GOTV_FOURCC_H */


/*
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <ffmpeg/config_ffmpeg.h>
#include <stdint.h>
#include <string.h>

#include <ffmpeg/config_ffmpeg.h>
#include "libavutil/avutil_attributes.h"
#include "avcodec.h"
#include "blockdsp.h"
#include "version.h"

static void clear_block_c(int16_t *block)
{
    memset(block, 0, sizeof(int16_t) * 64);
}

static void clear_blocks_c(int16_t *blocks)
{
    memset(blocks, 0, sizeof(int16_t) * 6 * 64);
}

static void fill_block16_c(uint8_t *block, uint8_t value, ptrdiff_t line_size,
                           int h)
{
    int i;

    for (i = 0; i < h; i++) {
        memset(block, value, 16);
        block += line_size;
    }
}

static void fill_block8_c(uint8_t *block, uint8_t value, ptrdiff_t line_size,
                          int h)
{
    int i;

    for (i = 0; i < h; i++) {
        memset(block, value, 8);
        block += line_size;
    }
}

av_cold void ff_blockdsp_init(BlockDSPContext *c, AVCodecContext *avctx)
{
    c->clear_block  = clear_block_c;
    c->clear_blocks = clear_blocks_c;

    c->fill_block_tab[0] = fill_block16_c;
    c->fill_block_tab[1] = fill_block8_c;

#if ARCH_ALPHA
		ff_blockdsp_init_alpha( c );
#endif // ARCH_ALPHA
#if ARCH_ARM
		ff_blockdsp_init_arm( c );
#endif // ARCH_ARM
#if ARCH_PPC
		ff_blockdsp_init_ppc( c );
#endif //ARCH_PPC
#if ARCH_X86 && HAVE_X86ASM
		ff_blockdsp_init_x86( c, avctx );
#endif // ARCH_X86
#if ARCH_MIPS
		ff_blockdsp_init_mips( c );
#endif // ARCH_MIPS
}

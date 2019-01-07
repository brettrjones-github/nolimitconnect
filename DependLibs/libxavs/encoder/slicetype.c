/*****************************************************************************
 * slicetype.c: h264 encoder library
 *****************************************************************************
 * Copyright (C) 2005-2008 x264 project
 *
 * Authors: Jason Garrett-Glaser <darkshikari@gmail.com>
 *          Loren Merritt <lorenm@u.washington.edu>
 *          Dylan Yudaken <dyudaken@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111, USA.
 *****************************************************************************/

/*****************************************************************************
 * slicetype.c: xavs encoder library
 *****************************************************************************
 * Copyright (C) 2009~2010 xavs project
 * Authors: Jianwen Chen <jianwen.chen.video@gmail.com>
 * This code is modified on x264 project and will follow the license of x264
 *****************************************************************************/

#include <string.h>
#include <math.h>
#include <limits.h>

#include "common/common.h"
#include "common/cpu.h"
#include "macroblock.h"
#include "me.h"


static void
xavs_lowres_context_init (xavs_t * h, xavs_mb_analysis_t * a)
{
  a->i_qp = 12;                 // arbitrary, but low because SATD scores are 1/4 normal
  a->i_lambda = i_qp0_cost_table[a->i_qp];
  xavs_mb_analyse_load_costs (h, a);
  h->mb.i_me_method = XAVS_MIN (XAVS_ME_HEX, h->param.analyse.i_me_method);     // maybe dia?
  h->mb.i_subpel_refine = 4;    // 3 should be enough, but not tweaking for speed now
  h->mb.b_chroma_me = 0;
}

int
xavs_slicetype_mb_cost (xavs_t * h, xavs_mb_analysis_t * a, xavs_frame_t ** frames, int p0, int p1, int b, int dist_scale_factor)
{
  xavs_frame_t *fref0 = frames[p0];
  xavs_frame_t *fref1 = frames[p1];
  xavs_frame_t *fenc = frames[b];
  const int b_bidir = (b < p1);
  const int i_mb_x = h->mb.i_mb_x;
  const int i_mb_y = h->mb.i_mb_y;
  const int i_mb_stride = h->sps->i_mb_width;
  const int i_mb_xy = i_mb_x + i_mb_y * i_mb_stride;
  const int i_stride = fenc->i_stride_lowres;
  const int i_pel_offset = 8 * (i_mb_x + i_mb_y * i_stride);

  uint8_t pix1[9 * 9], pix2[8 * 8];
  xavs_me_t m[2];
  int i_bcost = COST_MAX;
  int i_cost_bak;
  int l, i;

  h->mb.pic.p_fenc[0] = h->mb.pic.fenc_buf;
  h->mc.copy[PIXEL_8x8] (h->mb.pic.p_fenc[0], FENC_STRIDE, &fenc->lowres[0][i_pel_offset], i_stride, 8);

  if (!p0 && !p1 && !b)
    goto lowres_intra_mb;

  // no need for h->mb.mv_min[]
  h->mb.mv_min_fpel[0] = -8 * h->mb.i_mb_x - 4;
  h->mb.mv_max_fpel[0] = 8 * (h->sps->i_mb_width - h->mb.i_mb_x - 1) + 4;
  h->mb.mv_min_spel[0] = 4 * (h->mb.mv_min_fpel[0] - 8);
  h->mb.mv_max_spel[0] = 4 * (h->mb.mv_max_fpel[0] + 8);
  if (h->mb.i_mb_x <= 1)
  {
    h->mb.mv_min_fpel[1] = -8 * h->mb.i_mb_y - 4;
    h->mb.mv_max_fpel[1] = 8 * (h->sps->i_mb_height - h->mb.i_mb_y - 1) + 4;
    h->mb.mv_min_spel[1] = 4 * (h->mb.mv_min_fpel[1] - 8);
    h->mb.mv_max_spel[1] = 4 * (h->mb.mv_max_fpel[1] + 8);
  }

#define LOAD_HPELS_LUMA(dst, src) \
    { \
        (dst)[0] = &(src)[0][i_pel_offset]; \
        (dst)[1] = &(src)[1][i_pel_offset]; \
        (dst)[2] = &(src)[2][i_pel_offset]; \
        (dst)[3] = &(src)[3][i_pel_offset]; \
    }
#define SAVE_MVS( mv0, mv1 ) \
    { \
        fenc->mv[0][i_mb_xy][0] = mv0[0]; \
        fenc->mv[0][i_mb_xy][1] = mv0[1]; \
        if( b_bidir ) \
        { \
            fenc->mv[1][i_mb_xy][0] = mv1[0]; \
            fenc->mv[1][i_mb_xy][1] = mv1[1]; \
        } \
    }
#define CLIP_MV( mv ) \
    { \
        mv[0] = xavs_clip3( mv[0], h->mb.mv_min_spel[0], h->mb.mv_max_spel[0] ); \
        mv[1] = xavs_clip3( mv[1], h->mb.mv_min_spel[1], h->mb.mv_max_spel[1] ); \
    }
#define TRY_BIDIR( mv0, mv1, penalty ) \
    { \
        int stride2 = 8; \
        uint8_t *src2; \
        int i_cost; \
        h->mc.mc_luma( m[0].p_fref, m[0].i_stride[0], pix1, 8, \
                       (mv0)[0], (mv0)[1], 8, 8 ); \
        src2 = h->mc.get_ref( m[1].p_fref, m[1].i_stride[0], pix2, &stride2, \
                       (mv1)[0], (mv1)[1], 8, 8 ); \
        h->mc.avg[PIXEL_8x8]( pix1, 8, src2, stride2 ); \
        i_cost = penalty + h->pixf.mbcmp[PIXEL_8x8]( \
                           m[0].p_fenc[0], FENC_STRIDE, pix1, 8 ); \
        if( i_bcost > i_cost ) \
        { \
            i_bcost = i_cost; \
            SAVE_MVS( mv0, mv1 ); \
        } \
    }

  m[0].i_pixel = PIXEL_8x8;
  m[0].p_cost_mv = a->p_cost_mv;
  m[0].i_stride[0] = i_stride;
  m[0].p_fenc[0] = h->mb.pic.p_fenc[0];
  LOAD_HPELS_LUMA (m[0].p_fref, fref0->lowres);

  if (b_bidir)
  {
    int16_t *mvr = fref1->mv[0][i_mb_xy];
    int dmv[2][2];
    int mv0[2] = { 0, 0 };

    m[1] = m[0];
    LOAD_HPELS_LUMA (m[1].p_fref, fref1->lowres);

    dmv[0][0] = (mvr[0] * dist_scale_factor + 128) >> 8;
    dmv[0][1] = (mvr[1] * dist_scale_factor + 128) >> 8;
    dmv[1][0] = dmv[0][0] - mvr[0];
    dmv[1][1] = dmv[0][1] - mvr[1];
    CLIP_MV (dmv[0]);
    CLIP_MV (dmv[1]);

    TRY_BIDIR (dmv[0], dmv[1], 0);
    if (dmv[0][0] || dmv[0][1] || dmv[1][0] || dmv[1][1])
      TRY_BIDIR (mv0, mv0, 0);
//      if( i_bcost < 60 ) // arbitrary threshold
//          return i_bcost;
  }

  i_cost_bak = i_bcost;
  for (l = 0; l < 1 + b_bidir; l++)
  {
    int mvc[4][2] = { {0} }, i_mvc;
    int16_t (*fenc_mv)[2] = &fenc->mv[l][i_mb_xy];
    i_mvc = 0;
    if (i_mb_x > 0)
    {
      mvc[i_mvc][0] = fenc_mv[-1][0];
      mvc[i_mvc][1] = fenc_mv[-1][1];
      i_mvc++;
    }
    if (i_mb_y > 0)
    {
      mvc[i_mvc][0] = fenc_mv[-i_mb_stride][0];
      mvc[i_mvc][1] = fenc_mv[-i_mb_stride][1];
      i_mvc++;
      if (i_mb_x < h->sps->i_mb_width - 1)
      {
        mvc[i_mvc][0] = fenc_mv[-i_mb_stride + 1][0];
        mvc[i_mvc][1] = fenc_mv[-i_mb_stride + 1][1];
        i_mvc++;
      }
      if (i_mb_x > 0)
      {
        mvc[i_mvc][0] = fenc_mv[-i_mb_stride - 1][0];
        mvc[i_mvc][1] = fenc_mv[-i_mb_stride - 1][1];
        i_mvc++;
      }
    }
    m[l].mvp[0] = xavs_median (mvc[0][0], mvc[1][0], mvc[2][0]);
    m[l].mvp[1] = xavs_median (mvc[0][1], mvc[1][1], mvc[2][1]);

    xavs_me_search (h, &m[l], mvc, i_mvc);

    i_bcost = XAVS_MIN (i_bcost, m[l].cost + 3);
  }

  if (b_bidir && (m[0].mv[0] || m[0].mv[1] || m[1].mv[0] || m[1].mv[1]))
    TRY_BIDIR (m[0].mv, m[1].mv, 5);

  if (i_bcost < i_cost_bak)
    SAVE_MVS (m[0].mv, m[1].mv);

lowres_intra_mb:
  {
    uint8_t pix_buf[9 * FDEC_STRIDE];
    uint8_t *pix = &pix_buf[8 + FDEC_STRIDE - 1];
    uint8_t *src = &fenc->lowres[0][i_pel_offset - 1];
    int intra_penalty = 5 + 10 * b_bidir;
    i_cost_bak = i_bcost;

    memcpy (pix - FDEC_STRIDE, src - i_stride, 9);
    for (i = 0; i < 8; i++)
      pix[i * FDEC_STRIDE] = src[i * i_stride];
    pix++;

    for (i = I_PRED_CHROMA_DC; i <= I_PRED_CHROMA_P; i++)
    {
      int i_cost;
      h->predict_8x8c[i] (pix, h->mb.i_neighbour);
      i_cost = h->pixf.mbcmp[PIXEL_8x8] (pix, FDEC_STRIDE, h->mb.pic.p_fenc[0], FENC_STRIDE) + intra_penalty;
      i_bcost = XAVS_MIN (i_bcost, i_cost);
    }
    if (i_bcost != i_cost_bak)
    {
      if (!b_bidir && i_mb_x > 0 && i_mb_x < h->sps->i_mb_width - 1 && i_mb_y > 0 && i_mb_y < h->sps->i_mb_height - 1)
      {
        fenc->i_intra_mbs[b - p0]++;
      }
      if (p1 > p0 + 1)
        i_bcost = i_bcost * 9 / 8;      // arbitray penalty for I-blocks in and after B-frames
    }
  }

  return i_bcost;
}

#undef TRY_BIDIR
#undef SAVE_MVS

int
xavs_slicetype_frame_cost (xavs_t * h, xavs_mb_analysis_t * a, xavs_frame_t ** frames, int p0, int p1, int b)
{
  int i_score = 0;
  int dist_scale_factor = 128;
  int *row_satd = frames[b]->i_row_satds[b - p0][p1 - b];

  /* Check whether we already evaluated this frame
   * If we have tried this frame as P, then we have also tried
   * the preceding frames as B. (is this still true?) */
  if (frames[b]->i_cost_est[b - p0][p1 - b] >= 0)
    return frames[b]->i_cost_est[b - p0][p1 - b];

  /* Init MVs so that we don't have to check edge conditions when loading predictors. */
  /* FIXME: not needed every time */
  memset (frames[b]->mv[0], 0, h->sps->i_mb_height * h->sps->i_mb_width * 2 * sizeof (int16_t));
  if (b != p1)
    memset (frames[b]->mv[1], 0, h->sps->i_mb_height * h->sps->i_mb_width * 2 * sizeof (int16_t));

  if (b == p1)
    frames[b]->i_intra_mbs[b - p0] = 0;
  if (p1 != p0)
    dist_scale_factor = (((b - p0) << 8) + ((p1 - p0) >> 1)) / (p1 - p0);

  /* the edge mbs seem to reduce the predictive quality of the
   * whole frame's score, but are needed for a spatial distribution. */
  if (h->param.rc.i_vbv_buffer_size)
  {
    for (h->mb.i_mb_y = 0; h->mb.i_mb_y < h->sps->i_mb_height; h->mb.i_mb_y++)
    {
      row_satd[h->mb.i_mb_y] = 0;
      for (h->mb.i_mb_x = 0; h->mb.i_mb_x < h->sps->i_mb_width; h->mb.i_mb_x++)
      {
        int i_mb_cost = xavs_slicetype_mb_cost (h, a, frames, p0, p1, b, dist_scale_factor);
        row_satd[h->mb.i_mb_y] += i_mb_cost;
        if (h->mb.i_mb_y > 0 && h->mb.i_mb_y < h->sps->i_mb_height - 1 && h->mb.i_mb_x > 0 && h->mb.i_mb_x < h->sps->i_mb_width - 1)
        {
          i_score += i_mb_cost;
        }
      }
    }
  }
  else
  {
    for (h->mb.i_mb_y = 1; h->mb.i_mb_y < h->sps->i_mb_height - 1; h->mb.i_mb_y++)
      for (h->mb.i_mb_x = 1; h->mb.i_mb_x < h->sps->i_mb_width - 1; h->mb.i_mb_x++)
        i_score += xavs_slicetype_mb_cost (h, a, frames, p0, p1, b, dist_scale_factor);
  }

  if (b != p1)
    i_score = i_score * 100 / (120 + h->param.i_bframe_bias);

  frames[b]->i_cost_est[b - p0][p1 - b] = i_score;
//  fprintf( stderr, "frm %d %c(%d,%d): %6d I:%d  \n", frames[b]->i_frame,
//           (p1==0?'I':b<p1?'B':'P'), b-p0, p1-b, i_score, frames[b]->i_intra_mbs[b-p0] );
  xavs_cpu_restore (h->param.cpu);
  return i_score;
}

void
xavs_slicetype_analyse (xavs_t * h)
{
  xavs_mb_analysis_t a;
  xavs_frame_t *frames[XAVS_BFRAME_MAX + 3] = { NULL, };
  int num_frames;
  int keyint_limit;
  int j;
  int i_mb_count = (h->sps->i_mb_width - 2) * (h->sps->i_mb_height - 2);
  int cost1p0, cost2p0, cost1b1, cost2p1;
  int i_max_search = XAVS_MIN( h->lookahead->next.i_size, XAVS_LOOKAHEAD_MAX );

  if (!h->lookahead->last_nonb)
    return;
  frames[0] = h->lookahead->last_nonb;
  for (j = 0; j < i_max_search && h->lookahead->next.list[j]; j++)
    frames[j + 1] = h->lookahead->next.list[j];
  keyint_limit = h->param.i_keyint_max - frames[0]->i_frame + h->lookahead->i_last_idr - 1;
  num_frames = XAVS_MIN (j, keyint_limit);
  if (num_frames == 0)
    return;
  if (num_frames == 1)
  {
  no_b_frames:
    frames[1]->i_type = XAVS_TYPE_P;
    return;
  }

  xavs_lowres_context_init (h, &a);

  cost2p1 = xavs_slicetype_frame_cost (h, &a, frames, 0, 2, 2);
  if (frames[2]->i_intra_mbs[2] > i_mb_count / 2)
    goto no_b_frames;

  cost1b1 = xavs_slicetype_frame_cost (h, &a, frames, 0, 2, 1);
  cost1p0 = xavs_slicetype_frame_cost (h, &a, frames, 0, 1, 1);
  cost2p0 = xavs_slicetype_frame_cost (h, &a, frames, 1, 2, 2);
//  fprintf( stderr, "PP: %d + %d <=> BP: %d + %d \n",
//           cost1p0, cost2p0, cost1b1, cost2p1 );
  if (cost1p0 + cost2p0 < cost1b1 + cost2p1)
    goto no_b_frames;

// arbitrary and untuned
#define INTER_THRESH 300
#define P_SENS_BIAS (50 - h->param.i_bframe_bias)
  frames[1]->i_type = XAVS_TYPE_B;

  for (j = 2; j <= XAVS_MIN (h->param.i_bframe, num_frames - 1); j++)
  {
    int pthresh = XAVS_MAX (INTER_THRESH - P_SENS_BIAS * (j - 1), INTER_THRESH / 10);
    int pcost = xavs_slicetype_frame_cost (h, &a, frames, 0, j + 1, j + 1);
//      fprintf( stderr, "frm%d+%d: %d <=> %d, I:%d/%d \n",
//               frames[0]->i_frame, j-1, pthresh, pcost/i_mb_count,
//               frames[j+1]->i_intra_mbs[j+1], i_mb_count );
    if (pcost > pthresh * i_mb_count || frames[j + 1]->i_intra_mbs[j + 1] > i_mb_count / 3)
    {
      frames[j]->i_type = XAVS_TYPE_P;
      break;
    }
    else
      frames[j]->i_type = XAVS_TYPE_B;
  }
}

void
xavs_slicetype_decide (xavs_t * h)
{
  xavs_frame_t *frames[XAVS_BFRAME_MAX+2];
  xavs_frame_t *frm;
  int bframes;
  int i;
  int i_coded;

  if (h->lookahead->next.list[0] == NULL)
    return;

  if (h->param.rc.b_stat_read)
  {
    /* Use the frame types from the first pass */
    for (i = 0; i < h->lookahead->next.i_size; i++)
        h->lookahead->next.list[i]->i_type = 
                    xavs_ratecontrol_slice_type (h, h->lookahead->next.list[i]->i_frame);
  }
  else if ((h->param.i_bframe && h->param.i_bframe_adaptive) || 
           (h->param.rc.i_vbv_buffer_size && h->param.rc.i_lookahead) )
    xavs_slicetype_analyse (h);

  for (bframes = 0;; bframes++)
  {
    frm = h->lookahead->next.list[bframes];

    /* Limit GOP size */
    if (frm->i_frame - h->lookahead->i_last_idr >= h->param.i_keyint_max)
    {
      if (frm->i_type == XAVS_TYPE_AUTO)
        frm->i_type = XAVS_TYPE_IDR;
      if (frm->i_type != XAVS_TYPE_IDR)
        xavs_log (h, XAVS_LOG_ERROR, "specified frame type (%d) is not compatible with keyframe interval\n", frm->i_type);
    }
    if (frm->i_type == XAVS_TYPE_IDR)
    {
      /* Close GOP */
      h->lookahead->i_last_idr = frm->i_frame;
      if (bframes > 0)
      {
        bframes--;
        h->lookahead->next.list[bframes]->i_type = XAVS_TYPE_P;
      }
    }

    if (bframes == h->param.i_bframe || h->lookahead->next.list[bframes + 1] == NULL)
    {
      if (IS_XAVS_TYPE_B (frm->i_type))
        xavs_log (h, XAVS_LOG_ERROR, "specified frame type is not compatible with max B-frames\n");
      if (frm->i_type == XAVS_TYPE_AUTO || IS_XAVS_TYPE_B (frm->i_type))
        frm->i_type = XAVS_TYPE_P;
    }

    if (frm->i_type != XAVS_TYPE_AUTO && frm->i_type != XAVS_TYPE_B && frm->i_type != XAVS_TYPE_BREF)
      break;

    frm->i_type = XAVS_TYPE_B;
  }

  if (bframes)
    h->lookahead->next.list[bframes-1]->b_last_minigop_bframe = 1;
  h->lookahead->next.list[bframes]->i_bframes = bframes;
  
  /* shift sequence to coded order.
     use a small temporary list to avoid shifting the entire next buffer around */
  /*int*/
  i_coded = h->lookahead->next.list[0]->i_frame;
  if( bframes )
  {
      for( i = 0; i < bframes; i++ )
      {
          frames[i+1] = h->lookahead->next.list[i];
      }
      frames[0] = h->lookahead->next.list[bframes];
      memcpy( h->lookahead->next.list, frames, (bframes+1) * sizeof(xavs_frame_t*) );
  }

}

int
xavs_rc_analyse_slice (xavs_t * h)
{
  xavs_mb_analysis_t a;
  xavs_frame_t *frames[XAVS_BFRAME_MAX + 2] = { NULL, };
  int p0 = 0, p1, b;
  int cost;

  xavs_lowres_context_init (h, &a);

  if (IS_XAVS_TYPE_I (h->fenc->i_type))
  {
    p1 = b = 0;
  }
  else if (XAVS_TYPE_P == h->fenc->i_type)
  {
    p1 = 0;
    while (h->frames.current[p1] && IS_XAVS_TYPE_B (h->frames.current[p1]->i_type))
      p1++;
    p1++;
    b = p1;
  }
  else                          //B
  {
    p1 = (h->fref1[0]->i_poc - h->fref0[0]->i_poc) / 2;
    b = (h->fref1[0]->i_poc - h->fenc->i_poc) / 2;
    frames[p1] = h->fref1[0];
  }
  frames[p0] = h->fref0[0];
  frames[b] = h->fenc;

  cost = xavs_slicetype_frame_cost (h, &a, frames, p0, p1, b);
  h->fenc->i_row_satd = h->fenc->i_row_satds[b - p0][p1 - b];
  h->fdec->i_row_satd = h->fdec->i_row_satds[b - p0][p1 - b];
  h->fdec->i_satd = cost;
  memcpy (h->fdec->i_row_satd, h->fenc->i_row_satd, h->sps->i_mb_height * sizeof (int));
  return cost;
}

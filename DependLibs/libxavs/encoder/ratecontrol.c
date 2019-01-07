/*****************************************************************************
 * ratecontrol.c: h264 encoder library (Rate Control)
 *****************************************************************************
 * Copyright (C) 2005-2008 x264 project
 *
 * Authors: Loren Merritt <lorenm@u.washington.edu>
 *          Michael Niedermayer <michaelni@gmx.at>
 *          Gabriel Bouvigne <gabriel.bouvigne@joost.com>
 *          Jason Garrett-Glaser <darkshikari@gmail.com>
 *          Måns Rullgård <mru@mru.ath.cx>
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
 * ratecontrol.c: xavs encoder library (Rate Control)
 *****************************************************************************
 * Copyright (C) 2009~2010 xavs project
 * Authors: Jianwen Chen <jianwen.chen.video@gmail.com>
 * This code is modified on x264 project and will follow the license of x264
 *****************************************************************************/

#define _ISOC99_SOURCE
#undef NDEBUG                   // always check asserts, the speed effect is far too small to disable them


#include "libxavs/common/common.h"
#include <math.h>
#include <limits.h>
#include <assert.h>

#include "libxavs/common/cpu.h"
#include "ratecontrol.h"

typedef struct
{
  int pict_type;
  int kept_as_ref;
  float qscale;
  int mv_bits;
  int tex_bits;
  int misc_bits;
  uint64_t expected_bits;       /*total expected bits up to the current frame (current one excluded) */
  double expected_vbv;
  float new_qscale;
  int new_qp;
  int i_count;
  int p_count;
  int s_count;
  float blurred_complexity;
  char direct_mode;
} ratecontrol_entry_t;

typedef struct
{
  double coeff;
  double count;
  double decay;
  double offset;
} predictor_t;

struct xavs_ratecontrol_t
{
  /* constants */
  int b_abr;
  int b_2pass;
  int b_vbv;
  int b_vbv_min_rate;
  double fps;
  double bitrate;
  double rate_tolerance;
  double qcompress;
  int nmb;                      /* number of macroblocks in a frame */
  int qp_constant[3];

  /* current frame */
  ratecontrol_entry_t *rce;
  int qp;                       /* qp for current frame */
  int qpm;                      /* qp for current macroblock */
  float f_qpm;                  /* qp for current macroblock: precise float for AQ */
  float qpa_rc;                 /* average of macroblocks' qp before aq */
  float qpa_aq;                 /* average of macroblocks' qp after aq */
  int qp_force;

  /* VBV stuff */
  double buffer_size;
  double buffer_fill_final;     /* real buffer as of the last finished frame */
  double buffer_fill;           /* planned buffer, if all in-progress frames hit their bit budget */
  double buffer_rate;           /* # of bits added to buffer_fill after each frame */
  predictor_t *pred;            /* predict frame size from satd */

  /* ABR stuff */
  int last_satd;
  double last_rceq;
  double cplxr_sum;             /* sum of bits*qscale/rceq */
  double expected_bits_sum;     /* sum of qscale2bits after rceq, ratefactor, and overflow, only includes finished frames */
  double wanted_bits_window;    /* target bitrate * window */
  double cbr_decay;
  double short_term_cplxsum;
  double short_term_cplxcount;
  double rate_factor_constant;
  double ip_offset;
  double pb_offset;

  /* 2pass stuff */
  FILE *p_stat_file_out;
  char *psz_stat_file_tmpname;
  FILE *p_mbtree_stat_file_out;
  char *psz_mbtree_stat_file_tmpname;
  char *psz_mbtree_stat_file_name;
  FILE *p_mbtree_stat_file_in;

  int num_entries;              /* number of ratecontrol_entry_ts */
  ratecontrol_entry_t *entry;   /* FIXME: copy needed data and free this once init is done */
  double last_qscale;
  double last_qscale_for[5];    /* last qscale for a specific pict type, used for max_diff & ipb factor stuff  */
  int last_non_b_pict_type;
  double accum_p_qp;            /* for determining I-frame quant */
  double accum_p_norm;
  double last_accum_p_norm;
  double lmin[5];               /* min qscale by frame type */
  double lmax[5];
  double lstep;                 /* max change (multiply) in qscale per frame */
  uint16_t *qp_buffer;          /* Global buffer for converting MB-tree quantizer data. */

  /* MBRC stuff */
  double frame_size_estimated;
  double frame_size_planned;
  predictor_t *row_pred;
  predictor_t row_preds[5];
  predictor_t *pred_b_from_p;   /* predict B-frame size from P-frame satd */
  int bframes;                  /* # consecutive B-frames before this P-frame */
  int bframe_bits;              /* total cost of those frames */

  int i_zones;
  xavs_zone_t *zones;
  xavs_zone_t *prev_zone;
};


static int init_pass2 (xavs_t *);
//static float rate_estimate_qscale( xavs_t *h );
static float rate_estimate_qscale (xavs_t * h, int slice_type);
static void update_vbv (xavs_t * h, int bits);
static void update_vbv_plan (xavs_t * h);
static double predict_size (predictor_t * p, double q, double var);
static void update_predictor (predictor_t * p, double q, double var, double bits);

/* Terminology:
* qp = h.avs's quantizer
* qscale = linearized quantizer = Lagrange multiplier
*/
static GOTV_INLINE double
qp2qscale (double qp)
{
  return 0.85 * pow (2.0, (qp - 12.0) / 8.0);
}
static GOTV_INLINE double
qscale2qp (double qscale)
{
  return 12.0 + 8.0 * log (qscale / 0.85) / log (2.0);
}

/* Texture bitrate is not quite inversely proportional to qscale,
* probably due the the changing number of SKIP blocks.
* MV bits level off at about qp<=12, because the lambda used
* for motion estimation is constant there. */
static GOTV_INLINE double
qscale2bits (ratecontrol_entry_t * rce, double qscale)
{
  if (qscale < 0.1)
    qscale = 0.1;
  return (rce->tex_bits + .1) * pow (rce->qscale / qscale, 1.1) + rce->mv_bits * pow (XAVS_MAX (rce->qscale, 1) / XAVS_MAX (qscale, 1), 0.5) + rce->misc_bits;
}

static char *
xavs_strcat_filename (char *input, char *suffix)
{
  char *output = xavs_malloc (strlen (input) + strlen (suffix) + 1);
  if (!output)
    return NULL;
  strcpy (output, input);
  strcat (output, suffix);
  return output;
}

int
xavs_ratecontrol_new (xavs_t * h)
{
  xavs_ratecontrol_t *rc;
  int i, j;
  int num_pred;

  xavs_emms ();

  CHECKED_MALLOCZERO (h->rc, h->param.i_threads * sizeof (xavs_ratecontrol_t));
  rc = h->rc;

  rc->b_abr = h->param.rc.i_rc_method != XAVS_RC_CQP && !h->param.rc.b_stat_read;
  rc->b_2pass = h->param.rc.i_rc_method == XAVS_RC_ABR && h->param.rc.b_stat_read;

  /* FIXME: use integers */
  if (h->param.i_fps_num > 0 && h->param.i_fps_den > 0)
    rc->fps = (float) h->param.i_fps_num / h->param.i_fps_den;
  else
    rc->fps = 25.0;

  if (h->param.rc.b_mb_tree)
  {
    h->param.rc.f_pb_factor = 1;
    rc->qcompress = 1;
  }
  else
    rc->qcompress = h->param.rc.f_qcompress;

  rc->bitrate = h->param.rc.i_bitrate * 1000.;
  rc->rate_tolerance = h->param.rc.f_rate_tolerance;
  rc->nmb = h->mb.i_mb_count;
  rc->last_non_b_pict_type = -1;
  rc->cbr_decay = 1.0;

  if (h->param.rc.i_rc_method == XAVS_RC_CRF && h->param.rc.b_stat_read)
  {
    xavs_log (h, XAVS_LOG_ERROR, "constant rate-factor is incompatible with 2pass.\n");
    return -1;
  }
  if (h->param.rc.i_vbv_buffer_size)
  {
    if (h->param.rc.i_rc_method == XAVS_RC_CQP)
    {
      xavs_log (h, XAVS_LOG_WARNING, "VBV is incompatible with constant QP, ignored.\n");
      h->param.rc.i_vbv_max_bitrate = 0;
      h->param.rc.i_vbv_buffer_size = 0;
    }
    else if (h->param.rc.i_vbv_max_bitrate == 0)
    {
      xavs_log (h, XAVS_LOG_DEBUG, "VBV maxrate unspecified, assuming CBR\n");
      h->param.rc.i_vbv_max_bitrate = h->param.rc.i_bitrate;
    }
  }
  if (h->param.rc.i_vbv_max_bitrate < h->param.rc.i_bitrate && h->param.rc.i_vbv_max_bitrate > 0)
    xavs_log (h, XAVS_LOG_WARNING, "max bitrate less than average bitrate, ignored.\n");
  else if (h->param.rc.i_vbv_max_bitrate > 0 && h->param.rc.i_vbv_buffer_size > 0)
  {
    if (h->param.rc.i_vbv_buffer_size < 3 * h->param.rc.i_vbv_max_bitrate / rc->fps)
    {
      h->param.rc.i_vbv_buffer_size = 3 * h->param.rc.i_vbv_max_bitrate / rc->fps;
      xavs_log (h, XAVS_LOG_WARNING, "VBV buffer size too small, using %d kbit\n", h->param.rc.i_vbv_buffer_size);
    }

    if (h->param.rc.f_vbv_buffer_init > 1.)
      h->param.rc.f_vbv_buffer_init = xavs_clip3f (h->param.rc.f_vbv_buffer_init / h->param.rc.i_vbv_buffer_size, 0, 1);
    rc->buffer_rate = h->param.rc.i_vbv_max_bitrate * 1000. / rc->fps;
    rc->buffer_size = h->param.rc.i_vbv_buffer_size * 1000.;
    rc->buffer_fill_final = rc->buffer_size * h->param.rc.f_vbv_buffer_init;
    rc->cbr_decay = 1.0 - rc->buffer_rate / rc->buffer_size * 0.5 * XAVS_MAX (0, 1.5 - rc->buffer_rate * rc->fps / rc->bitrate);
    rc->b_vbv = 1;
    rc->b_vbv_min_rate = !rc->b_2pass && h->param.rc.i_rc_method == XAVS_RC_ABR && h->param.rc.i_vbv_max_bitrate <= h->param.rc.i_bitrate;
  }
  else if (h->param.rc.i_vbv_max_bitrate)
  {
    xavs_log (h, XAVS_LOG_WARNING, "VBV maxrate specified, but no bufsize.\n");
    h->param.rc.i_vbv_max_bitrate = 0;
    rc->b_vbv = 0;
  }
  if (rc->rate_tolerance < 0.01)
  {
    xavs_log (h, XAVS_LOG_WARNING, "bitrate tolerance too small, using .01\n");
    rc->rate_tolerance = 0.01;
  }

  h->mb.b_variable_qp = rc->b_vbv || h->param.rc.i_aq_mode;

  if (rc->b_abr)
  {
    /* FIXME ABR_INIT_QP is actually used only in CRF */
#define ABR_INIT_QP ( h->param.rc.i_rc_method == XAVS_RC_CRF ? h->param.rc.f_rf_constant : 24 )
    rc->accum_p_norm = .01;
    rc->accum_p_qp = ABR_INIT_QP * rc->accum_p_norm;
    /* estimated ratio that produces a reasonable QP for the first I-frame */
    rc->cplxr_sum = .01 * pow (7.0e5, rc->qcompress) * pow (h->mb.i_mb_count, 0.5);     /* cplxr_sum:sum of bits*qscale/rceq */
    rc->wanted_bits_window = 1.0 * rc->bitrate / rc->fps;
    rc->last_non_b_pict_type = SLICE_TYPE_I;
  }

  if (h->param.rc.i_rc_method == XAVS_RC_CRF)
  {
    /* Arbitrary rescaling to make CRF somewhat similar to QP.
     * Try to compensate for MB-tree's effects as well. */
    double base_cplx = h->mb.i_mb_count * (h->param.i_bframe ? 120 : 80);
    double mbtree_offset = h->param.rc.b_mb_tree ? (1.0 - h->param.rc.f_qcompress) * 13.5 : 0;
    rc->rate_factor_constant = pow (base_cplx, 1 - rc->qcompress) / qp2qscale (h->param.rc.f_rf_constant + mbtree_offset);
  }

  rc->ip_offset = 6.0 * log (h->param.rc.f_ip_factor) / log (2.0);
  rc->pb_offset = 6.0 * log (h->param.rc.f_pb_factor) / log (2.0);
  rc->qp_constant[SLICE_TYPE_P] = h->param.rc.i_qp_constant;
  rc->qp_constant[SLICE_TYPE_I] = xavs_clip3 (h->param.rc.i_qp_constant - rc->ip_offset + 0.5, 0, 63);
  rc->qp_constant[SLICE_TYPE_B] = xavs_clip3 (h->param.rc.i_qp_constant + rc->pb_offset + 0.5, 0, 63);

  rc->lstep = pow (2, h->param.rc.i_qp_step / 6.0);     /* lstep : max change (multiply) in qscale per frame */
  rc->last_qscale = qp2qscale (26);     /*qp2scale():0.85 * pow(2.0, ( qp - 12.0 ) / 6.0); */
  /*int*/
  num_pred = h->param.b_sliced_threads * h->param.i_threads + 1;
  CHECKED_MALLOC (rc->pred, 5 * sizeof (predictor_t) * num_pred);
  CHECKED_MALLOC (rc->pred_b_from_p, sizeof (predictor_t));

  for (i = 0; i < 3; i++)
  {
    rc->last_qscale_for[i] = qp2qscale (ABR_INIT_QP);
    rc->lmin[i] = qp2qscale (h->param.rc.i_qp_min);
    rc->lmax[i] = qp2qscale (h->param.rc.i_qp_max);
    for (j = 0; j < num_pred; j++) 
    {
      rc->pred[i + j*5].coeff = 2.0;
      rc->pred[i + j*5].count = 1.0;
      rc->pred[i + j*5].decay = 0.5;
      rc->pred[i + j*5].offset = 0.0;   /*  predictor_t *pred: predict frame size from satd */
    }
    for (j = 0; j < 2; j++) 
    {
      rc->row_preds[i + j].coeff = .25;
      rc->row_preds[i + j].count = 1.0;
      rc->row_preds[i + j].decay = 0.5;
      rc->row_preds[i + j].offset = 0.0;
    }
  }
  *rc->pred_b_from_p = rc->pred[0];     /* predictor_t *pred_b_from_p; predict B-frame size from P-frame satd */

  /* Load stat file and init 2pass algo */
  if (h->param.rc.b_stat_read)
  {
    char *p, *stats_in, *stats_buf;

    /* read 1st pass stats */
    assert (h->param.rc.psz_stat_in);
    stats_buf = stats_in = xavs_slurp_file (h->param.rc.psz_stat_in);
    if (!stats_buf)
    {
      xavs_log (h, XAVS_LOG_ERROR, "ratecontrol_init: can't open stats file\n");
      return -1;
    }
    if (h->param.rc.b_mb_tree)
    {
      char *mbtree_stats_in = xavs_strcat_filename (h->param.rc.psz_stat_in, ".mbtree");
      if (!mbtree_stats_in)
        return -1;
      rc->p_mbtree_stat_file_in = fopen (mbtree_stats_in, "rb");
      xavs_free (mbtree_stats_in);
      if (!rc->p_mbtree_stat_file_in)
      {
        xavs_log (h, XAVS_LOG_ERROR, "ratecontrol_init: can't open mbtree stats file\n");
        return -1;
      }
    }

    /* check whether 1st pass options were compatible with current options */
    if (!strncmp (stats_buf, "#options:", 9))
    {
      int i;
      char *opts = stats_buf;
      stats_in = strchr (stats_buf, '\n');
      if (!stats_in)
        return -1;
      *stats_in = '\0';
      stats_in++;

      if ((p = strstr (opts, "bframes=")) && sscanf (p, "bframes=%d", &i) && h->param.i_bframe != i)
      {
        xavs_log (h, XAVS_LOG_ERROR, "different number of B-frames than 1st pass (%d vs %d)\n", h->param.i_bframe, i);
        return -1;
      }

      if ((p = strstr (opts, "keyint=")) && sscanf (p, "keyint=%d", &i) && h->param.i_keyint_max != i)
        xavs_log (h, XAVS_LOG_WARNING, "different keyint than 1st pass (%d vs %d)\n", h->param.i_keyint_max, i);

      if (strstr (opts, "qp=0") && h->param.rc.i_rc_method == XAVS_RC_ABR)
        xavs_log (h, XAVS_LOG_WARNING, "1st pass was lossless, bitrate prediction will be inaccurate\n");

      if (!strstr (opts, "direct=3") && h->param.analyse.i_direct_mv_pred == XAVS_DIRECT_PRED_AUTO)
      {
        xavs_log (h, XAVS_LOG_WARNING, "direct=auto not used on the first pass\n");
        h->mb.b_direct_auto_write = 1;
      }

      if ((p = strstr (opts, "b_adapt=")) && sscanf (p, "b_adapt=%d", &i) && i >= XAVS_B_ADAPT_NONE && i <= XAVS_B_ADAPT_TRELLIS)
        h->param.i_bframe_adaptive = i;
      else if (h->param.i_bframe)
      {
        xavs_log (h, XAVS_LOG_ERROR, "b_adapt method specified in stats file not valid\n");
        return -1;
      }

      if (h->param.rc.b_mb_tree && (p = strstr (opts, "rc_lookahead=")) && sscanf (p, "rc_lookahead=%d", &i))
        h->param.rc.i_lookahead = i;
    }

    /* find number of pics */
    p = stats_in;
    for (i = -1; p; i++)
      p = strchr (p + 1, ';');
    if (i == 0)
    {
      xavs_log (h, XAVS_LOG_ERROR, "empty stats file\n");
      return -1;
    }
    rc->num_entries = i;

    if (h->param.i_frame_total < rc->num_entries && h->param.i_frame_total > 0)
    {
      xavs_log (h, XAVS_LOG_WARNING, "2nd pass has fewer frames than 1st pass (%d vs %d)\n", h->param.i_frame_total, rc->num_entries);
    }
    if (h->param.i_frame_total > rc->num_entries)
    {
      xavs_log (h, XAVS_LOG_ERROR, "2nd pass has more frames than 1st pass (%d vs %d)\n", h->param.i_frame_total, rc->num_entries);
      return -1;
    }

    CHECKED_MALLOCZERO (rc->entry, rc->num_entries * sizeof (ratecontrol_entry_t));

    /* init all to skipped p frames */
    for (i = 0; i < rc->num_entries; i++)
    {
      ratecontrol_entry_t *rce = &rc->entry[i];
      rce->pict_type = SLICE_TYPE_P;
      rce->qscale = rce->new_qscale = qp2qscale (20);
      rce->misc_bits = rc->nmb + 10;
      rce->new_qp = 0;
    }

    /* read stats */
    p = stats_in;
    for (i = 0; i < rc->num_entries; i++)
    {
      ratecontrol_entry_t *rce;
      int frame_number;
      char pict_type;
      int e;
      char *next;
      float qp;

      next = strchr (p, ';');
      if (next)
      {
        (*next) = 0;            //sscanf is unbelievably slow on long strings
        next++;
      }
      e = sscanf (p, " in:%d ", &frame_number);

      if (frame_number < 0 || frame_number >= rc->num_entries)
      {
        xavs_log (h, XAVS_LOG_ERROR, "bad frame number (%d) at stats line %d\n", frame_number, i);
        return -1;
      }
      rce = &rc->entry[frame_number];
      rce->direct_mode = 0;

      e += sscanf (p, " in:%*d out:%*d type:%c q:%f tex:%d mv:%d misc:%d imb:%d pmb:%d smb:%d d:%c", &pict_type, &qp, &rce->tex_bits, &rce->mv_bits, &rce->misc_bits, &rce->i_count, &rce->p_count, &rce->s_count, &rce->direct_mode);

      switch (pict_type)
      {
      case 'I':
        rce->kept_as_ref = 1;
      case 'i':
        rce->pict_type = SLICE_TYPE_I;
        break;
      case 'P':
        rce->pict_type = SLICE_TYPE_P;
        break;
      case 'B':
        rce->kept_as_ref = 1;
      case 'b':
        rce->pict_type = SLICE_TYPE_B;
        break;
      default:
        e = -1;
        break;
      }
      if (e < 10)
      {
        xavs_log (h, XAVS_LOG_ERROR, "statistics are damaged at line %d, parser out=%d\n", i, e);
        return -1;
      }
      rce->qscale = qp2qscale (qp);
      p = next;
    }

    xavs_free (stats_buf);

    if (h->param.rc.i_rc_method == XAVS_RC_ABR)
    {
      if (init_pass2 (h) < 0)
        return -1;
    }                           /* else we're using constant quant, so no need to run the bitrate allocation */
  }

  /* Open output file */
  /* If input and output files are the same, output to a temp file
   * and move it to the real name only when it's complete */
  if (h->param.rc.b_stat_write)
  {
    char *p;
    rc->psz_stat_file_tmpname = xavs_strcat_filename (h->param.rc.psz_stat_out, ".temp");
    if (!rc->psz_stat_file_tmpname)
      return -1;

    rc->p_stat_file_out = fopen (rc->psz_stat_file_tmpname, "wb");
    if (rc->p_stat_file_out == NULL)
    {
      xavs_log (h, XAVS_LOG_ERROR, "ratecontrol_init: can't open stats file\n");
      return -1;
    }

    p = xavs_param2string (&h->param, 1);
    if (p)
      fprintf (rc->p_stat_file_out, "#options: %s\n", p);
    xavs_free (p);
    if (h->param.rc.b_mb_tree && !h->param.rc.b_stat_read)
    {
      rc->psz_mbtree_stat_file_tmpname = xavs_strcat_filename (h->param.rc.psz_stat_out, ".mbtree.temp");
      rc->psz_mbtree_stat_file_name = xavs_strcat_filename (h->param.rc.psz_stat_out, ".mbtree");
      if (!rc->psz_mbtree_stat_file_tmpname || !rc->psz_mbtree_stat_file_name)
        return -1;

      rc->p_mbtree_stat_file_out = fopen (rc->psz_mbtree_stat_file_tmpname, "wb");
      if (rc->p_mbtree_stat_file_out == NULL)
      {
        xavs_log (h, XAVS_LOG_ERROR, "ratecontrol_init: can't open mbtree stats file\n");
        return -1;
      }
    }
  }

  if (h->param.rc.b_mb_tree && (h->param.rc.b_stat_read || h->param.rc.b_stat_write))
    CHECKED_MALLOC (rc->qp_buffer, h->mb.i_mb_count * sizeof (uint16_t));

  for (i = 0; i < h->param.i_threads; i++)
  {
    h->thread[i]->rc = rc + i;
    if (i)
    {
      rc[i] = rc[0];
      memcpy (&h->thread[i]->param, &h->param, sizeof (xavs_param_t));
      h->thread[i]->mb.b_variable_qp = h->mb.b_variable_qp;
    }
  }

  return 0;
fail:
  return -1;
}



static xavs_zone_t *
get_zone (xavs_t * h, int frame_num)
{
  int i;
  for (i = h->rc->i_zones - 1; i >= 0; i--)
  {
    xavs_zone_t *z = &h->rc->zones[i];
    if (frame_num >= z->i_start && frame_num <= z->i_end)
      return z;
  }
  return NULL;
}

void
xavs_ratecontrol_summary (xavs_t * h)
{
  xavs_ratecontrol_t *rc = h->rc;
  if (rc->b_abr && h->param.rc.i_rc_method == XAVS_RC_ABR && rc->cbr_decay > .9999)
  {
    double base_cplx = h->mb.i_mb_count * (h->param.i_bframe ? 120 : 80);
    double mbtree_offset = h->param.rc.b_mb_tree ? (1.0 - h->param.rc.f_qcompress) * 12.5 : 0;
    xavs_log (h, XAVS_LOG_INFO, "final ratefactor: %.2f\n", qscale2qp (pow (base_cplx, 1 - rc->qcompress) * rc->cplxr_sum / rc->wanted_bits_window) - mbtree_offset);
  }
}

void
xavs_ratecontrol_delete (xavs_t * h)
{
  xavs_ratecontrol_t *rc = h->rc;
  int i;

  if (rc->p_stat_file_out)
  {
    fclose (rc->p_stat_file_out);
    if (h->i_frame >= rc->num_entries)
      if (rename (rc->psz_stat_file_tmpname, h->param.rc.psz_stat_out) != 0)
      {
        xavs_log (h, XAVS_LOG_ERROR, "failed to rename \"%s\" to \"%s\"\n", rc->psz_stat_file_tmpname, h->param.rc.psz_stat_out);
      }
    xavs_free (rc->psz_stat_file_tmpname);
  }
  if (rc->p_mbtree_stat_file_out)
  {
    fclose (rc->p_mbtree_stat_file_out);
    if (h->i_frame >= rc->num_entries)
      if (rename (rc->psz_mbtree_stat_file_tmpname, rc->psz_mbtree_stat_file_name) != 0)
      {
        xavs_log (h, XAVS_LOG_ERROR, "failed to rename \"%s\" to \"%s\"\n", rc->psz_mbtree_stat_file_tmpname, rc->psz_mbtree_stat_file_name);
      }
    xavs_free (rc->psz_mbtree_stat_file_tmpname);
    xavs_free (rc->psz_mbtree_stat_file_name);
  }
  xavs_free (rc->pred);
  xavs_free (rc->pred_b_from_p);
  xavs_free (rc->entry);
  xavs_free (rc->qp_buffer);
  if (rc->zones)
  {
    xavs_free (rc->zones[0].param);
    if (h->param.rc.psz_zones)
      for (i = 1; i < rc->i_zones; i++)
        if (rc->zones[i].param != rc->zones[0].param)
          xavs_free (rc->zones[i].param);
    xavs_free (rc->zones);
  }
  xavs_free (rc);
}

void
xavs_ratecontrol_set_estimated_size (xavs_t * h, int bits)
{
  xavs_pthread_mutex_lock (&h->fenc->mutex);
  h->rc->frame_size_estimated = bits;
  xavs_pthread_mutex_unlock (&h->fenc->mutex);
}

int
xavs_ratecontrol_get_estimated_size (xavs_t const *h)
{
  int size;
  xavs_pthread_mutex_lock (&h->fenc->mutex);
  size = h->rc->frame_size_estimated;
  xavs_pthread_mutex_unlock (&h->fenc->mutex);
  return size;
}

static void
accum_p_qp_update (xavs_t * h, float qp)
{
  xavs_ratecontrol_t *rc = h->rc;
  rc->accum_p_qp *= .95;
  rc->accum_p_norm *= .95;
  rc->accum_p_norm += 1;
  if (h->sh.i_type == SLICE_TYPE_I)
    rc->accum_p_qp += qp + rc->ip_offset;
  else
    rc->accum_p_qp += qp;
}

/* Before encoding a frame, choose a QP for it */
void
xavs_ratecontrol_start (xavs_t * h, int i_slice_type, int i_force_qp)
{
  xavs_ratecontrol_t *rc = h->rc;
  ratecontrol_entry_t *rce = NULL;
  xavs_zone_t *zone = get_zone (h, h->fenc->i_frame);
  float q;

  xavs_emms ();

  if (zone && (!rc->prev_zone || zone->param != rc->prev_zone->param))
    xavs_encoder_reconfig (h, zone->param);
  rc->prev_zone = zone;

  rc->qp_force = i_force_qp;

  if (h->param.rc.b_stat_read)
  {
    int frame = h->fenc->i_frame;
    assert (frame >= 0 && frame < rc->num_entries);
    rce = h->rc->rce = &h->rc->entry[frame];

    if (i_slice_type == SLICE_TYPE_B && h->param.analyse.i_direct_mv_pred == XAVS_DIRECT_PRED_AUTO)
    {
      h->sh.b_direct_spatial_mv_pred = (rce->direct_mode == 's');
      h->mb.b_direct_auto_read = (rce->direct_mode == 's' || rce->direct_mode == 't');
    }
  }

  if (rc->b_vbv)
  {
    memset (h->fdec->i_row_bits, 0, h->sps->i_mb_height * sizeof (int));
    rc->row_pred = &rc->row_preds[i_slice_type];
    update_vbv_plan (h);
  }

  if (i_slice_type != SLICE_TYPE_B)
  {
    rc->bframes = 0;
    while (h->frames.current[rc->bframes] && IS_XAVS_TYPE_B (h->frames.current[rc->bframes]->i_type))
      rc->bframes++;
  }

  if (i_force_qp)
  {
    q = i_force_qp - 1;
  }
  else if (rc->b_abr)
  {
    q = qscale2qp (rate_estimate_qscale (h, i_slice_type));
  }
  else if (rc->b_2pass)
  {
    rce->new_qscale = rate_estimate_qscale (h, i_slice_type);

    q = qscale2qp (rce->new_qscale);
  }
  else                          /* CQP */
  {
    q = rc->qp_constant[i_slice_type];
    if (zone)
    {
      if (zone->b_force_qp)
        q += zone->i_qp - rc->qp_constant[SLICE_TYPE_P];
      else
        q -= 6 * log (zone->f_bitrate_factor) / log (2);
    }
  }

  rc->qpa_rc = rc->qpa_aq = 0;
  h->fdec->f_qp_avg_rc = h->fdec->f_qp_avg_aq = rc->qpm = rc->qp = xavs_clip3 ((int) (q + 0.5), 0, 63);
  rc->f_qpm = q;
  if (rce)
    rce->new_qp = rc->qp;

  accum_p_qp_update (h, rc->qp);

  if (i_slice_type != SLICE_TYPE_B)
    rc->last_non_b_pict_type = i_slice_type;
}

static double
predict_row_size (xavs_t * h, int y, int qp)
{
  /* average between two predictors:
   * absolute SATD, and scaled bit cost of the colocated row in the previous frame */
  xavs_ratecontrol_t *rc = h->rc;
  double pred_s = predict_size (rc->row_pred, qp2qscale (qp), h->fdec->i_row_satd[y]);
  double pred_t = 0;
  if (h->sh.i_type != SLICE_TYPE_I && h->fref0[0]->i_type == h->fdec->i_type && h->fref0[0]->i_row_satd[y] > 0 && (abs (h->fref0[0]->i_row_satd[y] - h->fdec->i_row_satd[y]) < h->fdec->i_row_satd[y] / 2))
  {
    pred_t = h->fref0[0]->i_row_bits[y] * h->fdec->i_row_satd[y] / h->fref0[0]->i_row_satd[y] * qp2qscale (h->fref0[0]->i_row_qp[y]) / qp2qscale (qp);
  }
  if (pred_t == 0)
    pred_t = pred_s;

  return (pred_s + pred_t) / 2;
}

static double
row_bits_so_far (xavs_t * h, int y)
{
  int i;
  double bits = 0;
  for (i = 0; i <= y; i++)
    bits += h->fdec->i_row_bits[i];
  return bits;
}

static double
predict_row_size_sum (xavs_t * h, int y, int qp)
{
  int i;
  double bits = row_bits_so_far (h, y);
  for (i = y + 1; i < h->sps->i_mb_height; i++)
    bits += predict_row_size (h, i, qp);
  return bits;
}


void
xavs_ratecontrol_mb (xavs_t * h, int bits)
{
  xavs_ratecontrol_t *rc = h->rc;
  const int y = h->mb.i_mb_y;

  xavs_emms ();

  h->fdec->i_row_bits[y] += bits;
  rc->qpa_rc += rc->f_qpm;
  rc->qpa_aq += h->mb.i_qp;

  /*f_qpm;           qp for current macroblock: precise float for AQ 
   *float qpa_rc;    average of macroblocks' qp before aq 
   *float qpa_aq;    average of macroblocks' qp after aq */

  if (h->mb.i_mb_x != h->sps->i_mb_width - 1 || !rc->b_vbv)
    return;

  h->fdec->i_row_qp[y] = rc->qpm;

  if (h->sh.i_type == SLICE_TYPE_B)
  {
    /* B-frames shouldn't use lower QP than their reference frames.
     * This code is a bit overzealous in limiting B-frame quantizers, but it helps avoid
     * underflows due to the fact that B-frames are not explicitly covered by VBV. */
    if (y < h->sps->i_mb_height - 1)
    {
      int i_estimated;
      int avg_qp = XAVS_MIN (h->fref0[0]->i_row_qp[y + 1], h->fref1[0]->i_row_qp[y + 1]) + rc->pb_offset * ((h->fenc->i_type == XAVS_TYPE_BREF) ? 0.5 : 1);
      rc->qpm = XAVS_MIN (XAVS_MAX (rc->qp, avg_qp), 63);       //avg_qp could go higher than 51 due to pb_offset

      i_estimated = row_bits_so_far (h, y);     //FIXME: compute full estimated size
      if (i_estimated > h->rc->frame_size_planned)
        xavs_ratecontrol_set_estimated_size (h, i_estimated);
    }
  }
  else
  {
    update_predictor (rc->row_pred, qp2qscale (rc->qpm), h->fdec->i_row_satd[y], h->fdec->i_row_bits[y]);

    /* tweak quality based on difference from predicted size */
    if (y < h->sps->i_mb_height - 1 && h->stat.i_slice_count[h->sh.i_type] > 0)
    {
      int prev_row_qp = h->fdec->i_row_qp[y];
      int b0 = predict_row_size_sum (h, y, rc->qpm);
      int b1 = b0;
      int i_qp_max = XAVS_MIN (prev_row_qp + h->param.rc.i_qp_step, h->param.rc.i_qp_max);
      int i_qp_min = XAVS_MAX (prev_row_qp - h->param.rc.i_qp_step, h->param.rc.i_qp_min);
      float buffer_left_planned = rc->buffer_fill - rc->frame_size_planned;
      float rc_tol = 1;
      float headroom = 0;

      /* Don't modify the row QPs until a sufficent amount of the 
       *bits of the frame have been processed, in case a flat */
      /* area at the top of the frame was measured inaccurately. */
      if (row_bits_so_far (h, y) < 0.05 * rc->frame_size_planned)
        return;

      headroom = buffer_left_planned / rc->buffer_size;
      if (h->sh.i_type != SLICE_TYPE_I)
        headroom /= 2;
      rc_tol += headroom;

      if (!rc->b_vbv_min_rate)
        i_qp_min = XAVS_MAX (i_qp_min, h->sh.i_qp);

      while (rc->qpm < i_qp_max && (b1 > rc->frame_size_planned * rc_tol || (rc->buffer_fill - b1 < buffer_left_planned * 0.5)))
      {
        rc->qpm++;
        b1 = predict_row_size_sum (h, y, rc->qpm);
      }

      /* avoid VBV underflow */
      while ((rc->qpm < h->param.rc.i_qp_max) && (rc->buffer_fill - b1 < rc->buffer_size * 0.005))
      {
        rc->qpm++;
        b1 = predict_row_size_sum (h, y, rc->qpm);
      }

      while (rc->qpm > i_qp_min && rc->qpm > h->fdec->i_row_qp[0] && ((b1 < rc->frame_size_planned * 0.8 && rc->qpm <= prev_row_qp) || b1 < (rc->buffer_fill - rc->buffer_size + rc->buffer_rate) * 1.1))
      {
        rc->qpm--;
        b1 = predict_row_size_sum (h, y, rc->qpm);
      }
      xavs_ratecontrol_set_estimated_size (h, b1);
    }
  }
  /* loses the fractional part of the frame-wise qp */
  rc->f_qpm = rc->qpm;
}

int
xavs_ratecontrol_qp (xavs_t * h)
{
  return h->rc->qpm;
}

/* In 2pass, force the same frame types as in the 1st pass */
int
xavs_ratecontrol_slice_type (xavs_t * h, int frame_num)
{
  xavs_ratecontrol_t *rc = h->rc;
  if (h->param.rc.b_stat_read)
  {
    if (frame_num >= rc->num_entries)
    {
      /* We could try to initialize everything required for ABR and
       * adaptive B-frames, but that would be complicated.
       * So just calculate the average QP used so far. */
      int i;

      h->param.rc.i_qp_constant = (h->stat.i_slice_count[SLICE_TYPE_P] == 0) ? 24 : 1 + h->stat.i_slice_qp[SLICE_TYPE_P] / h->stat.i_slice_count[SLICE_TYPE_P];
      rc->qp_constant[SLICE_TYPE_P] = xavs_clip3 (h->param.rc.i_qp_constant, 0, 63);
      rc->qp_constant[SLICE_TYPE_I] = xavs_clip3 ((int) (qscale2qp (qp2qscale (h->param.rc.i_qp_constant) / fabs (h->param.rc.f_ip_factor)) + 0.5), 0, 63);
      rc->qp_constant[SLICE_TYPE_B] = xavs_clip3 ((int) (qscale2qp (qp2qscale (h->param.rc.i_qp_constant) * fabs (h->param.rc.f_pb_factor)) + 0.5), 0, 63);

      xavs_log (h, XAVS_LOG_ERROR, "2nd pass has more frames than 1st pass (%d)\n", rc->num_entries);
      xavs_log (h, XAVS_LOG_ERROR, "continuing anyway, at constant QP=%d\n", h->param.rc.i_qp_constant);
      if (h->param.i_bframe_adaptive)
        xavs_log (h, XAVS_LOG_ERROR, "disabling adaptive B-frames\n");

      for (i = 0; i < h->param.i_threads; i++)
      {
        h->thread[i]->rc->b_abr = 0;
        h->thread[i]->rc->b_2pass = 0;
        h->thread[i]->param.rc.i_rc_method = XAVS_RC_CQP;
        h->thread[i]->param.rc.b_stat_read = 0;
        h->thread[i]->param.i_bframe_adaptive = 0;
        h->thread[i]->param.i_scenecut_threshold = 0;
        if (h->thread[i]->param.i_bframe > 1)
          h->thread[i]->param.i_bframe = 1;
      }
      return XAVS_TYPE_AUTO;
    }
    switch (rc->entry[frame_num].pict_type)
    {
    case SLICE_TYPE_I:
      return rc->entry[frame_num].kept_as_ref ? XAVS_TYPE_IDR : XAVS_TYPE_I;

    case SLICE_TYPE_B:
      return rc->entry[frame_num].kept_as_ref ? XAVS_TYPE_BREF : XAVS_TYPE_B;

    case SLICE_TYPE_P:
    default:
      return XAVS_TYPE_P;
    }
  }
  else
  {
    return XAVS_TYPE_AUTO;
  }
}

/* After encoding one frame, save stats and update ratecontrol state */
int
xavs_ratecontrol_end (xavs_t * h, int bits)
{
  xavs_ratecontrol_t *rc = h->rc;
  const int *mbs = h->stat.frame.i_mb_count;
  int i;

  xavs_emms ();

  h->stat.frame.i_mb_count_skip = mbs[P_SKIP] + mbs[B_SKIP];
  h->stat.frame.i_mb_count_i = mbs[I_8x8];
  h->stat.frame.i_mb_count_p = mbs[P_L0] + mbs[P_8x8];
  for (i = B_DIRECT; i < B_8x8; i++)
    h->stat.frame.i_mb_count_p += mbs[i];

  h->fdec->f_qp_avg_rc = rc->qpa_rc /= h->mb.i_mb_count;        /*average of macroblocks' qp before aq */
  h->fdec->f_qp_avg_aq = rc->qpa_aq /= h->mb.i_mb_count;        /*average of macroblocks' qp after aq */

  if (h->param.rc.b_stat_write)
  {
    char c_type = h->sh.i_type == SLICE_TYPE_I ? (h->fenc->i_poc == 0 ? 'I' : 'i') : h->sh.i_type == SLICE_TYPE_P ? 'P' : h->fenc->b_kept_as_ref ? 'B' : 'b';
    int dir_frame = h->stat.frame.i_direct_score[1] - h->stat.frame.i_direct_score[0];
    int dir_avg = h->stat.i_direct_score[1] - h->stat.i_direct_score[0];
    char c_direct = h->mb.b_direct_auto_write ? (dir_frame > 0 ? 's' : dir_frame < 0 ? 't' : dir_avg > 0 ? 's' : dir_avg < 0 ? 't' : '-') : '-';
    if (fprintf (rc->p_stat_file_out,
                 "in:%d out:%d type:%c q:%.2f tex:%d mv:%d misc:%d imb:%d pmb:%d smb:%d d:%c;\n",
                 h->fenc->i_frame, h->i_frame, c_type, rc->qpa_rc, h->stat.frame.i_tex_bits, h->stat.frame.i_mv_bits, h->stat.frame.i_misc_bits, h->stat.frame.i_mb_count_i, h->stat.frame.i_mb_count_p, h->stat.frame.i_mb_count_skip, c_direct) < 0)
      goto fail;

    /* Don't re-write the data in multi-pass mode. */
    if (h->param.rc.b_mb_tree && h->fenc->b_kept_as_ref && !h->param.rc.b_stat_read)
    {
      uint8_t i_type = h->sh.i_type;
      int i;
      /* Values are stored as big-endian FIX8.8 */
      for (i = 0; i < h->mb.i_mb_count; i++)
        rc->qp_buffer[i] = endian_fix16 (h->fenc->f_qp_offset[i] * 256.0);
      if (fwrite (&i_type, 1, 1, rc->p_mbtree_stat_file_out) < 1)
        goto fail;
      if (fwrite (rc->qp_buffer, sizeof (uint16_t), h->mb.i_mb_count, rc->p_mbtree_stat_file_out) < h->mb.i_mb_count)
        goto fail;
    }
  }

  if (rc->b_abr)
  {
    if (h->sh.i_type != SLICE_TYPE_B)
      rc->cplxr_sum += bits * qp2qscale (rc->qpa_rc) / rc->last_rceq;
    else
    {
      /* Depends on the fact that B-frame's QP is an offset from the following P-frame's.
       * Not perfectly accurate with B-refs, but good enough. */
      rc->cplxr_sum += bits * qp2qscale (rc->qpa_rc) / (rc->last_rceq * fabs (h->param.rc.f_pb_factor));
    }
    rc->cplxr_sum *= rc->cbr_decay;
    rc->wanted_bits_window += rc->bitrate / rc->fps;
    rc->wanted_bits_window *= rc->cbr_decay;
  }

  if (rc->b_2pass)
  {
    rc->expected_bits_sum += qscale2bits (rc->rce, qp2qscale (rc->rce->new_qp));
  }

  if (h->mb.b_variable_qp)
  {
    if (h->sh.i_type == SLICE_TYPE_B)
    {
      rc->bframe_bits += bits;
      if (h->fenc->b_last_minigop_bframe)
      {
        update_predictor (rc->pred_b_from_p, qp2qscale (rc->qpa_rc), h->fref1[h->i_ref1 - 1]->i_satd, rc->bframe_bits / rc->bframes);
        rc->bframe_bits = 0;
      }
    }
  }

  update_vbv (h, bits);
  return 0;
fail:
  xavs_log (h, XAVS_LOG_ERROR, "ratecontrol_end: stats file could not be written to\n");
  return -1;
}

/****************************************************************************
* 2 pass functions
***************************************************************************/

/**
* modify the bitrate curve from pass1 for one frame
*/
static double
get_qscale (xavs_t * h, ratecontrol_entry_t * rce, double rate_factor, int frame_num)
{
  xavs_ratecontrol_t *rcc = h->rc;
  double q;
  xavs_zone_t *zone = get_zone (h, frame_num);

  q = pow (rce->blurred_complexity, 1 - rcc->qcompress);

  // avoid NaN's in the rc_eq
  if (!isfinite (q) || rce->tex_bits + rce->mv_bits == 0)
    q = rcc->last_qscale;
  else
  {
    rcc->last_rceq = q;
    q /= rate_factor;
    rcc->last_qscale = q;
  }

  if (zone)
  {
    if (zone->b_force_qp)
      q = qp2qscale (zone->i_qp);
    else
      q /= zone->f_bitrate_factor;
  }

  return q;
}

static double
get_diff_limited_q (xavs_t * h, ratecontrol_entry_t * rce, double q)
{
  xavs_ratecontrol_t *rcc = h->rc;
  const int pict_type = rce->pict_type;

  // force I/B quants as a function of P quants
  const double last_p_q = rcc->last_qscale_for[SLICE_TYPE_P];
  const double last_non_b_q = rcc->last_qscale_for[rcc->last_non_b_pict_type];
  if (pict_type == SLICE_TYPE_I)
  {
    double iq = q;
    double pq = qp2qscale (rcc->accum_p_qp / rcc->accum_p_norm);
    double ip_factor = fabs (h->param.rc.f_ip_factor);
    /* don't apply ip_factor if the following frame is also I */
    if (rcc->accum_p_norm <= 0)
      q = iq;
    else if (h->param.rc.f_ip_factor < 0)
      q = iq / ip_factor;
    else if (rcc->accum_p_norm >= 1)
      q = pq / ip_factor;
    else
      q = rcc->accum_p_norm * pq / ip_factor + (1 - rcc->accum_p_norm) * iq;
  }
  else if (pict_type == SLICE_TYPE_B)
  {
    if (h->param.rc.f_pb_factor > 0)
      q = last_non_b_q;
    if (!rce->kept_as_ref)
      q *= fabs (h->param.rc.f_pb_factor);
  }
  else if (pict_type == SLICE_TYPE_P && rcc->last_non_b_pict_type == SLICE_TYPE_P && rce->tex_bits == 0)
  {
    q = last_p_q;
  }

  /* last qscale / qdiff stuff */
  if (rcc->last_non_b_pict_type == pict_type && (pict_type != SLICE_TYPE_I || rcc->last_accum_p_norm < 1))
  {
    double last_q = rcc->last_qscale_for[pict_type];
    double max_qscale = last_q * rcc->lstep;
    double min_qscale = last_q / rcc->lstep;

    if (q > max_qscale)
      q = max_qscale;
    else if (q < min_qscale)
      q = min_qscale;
  }

  rcc->last_qscale_for[pict_type] = q;
  if (pict_type != SLICE_TYPE_B)
    rcc->last_non_b_pict_type = pict_type;
  if (pict_type == SLICE_TYPE_I)
  {
    rcc->last_accum_p_norm = rcc->accum_p_norm;
    rcc->accum_p_norm = 0;
    rcc->accum_p_qp = 0;
  }
  if (pict_type == SLICE_TYPE_P)
  {
    float mask = 1 - pow ((float) rce->i_count / rcc->nmb, 2);
    rcc->accum_p_qp = mask * (qscale2qp (q) + rcc->accum_p_qp);
    rcc->accum_p_norm = mask * (1 + rcc->accum_p_norm);
  }
  return q;
}

static double
predict_size (predictor_t * p, double q, double var)
{
  return (p->coeff * var + p->offset) / (q * p->count);
}

static void
update_predictor (predictor_t * p, double q, double var, double bits)
{
  const double range = 1.5;
  if (var < 10)
    return;
  {
    double old_coeff = p->coeff / p->count;
    double new_coeff = bits * q / var;
    double new_coeff_clipped = xavs_clip3f (new_coeff, old_coeff / range, old_coeff * range);
    double new_offset = bits * q - new_coeff_clipped * var;
    if (new_offset >= 0)
      new_coeff = new_coeff_clipped;
    else
      new_offset = 0;
    p->count *= p->decay;
    p->coeff *= p->decay;
    p->offset *= p->decay;
    p->count++;
    p->coeff += new_coeff;
    p->offset += new_offset;
  }
}

// update VBV after encoding a frame
static void
update_vbv (xavs_t * h, int bits)
{
  xavs_ratecontrol_t *rcc = h->rc;
  xavs_ratecontrol_t *rct = h->thread[0]->rc;

  if (rcc->last_satd >= h->mb.i_mb_count)
    update_predictor (&rct->pred[h->sh.i_type], qp2qscale (rcc->qpa_rc), rcc->last_satd, bits);

  if (!rcc->b_vbv)
    return;

  rct->buffer_fill_final += rct->buffer_rate - bits;
  if (rct->buffer_fill_final < 0)
    xavs_log (h, XAVS_LOG_WARNING, "VBV underflow (%.0f bits)\n", rct->buffer_fill_final);
  rct->buffer_fill_final = xavs_clip3f (rct->buffer_fill_final, 0, rct->buffer_size);
}

// provisionally update VBV according to the planned size of all frames currently in progress
static void
update_vbv_plan (xavs_t * h)
{
  xavs_ratecontrol_t *rcc = h->rc;
  rcc->buffer_fill = h->thread[0]->rc->buffer_fill_final;
  if (h->param.i_threads > 1)
  {
    int j = h->rc - h->thread[0]->rc;
    int i;
    for (i = 1; i < h->param.i_threads; i++)
    {
      xavs_t *t = h->thread[(j + i) % h->param.i_threads];
      double bits = t->rc->frame_size_planned;
      if( t->fenc == NULL)
          continue;
      bits = XAVS_MAX (bits, xavs_ratecontrol_get_estimated_size (t));
      rcc->buffer_fill += rcc->buffer_rate - bits;
      rcc->buffer_fill = xavs_clip3 (rcc->buffer_fill, 0, rcc->buffer_size);
    }
  }
}

// apply VBV constraints and clip qscale to between lmin and lmax
static double
clip_qscale (xavs_t * h, int pict_type, double q)
{
  xavs_ratecontrol_t *rcc = h->rc;
  double lmin = rcc->lmin[pict_type];
  double lmax = rcc->lmax[pict_type];
  double q0 = q;

  /* B-frames are not directly subject to VBV,
   * since they are controlled by the P-frames' QPs.
   * FIXME: in 2pass we could modify previous frames' QP too,
   *        instead of waiting for the buffer to fill */
  if (rcc->b_vbv && (pict_type == SLICE_TYPE_P || (pict_type == SLICE_TYPE_I && rcc->last_non_b_pict_type == SLICE_TYPE_I)))
  {
    if (rcc->buffer_fill / rcc->buffer_size < 0.5)
      q /= xavs_clip3f (2.0 * rcc->buffer_fill / rcc->buffer_size, 0.5, 1.0);
  }

  if (rcc->b_vbv && rcc->last_satd > 0)
  {
    /* Now a hard threshold to make sure the frame fits in VBV.
     * This one is mostly for I-frames. */
    double bits = predict_size (&rcc->pred[h->sh.i_type], q, rcc->last_satd);
    double qf = 1.0;
    if (bits > rcc->buffer_fill / 2)
      qf = rcc->buffer_fill / (2 * bits);
    q /= qf;
    bits *= qf;
    if (bits < rcc->buffer_rate / 2)
      q *= bits * 2 / rcc->buffer_rate;
    q = XAVS_MAX (q0, q);

    /* Check B-frame complexity, and use up any bits that would
     * overflow before the next P-frame. */
    if (h->sh.i_type == SLICE_TYPE_P)
    {
      int nb = rcc->bframes;
      double pbbits = bits;
      double bbits = predict_size (rcc->pred_b_from_p, q * h->param.rc.f_pb_factor, rcc->last_satd);
      double space;

      if (bbits > rcc->buffer_rate)
        nb = 0;
      pbbits += nb * bbits;

      space = rcc->buffer_fill + (1 + nb) * rcc->buffer_rate - rcc->buffer_size;
      if (pbbits < space)
      {
        q *= XAVS_MAX (pbbits / space, bits / (0.5 * rcc->buffer_size));
      }
      q = XAVS_MAX (q0 - 5, q);
    }

    if (!rcc->b_vbv_min_rate)
      q = XAVS_MAX (q0, q);
  }

  if (lmin == lmax)
    return lmin;
  else if (rcc->b_2pass)
  {
    double min2 = log (lmin);
    double max2 = log (lmax);
    q = (log (q) - min2) / (max2 - min2) - 0.5;
    q = 1.0 / (1.0 + exp (-4 * q));
    q = q * (max2 - min2) + min2;
    return exp (q);
  }
  else
    return xavs_clip3f (q, lmin, lmax);
}

// update qscale for 1 frame based on actual bits used so far
static float
rate_estimate_qscale (xavs_t * h, int slice_type)
{
  float q;
  xavs_ratecontrol_t *rcc = h->rc;
  ratecontrol_entry_t rce;
  int pict_type = slice_type;
  double lmin = rcc->lmin[pict_type];
  double lmax = rcc->lmax[pict_type];
  int64_t total_bits = 8 * (h->stat.i_slice_size[SLICE_TYPE_I] + h->stat.i_slice_size[SLICE_TYPE_P] + h->stat.i_slice_size[SLICE_TYPE_B]);

  if (rcc->b_2pass)
  {
    rce = *rcc->rce;
    if (pict_type != rce.pict_type)
    {
      xavs_log (h, XAVS_LOG_ERROR, "slice=%c but 2pass stats say %c\n", slice_type_to_char[pict_type], slice_type_to_char[rce.pict_type]);
    }
  }

  if (pict_type == SLICE_TYPE_B)
  {
    /* B-frames don't have independent ratecontrol, but rather get the
     * average QP of the two adjacent P-frames + an offset */

    int i0 = IS_XAVS_TYPE_I (h->fref0[0]->i_type);
    int i1 = IS_XAVS_TYPE_I (h->fref1[0]->i_type);
    int dt0 = abs (h->fenc->i_poc - h->fref0[0]->i_poc);
    int dt1 = abs (h->fenc->i_poc - h->fref1[0]->i_poc);
    float q0 = h->fref0[0]->f_qp_avg_rc;
    float q1 = h->fref1[0]->f_qp_avg_rc;

    if (i0 && i1)
      q = (q0 + q1) / 2 + rcc->ip_offset;
    else if (i0)
      q = q1;
    else if (i1)
      q = q0;
    else
      q = (q0 * dt1 + q1 * dt0) / (dt0 + dt1);

    q += rcc->pb_offset;

    rcc->frame_size_planned = predict_size (rcc->pred_b_from_p, q, h->fref1[h->i_ref1 - 1]->i_satd);

    xavs_ratecontrol_set_estimated_size (h, rcc->frame_size_planned);
    rcc->last_satd = 0;
    return qp2qscale (q);
  }
  else                          /* for I or P frame */
  {
    double abr_buffer = 2 * rcc->rate_tolerance * rcc->bitrate;

    if (rcc->b_2pass)
    {
      //FIXME adjust abr_buffer based on distance to the end of the video
      int64_t diff;
      int64_t predicted_bits = total_bits;

      if (rcc->b_vbv)
      {
        if (h->param.i_threads > 1)
        {
          int j = h->rc - h->thread[0]->rc;
          int i;
          for (i = 1; i < h->param.i_threads; i++)
          {
            xavs_t *t = h->thread[(j + i) % h->param.i_threads];
            double bits = t->rc->frame_size_planned;
            if( !t->b_thread_active )
                  continue;
            bits = XAVS_MAX (bits, xavs_ratecontrol_get_estimated_size (t));
            predicted_bits += (int64_t) bits;
          }
        }
      }
      else
      {
        if (h->fenc->i_frame < h->param.i_threads)
          predicted_bits += (int64_t) h->fenc->i_frame * rcc->bitrate / rcc->fps;
        else
          predicted_bits += (int64_t) (h->param.i_threads - 1) * rcc->bitrate / rcc->fps;
      }

      diff = predicted_bits - (int64_t) rce.expected_bits;
      q = rce.new_qscale;
      q /= xavs_clip3f ((double) (abr_buffer - diff) / abr_buffer, .5, 2);
      if (((h->fenc->i_frame + 1 - h->param.i_threads) >= rcc->fps) && (rcc->expected_bits_sum > 0))
      {
        /* Adjust quant based on the difference between
         * achieved and expected bitrate so far */
        double time = (double) h->fenc->i_frame / rcc->num_entries;
        double w = xavs_clip3f (time * 100, 0.0, 1.0);
        q *= pow ((double) total_bits / rcc->expected_bits_sum, w);
      }
      if (rcc->b_vbv)
      {
        /* Do not overflow vbv */
        double expected_size = qscale2bits (&rce, q);
        double expected_vbv = rcc->buffer_fill + rcc->buffer_rate - expected_size;
        double expected_fullness = rce.expected_vbv / rcc->buffer_size;
        double qmax = q * (2 - expected_fullness);
        double size_constraint = 1 + expected_fullness;
        qmax = XAVS_MAX (qmax, rce.new_qscale);
        if (expected_fullness < .05)
          qmax = lmax;
        qmax = XAVS_MIN (qmax, lmax);
        while (((expected_vbv < rce.expected_vbv / size_constraint) && (q < qmax)) || ((expected_vbv < 0) && (q < lmax)))
        {
          q *= 1.05;
          expected_size = qscale2bits (&rce, q);
          expected_vbv = rcc->buffer_fill + rcc->buffer_rate - expected_size;
        }
        rcc->last_satd = xavs_stack_align (xavs_rc_analyse_slice, h);
      }
      q = xavs_clip3f (q, lmin, lmax);
    }
    else                        /* 1pass ABR */
    {
      /* Calculate the quantizer which would have produced the desired
       * average bitrate if it had been applied to all frames so far.
       * Then modulate that quant based on the current frame's complexity
       * relative to the average complexity so far (using the 2pass RCEQ).
       * Then bias the quant up or down if total size so far was far from
       * the target.
       * Result: Depending on the value of rate_tolerance, there is a
       * tradeoff between quality and bitrate precision. But at large
       * tolerances, the bit distribution approaches that of 2pass. */

      double wanted_bits, overflow = 1, lmin, lmax;

      rcc->last_satd = xavs_stack_align (xavs_rc_analyse_slice, h);
      rcc->short_term_cplxsum *= 0.5;
      rcc->short_term_cplxcount *= 0.5;
      rcc->short_term_cplxsum += rcc->last_satd;
      rcc->short_term_cplxcount++;

      rce.tex_bits = rcc->last_satd;
      rce.blurred_complexity = rcc->short_term_cplxsum / rcc->short_term_cplxcount;
      rce.mv_bits = 0;
      rce.p_count = rcc->nmb;
      rce.i_count = 0;
      rce.s_count = 0;
      rce.qscale = 1;
      rce.pict_type = pict_type;

      if (h->param.rc.i_rc_method == XAVS_RC_CRF)
      {
        q = get_qscale (h, &rce, rcc->rate_factor_constant, h->fenc->i_frame);
      }
      else
      {
        int i_frame_done = h->fenc->i_frame + 1 - h->param.i_threads;

        /*get_qscale:modify the bitrate curve from pass1 for one frame */
        q = get_qscale (h, &rce, rcc->wanted_bits_window / rcc->cplxr_sum, h->fenc->i_frame);

        // FIXME is it simpler to keep track of wanted_bits in ratecontrol_end?
        wanted_bits = i_frame_done * rcc->bitrate / rcc->fps;
        if (wanted_bits > 0)
        {
          abr_buffer *= XAVS_MAX (1, sqrt (i_frame_done / 25));
          overflow = xavs_clip3f (1.0 + (total_bits - wanted_bits) / abr_buffer, .5, 2);
          q *= overflow;
        }
      }

      if (pict_type == SLICE_TYPE_I && h->param.i_keyint_max > 1
          /* should test _next_ pict type, but that isn't decided yet */
          && rcc->last_non_b_pict_type != SLICE_TYPE_I)
      {
        q = qp2qscale (rcc->accum_p_qp / rcc->accum_p_norm);
        q /= fabs (h->param.rc.f_ip_factor);
      }
      else if (h->i_frame > 0)
      {
        /* Asymmetric clipping, because symmetric would prevent
         * overflow control in areas of rapidly oscillating complexity */
        lmin = rcc->last_qscale_for[pict_type] / rcc->lstep;
        lmax = rcc->last_qscale_for[pict_type] * rcc->lstep;
        if (overflow > 1.1 && h->i_frame > 3)
          lmax *= rcc->lstep;
        else if (overflow < 0.9)
          lmin /= rcc->lstep;

        q = xavs_clip3f (q, lmin, lmax);
      }
      else if (h->param.rc.i_rc_method == XAVS_RC_CRF)
      {
        q = qp2qscale (ABR_INIT_QP) / fabs (h->param.rc.f_ip_factor);
      }

      //FIXME use get_diff_limited_q() ?
      /* apply VBV constraints and clip qscale to between lmin and lmax */
      q = clip_qscale (h, pict_type, q);
    }

    rcc->last_qscale_for[pict_type] = rcc->last_qscale = q;

    if (!(rcc->b_2pass && !rcc->b_vbv) && h->fenc->i_frame == 0)
      rcc->last_qscale_for[SLICE_TYPE_P] = q;

    if (rcc->b_2pass && rcc->b_vbv)
      rcc->frame_size_planned = qscale2bits (&rce, q);
    else
      rcc->frame_size_planned = predict_size (&rcc->pred[h->sh.i_type], q, rcc->last_satd);
    xavs_ratecontrol_set_estimated_size (h, rcc->frame_size_planned);
    return q;
  }
}

void
xavs_thread_sync_ratecontrol (xavs_t * cur, xavs_t * prev, xavs_t * next)
{
  if (cur != prev)
  {
#define COPY(var) memcpy(&cur->rc->var, &prev->rc->var, sizeof(cur->rc->var))
    /* these vars are updated in xavs_ratecontrol_start()
     * so copy them from the context that most recently started (prev)
     * to the context that's about to start (cur).
     */
    COPY (accum_p_qp);
    COPY (accum_p_norm);
    COPY (last_satd);
    COPY (last_rceq);
    COPY (last_qscale_for);
    COPY (last_non_b_pict_type);
    COPY (short_term_cplxsum);
    COPY (short_term_cplxcount);
    COPY (bframes);
    COPY (prev_zone);
#undef COPY
  }
  if (cur != next)
  {
#define COPY(var) next->rc->var = cur->rc->var
    /* these vars are updated in xavs_ratecontrol_end()
     * so copy them from the context that most recently ended (cur)
     * to the context that's about to end (next)
     */
    COPY (cplxr_sum);
    COPY (expected_bits_sum);
    COPY (wanted_bits_window);
    COPY (bframe_bits);
#undef COPY
  }
  //FIXME row_preds[] (not strictly necessary, but would improve prediction)
  /* the rest of the variables are either constant or thread-local */
}

static int
find_underflow (xavs_t * h, double *fills, int *t0, int *t1, int over)
{
  /* find an interval ending on an overflow or underflow (depending on whether
   * we're adding or removing bits), and starting on the earliest frame that
   * can influence the buffer fill of that end frame. */
  xavs_ratecontrol_t *rcc = h->rc;
  const double buffer_min = (over ? .1 : .1) * rcc->buffer_size;
  const double buffer_max = .9 * rcc->buffer_size;
  double fill = fills[*t0 - 1];
  double parity = over ? 1. : -1.;
  int i, start = -1, end = -1;
  for (i = *t0; i < rcc->num_entries; i++)
  {
    fill += (rcc->buffer_rate - qscale2bits (&rcc->entry[i], rcc->entry[i].new_qscale)) * parity;
    fill = xavs_clip3f (fill, 0, rcc->buffer_size);
    fills[i] = fill;
    if (fill <= buffer_min || i == 0)
    {
      if (end >= 0)
        break;
      start = i;
    }
    else if (fill >= buffer_max && start >= 0)
      end = i;
  }
  *t0 = start;
  *t1 = end;
  return start >= 0 && end >= 0;
}

static int
fix_underflow (xavs_t * h, int t0, int t1, double adjustment, double qscale_min, double qscale_max)
{
  xavs_ratecontrol_t *rcc = h->rc;
  double qscale_orig, qscale_new;
  int i;
  int adjusted = 0;
  if (t0 > 0)
    t0++;
  for (i = t0; i <= t1; i++)
  {
    qscale_orig = rcc->entry[i].new_qscale;
    qscale_orig = xavs_clip3f (qscale_orig, qscale_min, qscale_max);
    qscale_new = qscale_orig * adjustment;
    qscale_new = xavs_clip3f (qscale_new, qscale_min, qscale_max);
    rcc->entry[i].new_qscale = qscale_new;
    adjusted = adjusted || (qscale_new != qscale_orig);
  }
  return adjusted;
}

static double
count_expected_bits (xavs_t * h)
{
  xavs_ratecontrol_t *rcc = h->rc;
  double expected_bits = 0;
  int i;
  for (i = 0; i < rcc->num_entries; i++)
  {
    ratecontrol_entry_t *rce = &rcc->entry[i];
    rce->expected_bits = expected_bits;
    expected_bits += qscale2bits (rce, rce->new_qscale);
  }
  return expected_bits;
}

static int
vbv_pass2 (xavs_t * h)
{
  /* for each interval of buffer_full .. underflow, uniformly increase the qp of all
   * frames in the interval until either buffer is full at some intermediate frame or the
   * last frame in the interval no longer underflows.  Recompute intervals and repeat.
   * Then do the converse to put bits back into overflow areas until target size is met */

  xavs_ratecontrol_t *rcc = h->rc;
  double *fills;
  double all_available_bits = h->param.rc.i_bitrate * 1000. * rcc->num_entries / rcc->fps;
  double expected_bits = 0;
  double adjustment;
  double prev_bits = 0;
  int i, t0, t1;
  double qscale_min = qp2qscale (h->param.rc.i_qp_min);
  double qscale_max = qp2qscale (h->param.rc.i_qp_max);
  int iterations = 0;
  int adj_min, adj_max;
  CHECKED_MALLOC (fills, (rcc->num_entries + 1) * sizeof (double));

  fills++;

  /* adjust overall stream size */
  do
  {
    iterations++;
    prev_bits = expected_bits;

    if (expected_bits != 0)
    {                           /* not first iteration */
      adjustment = XAVS_MAX (XAVS_MIN (expected_bits / all_available_bits, 0.999), 0.9);
      fills[-1] = rcc->buffer_size * h->param.rc.f_vbv_buffer_init;
      t0 = 0;
      /* fix overflows */
      adj_min = 1;
      while (adj_min && find_underflow (h, fills, &t0, &t1, 1))
      {
        adj_min = fix_underflow (h, t0, t1, adjustment, qscale_min, qscale_max);
        t0 = t1;
      }
    }

    fills[-1] = rcc->buffer_size * (1. - h->param.rc.f_vbv_buffer_init);
    t0 = 0;
    /* fix underflows -- should be done after overflow, as we'd better undersize target than underflowing VBV */
    adj_max = 1;
    while (adj_max && find_underflow (h, fills, &t0, &t1, 0))
      adj_max = fix_underflow (h, t0, t1, 1.001, qscale_min, qscale_max);

    expected_bits = count_expected_bits (h);
  }
  while ((expected_bits < .995 * all_available_bits) && ((int) (expected_bits + .5) > (int) (prev_bits + .5)));

  if (!adj_max)
    xavs_log (h, XAVS_LOG_WARNING, "vbv-maxrate issue, qpmax or vbv-maxrate too low\n");

  /* store expected vbv filling values for tracking when encoding */
  for (i = 0; i < rcc->num_entries; i++)
    rcc->entry[i].expected_vbv = rcc->buffer_size - fills[i];

  xavs_free (fills - 1);
  return 0;
fail:
  return -1;
}

static int
init_pass2 (xavs_t * h)
{
  xavs_ratecontrol_t *rcc = h->rc;
  uint64_t all_const_bits = 0;
  uint64_t all_available_bits = (uint64_t) (h->param.rc.i_bitrate * 1000. * rcc->num_entries / rcc->fps);
  double rate_factor, step, step_mult;
  double qblur = h->param.rc.f_qblur;
  double cplxblur = h->param.rc.f_complexity_blur;
  const int filter_size = (int) (qblur * 4) | 1;
  double expected_bits;
  double *qscale, *blurred_qscale;
  int i;

  /* find total/average complexity & const_bits */
  for (i = 0; i < rcc->num_entries; i++)
  {
    ratecontrol_entry_t *rce = &rcc->entry[i];
    all_const_bits += rce->misc_bits;
  }

  if (all_available_bits < all_const_bits)
  {
    xavs_log (h, XAVS_LOG_ERROR, "requested bitrate is too low. estimated minimum is %d kbps\n", (int) (all_const_bits * rcc->fps / (rcc->num_entries * 1000.)));
    return -1;
  }

  /* Blur complexities, to reduce local fluctuation of QP.
   * We don't blur the QPs directly, because then one very simple frame
   * could drag down the QP of a nearby complex frame and give it more
   * bits than intended. */
  for (i = 0; i < rcc->num_entries; i++)
  {
    ratecontrol_entry_t *rce = &rcc->entry[i];
    double weight_sum = 0;
    double cplx_sum = 0;
    double weight = 1.0;
    double gaussian_weight;
    int j;
    /* weighted average of cplx of future frames */
    for (j = 1; j < cplxblur * 2 && j < rcc->num_entries - i; j++)
    {
      ratecontrol_entry_t *rcj = &rcc->entry[i + j];
      weight *= 1 - pow ((float) rcj->i_count / rcc->nmb, 2);
      if (weight < .0001)
        break;
      gaussian_weight = weight * exp (-j * j / 200.0);
      weight_sum += gaussian_weight;
      cplx_sum += gaussian_weight * (qscale2bits (rcj, 1) - rcj->misc_bits);
    }
    /* weighted average of cplx of past frames */
    weight = 1.0;
    for (j = 0; j <= cplxblur * 2 && j <= i; j++)
    {
      ratecontrol_entry_t *rcj = &rcc->entry[i - j];
      gaussian_weight = weight * exp (-j * j / 200.0);
      weight_sum += gaussian_weight;
      cplx_sum += gaussian_weight * (qscale2bits (rcj, 1) - rcj->misc_bits);
      weight *= 1 - pow ((float) rcj->i_count / rcc->nmb, 2);
      if (weight < .0001)
        break;
    }
    rce->blurred_complexity = cplx_sum / weight_sum;
  }

  CHECKED_MALLOC (qscale, sizeof (double) * rcc->num_entries);
  if (filter_size > 1)
    CHECKED_MALLOC (blurred_qscale, sizeof (double) * rcc->num_entries);
  else
    blurred_qscale = qscale;

  /* Search for a factor which, when multiplied by the RCEQ values from
   * each frame, adds up to the desired total size.
   * There is no exact closed-form solution because of VBV constraints and
   * because qscale2bits is not invertible, but we can start with the simple
   * approximation of scaling the 1st pass by the ratio of bitrates.
   * The search range is probably overkill, but speed doesn't matter here. */

  expected_bits = 1;
  for (i = 0; i < rcc->num_entries; i++)
    expected_bits += qscale2bits (&rcc->entry[i], get_qscale (h, &rcc->entry[i], 1.0, i));
  step_mult = all_available_bits / expected_bits;

  rate_factor = 0;
  for (step = 1E4 * step_mult; step > 1E-7 * step_mult; step *= 0.5)
  {
    expected_bits = 0;
    rate_factor += step;

    rcc->last_non_b_pict_type = -1;
    rcc->last_accum_p_norm = 1;
    rcc->accum_p_norm = 0;

    /* find qscale */
    for (i = 0; i < rcc->num_entries; i++)
    {
      qscale[i] = get_qscale (h, &rcc->entry[i], rate_factor, i);
    }

    /* fixed I/B qscale relative to P */
    for (i = rcc->num_entries - 1; i >= 0; i--)
    {
      qscale[i] = get_diff_limited_q (h, &rcc->entry[i], qscale[i]);
      assert (qscale[i] >= 0);
    }

    /* smooth curve */
    if (filter_size > 1)
    {
      assert (filter_size % 2 == 1);
      for (i = 0; i < rcc->num_entries; i++)
      {
        ratecontrol_entry_t *rce = &rcc->entry[i];
        int j;
        double q = 0.0, sum = 0.0;

        for (j = 0; j < filter_size; j++)
        {
          int index = i + j - filter_size / 2;
          double d = index - i;
          double coeff = qblur == 0 ? 1.0 : exp (-d * d / (qblur * qblur));
          if (index < 0 || index >= rcc->num_entries)
            continue;
          if (rce->pict_type != rcc->entry[index].pict_type)
            continue;
          q += qscale[index] * coeff;
          sum += coeff;
        }
        blurred_qscale[i] = q / sum;
      }
    }

    /* find expected bits */
    for (i = 0; i < rcc->num_entries; i++)
    {
      ratecontrol_entry_t *rce = &rcc->entry[i];
      rce->new_qscale = clip_qscale (h, rce->pict_type, blurred_qscale[i]);
      assert (rce->new_qscale >= 0);
      expected_bits += qscale2bits (rce, rce->new_qscale);
    }

    if (expected_bits > all_available_bits)
      rate_factor -= step;
  }

  xavs_free (qscale);
  if (filter_size > 1)
    xavs_free (blurred_qscale);

  if (rcc->b_vbv)
    if (vbv_pass2 (h))
      return -1;
  expected_bits = count_expected_bits (h);

  if (fabs (expected_bits / all_available_bits - 1.0) > 0.01)
  {
    double avgq = 0;
    for (i = 0; i < rcc->num_entries; i++)
      avgq += rcc->entry[i].new_qscale;
    avgq = qscale2qp (avgq / rcc->num_entries);

    if ((expected_bits > all_available_bits) || (!rcc->b_vbv))
      xavs_log (h, XAVS_LOG_WARNING, "Error: 2pass curve failed to converge\n");
    xavs_log (h, XAVS_LOG_WARNING, "target: %.2f kbit/s, expected: %.2f kbit/s, avg QP: %.4f\n", (float) h->param.rc.i_bitrate, expected_bits * rcc->fps / (rcc->num_entries * 1000.), avgq);
    if (expected_bits < all_available_bits && avgq < h->param.rc.i_qp_min + 2)
    {
      if (h->param.rc.i_qp_min > 0)
        xavs_log (h, XAVS_LOG_WARNING, "try reducing target bitrate or reducing qp_min (currently %d)\n", h->param.rc.i_qp_min);
      else
        xavs_log (h, XAVS_LOG_WARNING, "try reducing target bitrate\n");
    }
    else if (expected_bits > all_available_bits && avgq > h->param.rc.i_qp_max - 2)
    {
      if (h->param.rc.i_qp_max < 51)
        xavs_log (h, XAVS_LOG_WARNING, "try increasing target bitrate or increasing qp_max (currently %d)\n", h->param.rc.i_qp_max);
      else
        xavs_log (h, XAVS_LOG_WARNING, "try increasing target bitrate\n");
    }
    else if (!(rcc->b_2pass && rcc->b_vbv))
      xavs_log (h, XAVS_LOG_WARNING, "internal error\n");
  }

  return 0;
fail:
  return -1;
}

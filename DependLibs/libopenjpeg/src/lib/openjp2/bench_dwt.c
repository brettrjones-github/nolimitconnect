/*
 * The copyright in this software is being made available under the 2-clauses
 * BSD License, included below. This software may be subject to other third
 * party and contributor rights, including patent rights, and no such rights
 * are granted under this license.
 *
 * Copyright (c) 2017, IntoPix SA <contact@intopix.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS `AS IS'
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef BUILD_BENCH_DWT_TOOL
#include "opj_includes.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/times.h>
#endif /* _WIN32 */

OPJ_INT32 getValue(OPJ_UINT32 i)
{
    return ((OPJ_INT32)i % 511) - 256;
}

void init_tilec(opj_tcd_tilecomp_t * l_tilec,
                OPJ_INT32 x0,
                OPJ_INT32 y0,
                OPJ_INT32 x1,
                OPJ_INT32 y1,
                OPJ_UINT32 numresolutions)
{
    opj_tcd_resolution_t* l_res;
    OPJ_UINT32 resno, l_level_no;
    size_t i, nValues;

    memset(l_tilec, 0, sizeof(*l_tilec));
    l_tilec->x0 = x0;
    l_tilec->y0 = y0;
    l_tilec->x1 = x1;
    l_tilec->y1 = y1;
    nValues = (size_t)(l_tilec->x1 - l_tilec->x0) *
              (size_t)(l_tilec->y1 - l_tilec->y0);
    l_tilec->data = opj_malloc(sizeof(OPJ_INT32) * nValues);
    for (i = 0; i < nValues; i++) {
        l_tilec->data[i] = getValue(i);
    }
    l_tilec->numresolutions = numresolutions;
    l_tilec->resolutions = opj_calloc(l_tilec->numresolutions,
                                      sizeof(opj_tcd_resolution_t));

    l_level_no = l_tilec->numresolutions;
    l_res = l_tilec->resolutions;

    /* Adapted from opj_tcd_init_tile() */
    for (resno = 0; resno < l_tilec->numresolutions; ++resno) {

        --l_level_no;

        /* border for each resolution level (global) */
        l_res->x0 = opj_int_ceildivpow2(l_tilec->x0, (OPJ_INT32)l_level_no);
        l_res->y0 = opj_int_ceildivpow2(l_tilec->y0, (OPJ_INT32)l_level_no);
        l_res->x1 = opj_int_ceildivpow2(l_tilec->x1, (OPJ_INT32)l_level_no);
        l_res->y1 = opj_int_ceildivpow2(l_tilec->y1, (OPJ_INT32)l_level_no);

        ++l_res;
    }
}

void free_tilec(opj_tcd_tilecomp_t * l_tilec)
{
    opj_free(l_tilec->data);
    opj_free(l_tilec->resolutions);
}

void usage(void)
{
    printf(
        "bench_dwt [-size value] [-check] [-display] [-num_resolutions val]\n");
    printf(
        "          [-offset x y] [-num_threads val]\n");
    exit(1);
}


OPJ_FLOAT64 opj_clock(void)
{
#ifdef _WIN32
    /* _WIN32: use QueryPerformance (very accurate) */
    LARGE_INTEGER freq, t ;
    /* freq is the clock speed of the CPU */
    QueryPerformanceFrequency(&freq) ;
    /* cout << "freq = " << ((double) freq.QuadPart) << endl; */
    /* t is the high resolution performance counter (see MSDN) */
    QueryPerformanceCounter(& t) ;
    return freq.QuadPart ? (t.QuadPart / (OPJ_FLOAT64) freq.QuadPart) : 0 ;
#else
    /* Unix or Linux: use resource usage */
    struct rusage t;
    OPJ_FLOAT64 procTime;
    /* (1) Get the rusage data structure at this moment (man getrusage) */
    getrusage(0, &t);
    /* (2) What is the elapsed time ? - CPU time = User time + System time */
    /* (2a) Get the seconds */
    procTime = (OPJ_FLOAT64)(t.ru_utime.tv_sec + t.ru_stime.tv_sec);
    /* (2b) More precisely! Get the microseconds part ! */
    return (procTime + (OPJ_FLOAT64)(t.ru_utime.tv_usec + t.ru_stime.tv_usec) *
            1e-6) ;
#endif
}

int main(int argc, char** argv)
{
    int num_threads = 0;
    opj_tcd_tilecomp_t tilec;
    opj_thread_pool_t* tp;
    OPJ_INT32 i, j, k;
    OPJ_BOOL display = OPJ_FALSE;
    OPJ_BOOL check = OPJ_FALSE;
    OPJ_INT32 size = 16384 - 1;
    OPJ_FLOAT64 start, stop;
    OPJ_UINT32 offset_x = (size + 1) / 2 - 1;
    OPJ_UINT32 offset_y = (size + 1) / 2 - 1;
    OPJ_UINT32 num_resolutions = 6;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-display") == 0) {
            display = OPJ_TRUE;
            check = OPJ_TRUE;
        } else if (strcmp(argv[i], "-check") == 0) {
            check = OPJ_TRUE;
        } else if (strcmp(argv[i], "-size") == 0 && i + 1 < argc) {
            size = atoi(argv[i + 1]);
            i ++;
        } else if (strcmp(argv[i], "-num_threads") == 0 && i + 1 < argc) {
            num_threads = atoi(argv[i + 1]);
            i ++;
        } else if (strcmp(argv[i], "-num_resolutions") == 0 && i + 1 < argc) {
            num_resolutions = atoi(argv[i + 1]);
            if (num_resolutions == 0 || num_resolutions > 32) {
                fprintf(stderr,
                        "Invalid value for num_resolutions. Should be >= 1 and <= 32\n");
                exit(1);
            }
            i ++;
        } else if (strcmp(argv[i], "-offset") == 0 && i + 2 < argc) {
            offset_x = atoi(argv[i + 1]);
            offset_y = atoi(argv[i + 2]);
            i += 2;
        } else {
            usage();
        }
    }

    tp = opj_thread_pool_create(num_threads);

    init_tilec(&tilec, offset_x, offset_y, offset_x + size, offset_y + size,
               num_resolutions);

    if (display) {
        printf("Before\n");
        k = 0;
        for (j = 0; j < tilec.y1 - tilec.y0; j++) {
            for (i = 0; i < tilec.x1 - tilec.x0; i++) {
                printf("%d ", tilec.data[k]);
                k ++;
            }
            printf("\n");
        }
    }

    start = opj_clock();
    opj_dwt_decode(tp, &tilec, tilec.numresolutions);
    stop = opj_clock();
    printf("time for dwt_decode: %.03f s\n", stop - start);

    if (display || check) {
        if (display) {
            printf("After IDWT\n");
            k = 0;
            for (j = 0; j < tilec.y1 - tilec.y0; j++) {
                for (i = 0; i < tilec.x1 - tilec.x0; i++) {
                    printf("%d ", tilec.data[k]);
                    k ++;
                }
                printf("\n");
            }
        }

        opj_dwt_encode(&tilec);
        if (display) {
            printf("After FDWT\n");
            k = 0;
            for (j = 0; j < tilec.y1 - tilec.y0; j++) {
                for (i = 0; i < tilec.x1 - tilec.x0; i++) {
                    printf("%d ", tilec.data[k]);
                    k ++;
                }
                printf("\n");
            }
        }

        if (check) {
            size_t idx;
            size_t nValues = (size_t)(tilec.x1 - tilec.x0) *
                             (size_t)(tilec.y1 - tilec.y0);
            for (idx = 0; i < nValues; i++) {
                if (tilec.data[idx] != getValue(idx)) {
                    printf("Difference found at idx = %u\n", (OPJ_UINT32)idx);
                    exit(1);
                }
            }
        }
    }

    free_tilec(&tilec);

    opj_thread_pool_destroy(tp);
    return 0;
}

#endif // BUILD_BENCH_DWT_TOOL

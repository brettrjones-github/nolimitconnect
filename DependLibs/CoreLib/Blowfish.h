/*
  Blowfish algorithm.  Written 1997 by by Paul Kocher
  (pck@netcom.com).  The Blowfish algorithm is public domain.
*/
// modified for CheezyCrypt
#ifndef ___BLOWFISH_H___
#define ___BLOWFISH_H___

#include <CoreLib/config_corelib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_BLOW_STRING   256
#define MAX_BLOW_PASSWD   56  // 448bits
#define NUM_SUBKEYS  18
#define NUM_S_BOXES  4
#define NUM_ENTRIES  256

typedef struct {
  uint32_t PA[16 + 2];
  uint32_t SB[NUM_S_BOXES][NUM_ENTRIES];
}BlowCtx;


//NOTE all lengths must be a multiple of 16 bytes
void BlowsetKey( BlowCtx *pgBlowCtx, unsigned char *pu8Passwd, int iLen );
void BlowEncrypt( BlowCtx * pgBlowCtx, unsigned char * pu8Data, int iLen );
void BlowDecrypt( BlowCtx * pgBlowCtx, unsigned char * pu8Data, int iLen );


#ifdef __cplusplus
}  /* extern "C" */
#endif


#endif //___BLOWFISH_H___


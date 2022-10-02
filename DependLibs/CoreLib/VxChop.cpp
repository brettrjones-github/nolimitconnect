//============================================================================
// Copyright (C) 2014 Brett R. Jones 
// Issued to MIT style license by Brett R. Jones in 2017
//
// You may use, copy, modify, merge, publish, distribute, sub-license, and/or sell this software 
// provided this Copyright is not modified or removed and is included all copies or substantial portions of the Software
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// bjones.engineer@gmail.com
// http://www.nolimitconnect.org
//============================================================================

#include "config_corelib.h"

#include "VxChop.h"
#include "VxChopMap.h"
#include "VxDebug.h"
#include "VxParse.h"

#include <string.h>

#define  USE_MAPPING 1  // if defined Chop will use mapping to remap the chars so that
						// the values used the most will be lower values thus better
						// compression is archived

//============================================================================
// ascii
//============================================================================

//============================================================================
int VxChopStr( std::string &csStr, unsigned char * pu8RetChoppedStr )
{
	return VxChopStr( (const char*)csStr.c_str(), pu8RetChoppedStr );
}

//============================================================================
void VxUnchopStr( unsigned char * pu8ChoppedStr, std::string &csRetStr )
{
	vx_assert( pu8ChoppedStr );
	if( 0 == * pu8ChoppedStr )
	{
		csRetStr = "";
		return;
	}

	char as8Buf[ VX_MAX_PATH ];
	VxUnchopStr( pu8ChoppedStr, as8Buf );
	csRetStr = as8Buf;
}

//============================================================================
int VxCalcChoppedLen( const char* pStrIn )
{
	unsigned char ucBitsPerOffset;
	unsigned char ucHighest = 0;
	unsigned char ucBaseLevel = 0xff;
	if( 0 == * pStrIn )
		return 0;
	int iDataLen = (int)strlen( pStrIn );
	char * pStr;

#ifdef USE_MAPPING
	pStr = new char[ iDataLen + 1 ]; // make a copy before mapping so don't mess up callers original
	strcpy( pStr, pStrIn );
	VxMapStr( pStr ); // remap the chars
#else
	pStr = pStrIn;
#endif
	unsigned char * pucTemp = ( unsigned char *)pStr;
	//=== determine the highest and lowest values in the data stream ===//
	while( * pucTemp )
	{
		ucHighest = * pucTemp > ucHighest ? * pucTemp : ucHighest;
		ucBaseLevel = * pucTemp < ucBaseLevel ? * pucTemp : ucBaseLevel;
		pucTemp++;
	}
#ifdef USE_MAPPING
	delete pStr;
#endif

	// base level must be lowest value - 1 so that zero is unique and equal to base level
	if( ucBaseLevel > 1 )
		ucBaseLevel -= 1;
	ucBaseLevel = ucBaseLevel & 0xf8; //round base level down to nearest multiple of 8

	unsigned char ucBiggestOffset = ucHighest - ucBaseLevel; //this is the range of offsets we need to store

	ucBitsPerOffset = 8;	// start at worse case
							// we would need 8 bits to represent each byte offset
							// if the lowest value in chunk is 0 and the highest is 0xff
	unsigned char ucHighestBitMask = 0x80;
	do
	{
		if( ucHighestBitMask & ucBiggestOffset )
		{
			break;
		}
		ucHighestBitMask  = ucHighestBitMask >> 1;
		ucBitsPerOffset--;
	}while( ucHighestBitMask > 1);//for simplicity of coding we leave at least 1 bit per offset

	int iTotalBits = iDataLen * ucBitsPerOffset;
	int iTotalBytes = iTotalBits / 8;
	if( iTotalBits
		% 8 )
	{
		iTotalBytes++;
	}
	iTotalBytes+=2; //one byte for header //one byte for length

	return iTotalBytes;
}
//============================================================================
int VxGetUnchopedStrLen( char * pu8ChoppedStr )
{
	return (int)strlen( pu8ChoppedStr);
}

// format of chopped string of data 1 byte header
// Byte 1, Bits 7-3 represent Base Level of data chunk
// Byte 1, Bits 0-2 represent Number of Bits need to represent each offset from base line

//============================================================================
// ChopStr returns Compressed length
int VxChopStr( const char* pStrIn, unsigned char * pu8RetChoppedStr )
{
	unsigned char ucBiggestOffset;

	unsigned char ucBitsPerOffset;		// number of bits per offset
	unsigned char ucBitsPerOffsetMinus1;// number of bits ( minus 1 ) required to represent the largest offset from the base level
										// so a value of 7 would represent the worst case of 8 bits needed
	unsigned char ucHighest = 0;
	unsigned char ucBaseLevel = 0xff;
	if( 0 == * pStrIn )
	{
		* pu8RetChoppedStr = 0;
		return 0;
	}
	int iDataLen = (int)strlen( pStrIn );
	vx_assert( iDataLen < 255 );
	char * pStr;
#ifdef USE_MAPPING
	pStr = new char[ iDataLen + 1 ] ; // make a copy before mapping so don't mess up callers original
	strcpy( pStr, pStrIn );
	VxMapStr( pStr ); // remap the chars
#else
	pStr = pStrIn;
#endif
	unsigned char *pucTemp = (unsigned char *)pStr;

	//=== determine the highest and lowest values in the data stream ===//
	while( * pucTemp )
	{
		ucHighest = * pucTemp > ucHighest ? * pucTemp : ucHighest;
		ucBaseLevel = * pucTemp < ucBaseLevel ? * pucTemp : ucBaseLevel;
		pucTemp++;
	}
	pucTemp = ( unsigned char *)pStr; // save so we can delete after compression is done

	//=== determine base level value to subtract from each byte in data chunk ===//
	// this is the base value of the data chunk.
	// the data will be stored as offsets to be added to this level when decompressed

	// base level must be lowest value - 1 so that zero is unique and equal to base level
	// for strings we want to make sure every sample has at least one set bit
	// so that we dont get zeros in our string so subtract one from base level
	if( ucBaseLevel > 1 )
		ucBaseLevel -= 1;
	ucBaseLevel = ucBaseLevel & 0xf8; //round base level down to nearest multiple of 8

	//=== calculate the number of bits required to represent all offsets in the stream
	ucBiggestOffset = ucHighest - ucBaseLevel; //this is the range of offsets we need to store

	ucBitsPerOffsetMinus1 = 7;	// start at worse case
								// we would need 8 bits to represent each byte offset
								//if the lowest value in chunk is 0 and the highest is 0xff
	unsigned char ucHighestBitMask = 0x80;
	do
	{
		if( ucHighestBitMask & ucBiggestOffset )
		{
			break;
		}
		ucHighestBitMask  = ucHighestBitMask >> 1;
		ucBitsPerOffsetMinus1--;
	}while( ucHighestBitMask > 1);//for simplicity of coding we leave at least 1 bit per offset
	unsigned char * pu8Len = ( unsigned char * )pu8RetChoppedStr;
	*pu8Len = 3;//length header plus at least on char in string

	pu8RetChoppedStr++;

	* pu8RetChoppedStr = ucBaseLevel | ucBitsPerOffsetMinus1; // fill in the chunk header
	pu8RetChoppedStr++;

	//=== now pack the offsets that represent the data ===//
	ucBitsPerOffset = ucBitsPerOffsetMinus1 + 1;
	int iBitsThisByte = 8;//number of bits available to pack in this byte
	unsigned char ucThisOffset;
	unsigned char ucTempOffset;
	pStr[ iDataLen ] = ucBaseLevel; // set base level so when reaches this then we are done
	do
	{
		ucThisOffset =  ( unsigned char )*pStr - ucBaseLevel;
		pStr++;
		switch( iBitsThisByte )
		{
		case 1:
			ucTempOffset = ucThisOffset >> ( ucBitsPerOffset - 1);
			*pu8RetChoppedStr |= ucTempOffset;
			iBitsThisByte -= ( int ) ucBitsPerOffset;
			if( iBitsThisByte <= 0 )
			{
				//=== put the remaining bits in the next byte ===//
				pu8Len[0]++;//increment chopped length
				pu8RetChoppedStr++;
				*pu8RetChoppedStr = ( char )( ucThisOffset << ( 8 - ( ucBitsPerOffset - 1 )));
				iBitsThisByte += 8;
			}
			break;
		case 2:
			if( ucBitsPerOffset >= 2 )
			{
				ucTempOffset = ucThisOffset >> ( ucBitsPerOffset - 2 );
			}
			else
			{
				ucTempOffset = ucThisOffset << ( 2 - ucBitsPerOffset );
			}
			*pu8RetChoppedStr |= ucTempOffset;
			iBitsThisByte -= ( int ) ucBitsPerOffset;
			if( iBitsThisByte <= 0 )
			{
				//=== put the remaining bits in the next byte ===//
				pu8Len[0]++;//increment chopped length
				pu8RetChoppedStr++;
				*pu8RetChoppedStr = ( char )( ucThisOffset << ( 8 - ( ucBitsPerOffset - 2 )));
				iBitsThisByte += 8;
			}
			break;
		case 3:
			if( ucBitsPerOffset >= 3 )
			{
				ucTempOffset = ucThisOffset >> ( ucBitsPerOffset - 3 );
			}
			else
			{
				ucTempOffset = ucThisOffset << ( 3 - ucBitsPerOffset );
			}
			*pu8RetChoppedStr |= ucTempOffset;
			iBitsThisByte -= ( int ) ucBitsPerOffset;
			if( iBitsThisByte <= 0 )
			{
				//=== put the remaining bits in the next byte ===//
				pu8Len[0]++;//increment chopped length
				pu8RetChoppedStr++;
				*pu8RetChoppedStr = ( char )( ucThisOffset << ( 8 - ( ucBitsPerOffset - 3 )));
				iBitsThisByte += 8;
			}
			break;
		case 4:
			if( ucBitsPerOffset >= 4 )
			{
				ucTempOffset = ucThisOffset >> ( ucBitsPerOffset - 4 );
			}
			else
			{
				ucTempOffset = ucThisOffset << ( 4 - ucBitsPerOffset );
			}
			*pu8RetChoppedStr |= ucTempOffset;
			iBitsThisByte -= ( int ) ucBitsPerOffset;
			if( iBitsThisByte <= 0 )
			{
				//=== put the remaining bits in the next byte ===//
				pu8Len[0]++;//increment chopped length
				pu8RetChoppedStr++;
				*pu8RetChoppedStr = ( char )( ucThisOffset << ( 8 - ( ucBitsPerOffset - 4 )));
				iBitsThisByte += 8;
			}
			break;
		case 5:
			if( ucBitsPerOffset >= 5 )
			{
				ucTempOffset = ucThisOffset >> ( ucBitsPerOffset - 5 );
			}
			else
			{
				ucTempOffset = ucThisOffset << ( 5 - ucBitsPerOffset );
			}
			*pu8RetChoppedStr |= ucTempOffset;
			iBitsThisByte -= ( int ) ucBitsPerOffset;
			if( iBitsThisByte <= 0 )
			{
				//=== put the remaining bits in the next byte ===//
				pu8Len[ 0 ]++;//increment chopped length
				pu8RetChoppedStr++;
				*pu8RetChoppedStr =( char )(  ucThisOffset << ( 8 - ( ucBitsPerOffset - 5 )));
				iBitsThisByte += 8;
			}
			break;
		case 6:
			if( ucBitsPerOffset >= 6 )
			{
				ucTempOffset = ucThisOffset >> ( ucBitsPerOffset - 6 );
			}
			else
			{
				ucTempOffset = ucThisOffset << ( 6 - ucBitsPerOffset );
			}
			*pu8RetChoppedStr |= ucTempOffset;
			iBitsThisByte -= ( int ) ucBitsPerOffset;
			if( iBitsThisByte <= 0 )
			{
				//=== put the remaining bits in the next byte ===//
				pu8Len[0]++;//increment chopped length
				pu8RetChoppedStr++;
				*pu8RetChoppedStr = ( char )( ucThisOffset << ( 8 - ( ucBitsPerOffset - 6 )));
				iBitsThisByte += 8;
			}
			break;
		case 7:
			if( ucBitsPerOffset >= 7 )
			{
				ucTempOffset = ucThisOffset >> ( ucBitsPerOffset - 7 );
			}
			else
			{
				ucTempOffset = ucThisOffset << ( 7 - ucBitsPerOffset );
			}
			*pu8RetChoppedStr |= ucTempOffset;
			iBitsThisByte -= ( int ) ucBitsPerOffset;
			if( iBitsThisByte <= 0 )
			{
				//=== put the remaining bits in the next byte ===//
				pu8Len[0]++;//increment chopped length
				pu8RetChoppedStr++;
				*pu8RetChoppedStr = ( char )( ucThisOffset << ( 8 - ( ucBitsPerOffset - 7 )));
				iBitsThisByte += 8;
			}
			break;
		case 8:
			if( ucBitsPerOffset >= 8 )
			{
				ucTempOffset = ucThisOffset >> ( ucBitsPerOffset - 8 );
			}
			else
			{
				ucTempOffset = ucThisOffset << ( 8 - ucBitsPerOffset );
			}
			*pu8RetChoppedStr = ucTempOffset;
			iBitsThisByte -= ( int ) ucBitsPerOffset;
			if( iBitsThisByte <= 0 )
			{
				//=== put the remaining bits in the next byte ===//
				pu8Len[0]++;//increment chopped length
				pu8RetChoppedStr++;
				*pu8RetChoppedStr = ( char )( ucThisOffset << ( 8 - ( ucBitsPerOffset - 8 )));
				iBitsThisByte += 8;
			}
			break;
		}
	}while( * ( pStr - 1) != ucBaseLevel );
	pucTemp[ iDataLen ] = 0; // re null terminate the string
#ifdef USE_MAPPING
	delete( pucTemp );
#endif
	return *pu8Len; //return the length of the chopped str
}
// format of chopped chunk of data 2 byte header
// Byte 1 = Length of uncompressed data
// Byte 2 Bits 7-3 represent Base Level of data chunk
// Byte 2- Bits 0-2 represent Number of Bits need to represent each offset from base line
//
//============================================================================
//UnChopStr uncompresses data into pRetUnchoppedStr
void VxUnchopStr(  unsigned char * pu8ChoppedStr, char * pRetUnchoppedStr )
{
	unsigned char ucTemp;
	if( 0 == * pu8ChoppedStr )
	{
		* pRetUnchoppedStr = 0;
		return;
	}

	unsigned char u8Len = *((unsigned char *)pu8ChoppedStr );
	pu8ChoppedStr++;
	unsigned char ucBitsPerOffset = (* pu8ChoppedStr & 0x07) + 1;// number of bits per offset
	unsigned char ucBaseLevel = *pu8ChoppedStr & 0xf8;
	pu8ChoppedStr++;
	unsigned char ucOffsetMask = 0xff >> (8 - ucBitsPerOffset );
#ifdef USE_MAPPING
	char * pSavedRetUnchoppedStrPointer = pRetUnchoppedStr;
#endif

	//=== unpack the data ===//
	int iBitsThisByte = 8;//number of bits left to unpack in this byte
	vx_assert( ucBitsPerOffset );
	while( 1 )
	{
		switch( iBitsThisByte )
		{
		case 1:
			ucTemp =  ((unsigned char )* pu8ChoppedStr & 0x01) << ( ucBitsPerOffset - 1 );
			pu8ChoppedStr++;
			iBitsThisByte = 8;
			//=== got what we could from that byte, get the rest from the next one ===//.
			ucTemp |= (unsigned char )* pu8ChoppedStr >> ( 8 - ( ucBitsPerOffset - 1 ));
			ucTemp &= ucOffsetMask;
			* pRetUnchoppedStr = (char )( ucTemp + ucBaseLevel);
			if( ucBaseLevel == * pRetUnchoppedStr )
			{
				goto Done;
			}
			iBitsThisByte -= ( ucBitsPerOffset - 1);
			pRetUnchoppedStr++;
			break;
		case 2:
			if( 2 >= ucBitsPerOffset )
			{
				//=== we have enough bits so shift em into position ===//
				ucTemp = * pu8ChoppedStr >> ( 2 - ucBitsPerOffset);
				* pRetUnchoppedStr = (char )(( ucTemp & ucOffsetMask )+ ucBaseLevel);	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				pRetUnchoppedStr++;
				iBitsThisByte -= ( int ) ucBitsPerOffset;
				if( 0 == iBitsThisByte )
				{
					iBitsThisByte = 8;
					pu8ChoppedStr++;
				}
			}
			else
			{
				ucTemp =  ((unsigned char )* pu8ChoppedStr & 0x03) << ( ucBitsPerOffset - 2 );
				pu8ChoppedStr++;
				u8Len--;//decrement length
				iBitsThisByte = 8;
				//=== got what we could from that byte, get the rest from the next one ===//.
				ucTemp |= (unsigned char )* pu8ChoppedStr >> ( 8 - ( ucBitsPerOffset - 2 ));
				ucTemp &= ucOffsetMask;
				* pRetUnchoppedStr =  (char )( ucTemp + ucBaseLevel);	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				iBitsThisByte -= ( ucBitsPerOffset - 2 );
				pRetUnchoppedStr++;
			}
			break;
		case 3:
			if( 3 >= ucBitsPerOffset )
			{
				//=== we have enough bits so shift em into position ===//
				ucTemp = ((unsigned char )* pu8ChoppedStr & 0x07) >> ( 3 - ucBitsPerOffset);
				* pRetUnchoppedStr = ( ucTemp & ucOffsetMask )+ ucBaseLevel;	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				pRetUnchoppedStr++;
				iBitsThisByte -= ( int ) ucBitsPerOffset;
				if( 0 == iBitsThisByte )
				{
					iBitsThisByte = 8;
					pu8ChoppedStr++;
				}
			}
			else
			{
				ucTemp =  ((unsigned char )* pu8ChoppedStr & 0x07) << ( ucBitsPerOffset - 3 );
				pu8ChoppedStr++;
				u8Len--;//decrement length
				iBitsThisByte = 8;
				//=== got what we could from that byte, get the rest from the next one ===//.
				ucTemp |= (unsigned char )* pu8ChoppedStr >> ( 8 - ( ucBitsPerOffset - 3 ));
				ucTemp &= ucOffsetMask;
				* pRetUnchoppedStr =  (char )( ucTemp + ucBaseLevel);	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				iBitsThisByte -= ( ucBitsPerOffset - 3 );
				pRetUnchoppedStr++;
			}
			break;
		case 4:
			if( 4 >= ucBitsPerOffset )
			{
				//=== we have enough bits so shift em into position ===//
				ucTemp = ((unsigned char )* pu8ChoppedStr & 0x0f) >> ( 4 - ucBitsPerOffset);
				* pRetUnchoppedStr = ( ucTemp & ucOffsetMask )+ ucBaseLevel;	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				pRetUnchoppedStr++;
				iBitsThisByte -= ( int ) ucBitsPerOffset;
				if( 0 == iBitsThisByte )
				{
					iBitsThisByte = 8;
					pu8ChoppedStr++;
				}
			}
			else
			{
				ucTemp =  ((unsigned char )* pu8ChoppedStr & 0x0f) << ( ucBitsPerOffset - 4 );
				pu8ChoppedStr++;
				u8Len--;//decrement length
				iBitsThisByte = 8;
				//=== got what we could from that byte, get the rest from the next one ===//
				ucTemp |= ((unsigned char )* pu8ChoppedStr) >>  ( 8 - ( ucBitsPerOffset - 4 ) );
				ucTemp &= ucOffsetMask;
				* pRetUnchoppedStr =  (char )( ucTemp + ucBaseLevel);	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				pRetUnchoppedStr++;
				iBitsThisByte -= ( ucBitsPerOffset - 4 );
			}
			break;
		case 5:
			if( 5 >= ucBitsPerOffset )
			{
				//=== we have enough bits so shift em into position ===//
				ucTemp = ((unsigned char )* pu8ChoppedStr & 0x1f) >> ( 5 - ucBitsPerOffset);
				* pRetUnchoppedStr = ( ucTemp & ucOffsetMask )+ ucBaseLevel;	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				pRetUnchoppedStr++;
				iBitsThisByte -= ( int ) ucBitsPerOffset;
				if( 0 == iBitsThisByte )
				{
					iBitsThisByte = 8;
					pu8ChoppedStr++;
				}
			}
			else
			{
				ucTemp =  ((unsigned char )* pu8ChoppedStr & 0x1f) << ( ucBitsPerOffset - 5 );
				pu8ChoppedStr++;
				u8Len--;//decrement length
				iBitsThisByte = 8;
				//=== got what we could from that byte, get the rest from the next one ===//.
				ucTemp |= (unsigned char )* pu8ChoppedStr >> ( 8 - ( ucBitsPerOffset - 5 ));
				ucTemp &= ucOffsetMask;
				* pRetUnchoppedStr =  (char )( ucTemp + ucBaseLevel);	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				iBitsThisByte -= ( ucBitsPerOffset - 5 );
				pRetUnchoppedStr++;
			}
			break;
		case 6:
			if( 6 >= ucBitsPerOffset )
			{
				//=== we have enough bits so shift em into position ===//
				ucTemp = ((unsigned char )* pu8ChoppedStr & 0x3f) >> ( 6 - ucBitsPerOffset);
				* pRetUnchoppedStr = ( ucTemp & ucOffsetMask )+ ucBaseLevel;	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				pRetUnchoppedStr++;
				iBitsThisByte -= ( int ) ucBitsPerOffset;
				if( 0 == iBitsThisByte )
				{
					iBitsThisByte = 8;
					pu8ChoppedStr++;
				}
			}
			else
			{
				ucTemp =  ((unsigned char )* pu8ChoppedStr & 0x3f) << ( ucBitsPerOffset - 6 );
				pu8ChoppedStr++;
				u8Len--;//decrement length
				iBitsThisByte = 8;
				//=== got what we could from that byte, get the rest from the next one ===//.
				ucTemp |= (unsigned char )* pu8ChoppedStr >> ( 8 - ( ucBitsPerOffset - 6 ));
				ucTemp &= ucOffsetMask;
				* pRetUnchoppedStr = ucTemp + ucBaseLevel;	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				iBitsThisByte -= ( ucBitsPerOffset - 6 );
				pRetUnchoppedStr++;
			}
			break;
		case 7:
			if( 7 >= ucBitsPerOffset )
			{
				//=== we have enough bits so shift em into position ===//
				ucTemp = ((unsigned char )* pu8ChoppedStr & 0x7f) >> ( 7 - ucBitsPerOffset);
				* pRetUnchoppedStr = ( ucTemp & ucOffsetMask ) + ucBaseLevel;	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				pRetUnchoppedStr++;
				iBitsThisByte -= ( int ) ucBitsPerOffset;
				if( 0 == iBitsThisByte )
				{
					iBitsThisByte = 8;
					pu8ChoppedStr++;
				}
			}
			else
			{
				ucTemp =  ((unsigned char )* pu8ChoppedStr & 0x7f) << ( ucBitsPerOffset - 7 );
				pu8ChoppedStr++;
				u8Len--;//decrement length
				iBitsThisByte = 8;
				//=== got what we could from that byte, get the rest from the next one ===//.
				ucTemp |= (unsigned char )* pu8ChoppedStr >> ( 8 - ( ucBitsPerOffset - 7 ));
				ucTemp &= ucOffsetMask;
				* pRetUnchoppedStr = ucTemp  + ucBaseLevel;	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				iBitsThisByte -= ( ucBitsPerOffset - 7 );
				pRetUnchoppedStr++;
			}
			break;
		case 8:
			if( 8 >= ucBitsPerOffset )
			{
				//=== we have enough bits so shift em into position ===//
				ucTemp = (unsigned char )* pu8ChoppedStr >> ( 8 - ucBitsPerOffset);
				* pRetUnchoppedStr = ( ucTemp & ucOffsetMask )+ ucBaseLevel;	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				pRetUnchoppedStr++;
				iBitsThisByte -= ( int ) ucBitsPerOffset;
				if( 0 == iBitsThisByte )
				{
					iBitsThisByte = 8;
					pu8ChoppedStr++;
				}
			}
			else
			{
				ucTemp =  (unsigned char )* pu8ChoppedStr << ( ucBitsPerOffset - 8 );
				pu8ChoppedStr++;
				u8Len--;//decrement length
				iBitsThisByte = 8;
				//=== got what we could from that byte, get the rest from the next one ===//.
				ucTemp |= (unsigned char )* pu8ChoppedStr >> ucBitsPerOffset;
				ucTemp &= ucOffsetMask;
				* pRetUnchoppedStr = ucTemp + ucBaseLevel;	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				iBitsThisByte -= ( ucBitsPerOffset - 8 );
				pRetUnchoppedStr++;
			}
			break;
		}
	}
Done:
	*pRetUnchoppedStr = 0;

#ifdef USE_MAPPING
	VxUnMapStr( pSavedRetUnchoppedStrPointer );
#endif
}

//============================================================================
// unicode
//============================================================================

//============================================================================
int VxChopStr( std::wstring &csStr, uint16_t * pu8RetChoppedStr )
{
	return VxChopStr( (const wchar_t *)csStr.c_str(), pu8RetChoppedStr );
}
//============================================================================
void VxUnchopStr( uint16_t * pu8ChoppedStr, std::wstring &csRetStr )
{
	vx_assert( pu8ChoppedStr );
	if( 0 == * pu8ChoppedStr )
	{
		csRetStr = L"";
		return;
	}

	wchar_t as8Buf[ VX_MAX_PATH ];
	VxUnchopStr( pu8ChoppedStr, as8Buf );
	csRetStr = as8Buf;
}

//============================================================================
int VxCalcChoppedLen( const wchar_t * pStrIn )
{
	uint16_t ucBitsPerOffset;
	uint16_t ucHighest = 0;
	uint16_t ucBaseLevel = 0xffff;
	if( 0 == * pStrIn )
		return 0;
	int iDataLen = (int)wstrlen( pStrIn );
	wchar_t * pStr;

#ifdef USE_MAPPING
	pStr = new wchar_t[ iDataLen + 1 ]; // make a copy before mapping so don't mess up callers original
	wstrcpy( pStr, pStrIn );
	VxMapStr( pStr ); // remap the chars
#else
	pStr = pStrIn;
#endif
	uint16_t * pucTemp = ( uint16_t *)pStr;
	//=== determine the highest and lowest values in the data stream ===//
	while( * pucTemp )
	{
		ucHighest = * pucTemp > ucHighest ? * pucTemp : ucHighest;
		ucBaseLevel = * pucTemp < ucBaseLevel ? * pucTemp : ucBaseLevel;
		pucTemp++;
	}
#ifdef USE_MAPPING
	delete pStr;
#endif

	// base level must be lowest value - 1 so that zero is unique and equal to base level
	if( ucBaseLevel > 1 )
		ucBaseLevel -= 1;
	ucBaseLevel = ucBaseLevel & 0xfff0; //round base level down to nearest multiple of 16

	uint16_t ucBiggestOffset = ucHighest - ucBaseLevel; //this is the range of offsets we need to store

	ucBitsPerOffset = 16;	// start at worse case
	// we would need 16 bits to represent each byte offset
	// if the lowest value in chunk is 0 and the highest is 0xffff
	uint16_t ucHighestBitMask = 0x8000;
	do
	{
		if( ucHighestBitMask & ucBiggestOffset )
		{
			break;
		}
		ucHighestBitMask  = ucHighestBitMask >> 1;
		ucBitsPerOffset--;
	}while( ucHighestBitMask > 1);//for simplicity of coding we leave at least 1 bit per offset

	int iTotalBits = iDataLen * ucBitsPerOffset;
	int iTotalBytes = iTotalBits / 16;
	if( iTotalBits
		% 16 )
	{
		iTotalBytes++;
	}
	iTotalBytes+=2; //one byte for header //one byte for length

	return iTotalBytes;
}
//============================================================================
int VxGetUnchopedStrLen( wchar_t * pu8ChoppedStr )
{
	return (int)wstrlen( pu8ChoppedStr );
}

// format of chopped string of data 1 byte header
// Byte 1, Bits 15-4 represent Base Level of data chunk
// Byte 1, Bits 0-3 represent Number of Bits need to represent each offset from base line

//============================================================================
// ChopStr returns Compressed length
int VxChopStr( const wchar_t * pStrIn, uint16_t * pu8RetChoppedStr )
{
	uint16_t ucBiggestOffset;

	uint16_t ucBitsPerOffset;		// number of bits per offset
	uint16_t ucBitsPerOffsetMinus1;// number of bits ( minus 1 ) required to represent the largest offset from the base level
	// so a value of 16 would represent the worst case of 16 bits needed
	uint16_t ucHighest = 0;
	uint16_t ucBaseLevel = 0xffff;
	if( 0 == * pStrIn )
	{
		* pu8RetChoppedStr = 0;
		return 0;
	}
	int iDataLen = (int)wstrlen( pStrIn );
	vx_assert( iDataLen < 255 );
	wchar_t * pStr;
#ifdef USE_MAPPING
	pStr = new wchar_t[ iDataLen + 1 ] ; // make a copy before mapping so don't mess up callers original
	wstrcpy( pStr, pStrIn );
	VxMapStr( pStr ); // remap the chars
#else
	pStr = pStrIn;
#endif
	uint16_t *pucTemp = (uint16_t *)pStr;

	//=== determine the highest and lowest values in the data stream ===//
	while( * pucTemp )
	{
		ucHighest = * pucTemp > ucHighest ? * pucTemp : ucHighest;
		ucBaseLevel = * pucTemp < ucBaseLevel ? * pucTemp : ucBaseLevel;
		pucTemp++;
	}
	pucTemp = ( uint16_t *)pStr; // save so we can delete after compression is done

	//=== determine base level value to subtract from each byte in data chunk ===//
	// this is the base value of the data chunk.
	// the data will be stored as offsets to be added to this level when decompressed

	// base level must be lowest value - 1 so that zero is unique and equal to base level
	// for strings we want to make sure every sample has at least one set bit
	// so that we don't get zeros in our string so subtract one from base level
	if( ucBaseLevel > 1 )
		ucBaseLevel -= 1;
	ucBaseLevel = ucBaseLevel & 0xfff0; //round base level down to nearest multiple of 16

	//=== calculate the number of bits required to represent all offsets in the stream
	ucBiggestOffset = ucHighest - ucBaseLevel; //this is the range of offsets we need to store

	ucBitsPerOffsetMinus1 = 15;	// start at worse case
	// we would need 16 bits to represent each byte offset
	//if the lowest value in chunk is 0 and the highest is 0xff
	uint16_t ucHighestBitMask = 0x8000;
	do
	{
		if( ucHighestBitMask & ucBiggestOffset )
		{
			break;
		}
		ucHighestBitMask  = ucHighestBitMask >> 1;
		ucBitsPerOffsetMinus1--;
	}while( ucHighestBitMask > 1);//for simplicity of coding we leave at least 1 bit per offset

	uint16_t * pu8Len = ( uint16_t * )pu8RetChoppedStr;
	*pu8Len = 3;//length header plus at least on char in string

	pu8RetChoppedStr++;

	* pu8RetChoppedStr = ucBaseLevel | ucBitsPerOffsetMinus1; // fill in the chunk header
	pu8RetChoppedStr++;

	//=== now pack the offsets that represent the data ===//
	ucBitsPerOffset = ucBitsPerOffsetMinus1 + 1;
	int iBitsThisByte = 16;//number of bits available to pack in this byte
	uint16_t ucThisOffset;
	uint16_t ucTempOffset;
	pStr[ iDataLen ] = ucBaseLevel; // set base level so when reaches this then we are done
	do
	{
		ucThisOffset =  ( uint16_t )*pStr - ucBaseLevel;
		pStr++;
		switch( iBitsThisByte )
		{
		case 1:
			ucTempOffset = ucThisOffset >> ( ucBitsPerOffset - 1);
			*pu8RetChoppedStr |= ucTempOffset;
			iBitsThisByte -= ( int ) ucBitsPerOffset;
			if( iBitsThisByte <= 0 )
			{
				//=== put the remaining bits in the next byte ===//
				pu8Len[0]++;//increment chopped length
				pu8RetChoppedStr++;
				*pu8RetChoppedStr = ( uint16_t )( ucThisOffset << ( 16 - ( ucBitsPerOffset - 1 )));
				iBitsThisByte += 16;
			}
			break;
		case 2:
			if( ucBitsPerOffset >= 2 )
			{
				ucTempOffset = ucThisOffset >> ( ucBitsPerOffset - 2 );
			}
			else
			{
				ucTempOffset = ucThisOffset << ( 2 - ucBitsPerOffset );
			}
			*pu8RetChoppedStr |= ucTempOffset;
			iBitsThisByte -= ( int ) ucBitsPerOffset;
			if( iBitsThisByte <= 0 )
			{
				//=== put the remaining bits in the next byte ===//
				pu8Len[0]++;//increment chopped length
				pu8RetChoppedStr++;
				*pu8RetChoppedStr = ( uint16_t )( ucThisOffset << ( 16 - ( ucBitsPerOffset - 2 )));
				iBitsThisByte += 16;
			}
			break;
		case 3:
			if( ucBitsPerOffset >= 3 )
			{
				ucTempOffset = ucThisOffset >> ( ucBitsPerOffset - 3 );
			}
			else
			{
				ucTempOffset = ucThisOffset << ( 3 - ucBitsPerOffset );
			}
			*pu8RetChoppedStr |= ucTempOffset;
			iBitsThisByte -= ( int ) ucBitsPerOffset;
			if( iBitsThisByte <= 0 )
			{
				//=== put the remaining bits in the next byte ===//
				pu8Len[0]++;//increment chopped length
				pu8RetChoppedStr++;
				*pu8RetChoppedStr = ( uint16_t )( ucThisOffset << ( 16 - ( ucBitsPerOffset - 3 )));
				iBitsThisByte += 16;
			}
			break;
		case 4:
			if( ucBitsPerOffset >= 4 )
			{
				ucTempOffset = ucThisOffset >> ( ucBitsPerOffset - 4 );
			}
			else
			{
				ucTempOffset = ucThisOffset << ( 4 - ucBitsPerOffset );
			}
			*pu8RetChoppedStr |= ucTempOffset;
			iBitsThisByte -= ( int ) ucBitsPerOffset;
			if( iBitsThisByte <= 0 )
			{
				//=== put the remaining bits in the next byte ===//
				pu8Len[0]++;//increment chopped length
				pu8RetChoppedStr++;
				*pu8RetChoppedStr = ( uint16_t )( ucThisOffset << ( 16 - ( ucBitsPerOffset - 4 )));
				iBitsThisByte += 16;
			}
			break;
		case 5:
			if( ucBitsPerOffset >= 5 )
			{
				ucTempOffset = ucThisOffset >> ( ucBitsPerOffset - 5 );
			}
			else
			{
				ucTempOffset = ucThisOffset << ( 5 - ucBitsPerOffset );
			}
			*pu8RetChoppedStr |= ucTempOffset;
			iBitsThisByte -= ( int ) ucBitsPerOffset;
			if( iBitsThisByte <= 0 )
			{
				//=== put the remaining bits in the next byte ===//
				pu8Len[ 0 ]++;//increment chopped length
				pu8RetChoppedStr++;
				*pu8RetChoppedStr =( uint16_t )(  ucThisOffset << ( 16 - ( ucBitsPerOffset - 5 )));
				iBitsThisByte += 16;
			}
			break;
		case 6:
			if( ucBitsPerOffset >= 6 )
			{
				ucTempOffset = ucThisOffset >> ( ucBitsPerOffset - 6 );
			}
			else
			{
				ucTempOffset = ucThisOffset << ( 6 - ucBitsPerOffset );
			}
			*pu8RetChoppedStr |= ucTempOffset;
			iBitsThisByte -= ( int ) ucBitsPerOffset;
			if( iBitsThisByte <= 0 )
			{
				//=== put the remaining bits in the next byte ===//
				pu8Len[0]++;//increment chopped length
				pu8RetChoppedStr++;
				*pu8RetChoppedStr = ( uint16_t )( ucThisOffset << ( 16 - ( ucBitsPerOffset - 6 )));
				iBitsThisByte += 16;
			}
			break;
		case 7:
			if( ucBitsPerOffset >= 7 )
			{
				ucTempOffset = ucThisOffset >> ( ucBitsPerOffset - 7 );
			}
			else
			{
				ucTempOffset = ucThisOffset << ( 7 - ucBitsPerOffset );
			}
			*pu8RetChoppedStr |= ucTempOffset;
			iBitsThisByte -= ( int ) ucBitsPerOffset;
			if( iBitsThisByte <= 0 )
			{
				//=== put the remaining bits in the next byte ===//
				pu8Len[0]++;//increment chopped length
				pu8RetChoppedStr++;
				*pu8RetChoppedStr = ( uint16_t )( ucThisOffset << ( 16 - ( ucBitsPerOffset - 7 )));
				iBitsThisByte += 16;
			}
			break;
		case 8:
			if( ucBitsPerOffset >= 8 )
			{
				ucTempOffset = ucThisOffset >> ( ucBitsPerOffset - 8 );
			}
			else
			{
				ucTempOffset = ucThisOffset << ( 8 - ucBitsPerOffset );
			}
			*pu8RetChoppedStr = ucTempOffset;
			iBitsThisByte -= ( int ) ucBitsPerOffset;
			if( iBitsThisByte <= 0 )
			{
				//=== put the remaining bits in the next byte ===//
				pu8Len[0]++;//increment chopped length
				pu8RetChoppedStr++;
				*pu8RetChoppedStr = ( uint16_t )( ucThisOffset << ( 16 - ( ucBitsPerOffset - 8 )));
				iBitsThisByte += 16;
			}
			break;
		case 9:
			if( ucBitsPerOffset >= 9 )
			{
				ucTempOffset = ucThisOffset >> ( ucBitsPerOffset - 9 );
			}
			else
			{
				ucTempOffset = ucThisOffset << ( 9 - ucBitsPerOffset );
			}
			*pu8RetChoppedStr = ucTempOffset;
			iBitsThisByte -= ( int ) ucBitsPerOffset;
			if( iBitsThisByte <= 0 )
			{
				//=== put the remaining bits in the next byte ===//
				pu8Len[0]++;//increment chopped length
				pu8RetChoppedStr++;
				*pu8RetChoppedStr = ( uint16_t )( ucThisOffset << ( 16 - ( ucBitsPerOffset - 9 )));
				iBitsThisByte += 16;
			}
			break;
		case 10:
			if( ucBitsPerOffset >= 10 )
			{
				ucTempOffset = ucThisOffset >> ( ucBitsPerOffset - 10 );
			}
			else
			{
				ucTempOffset = ucThisOffset << ( 10 - ucBitsPerOffset );
			}
			*pu8RetChoppedStr = ucTempOffset;
			iBitsThisByte -= ( int ) ucBitsPerOffset;
			if( iBitsThisByte <= 0 )
			{
				//=== put the remaining bits in the next byte ===//
				pu8Len[0]++;//increment chopped length
				pu8RetChoppedStr++;
				*pu8RetChoppedStr = ( uint16_t )( ucThisOffset << ( 16 - ( ucBitsPerOffset - 10 )));
				iBitsThisByte += 16;
			}
			break;
		case 11:
			if( ucBitsPerOffset >= 11 )
			{
				ucTempOffset = ucThisOffset >> ( ucBitsPerOffset - 11 );
			}
			else
			{
				ucTempOffset = ucThisOffset << ( 16 - ucBitsPerOffset );
			}
			*pu8RetChoppedStr = ucTempOffset;
			iBitsThisByte -= ( int ) ucBitsPerOffset;
			if( iBitsThisByte <= 0 )
			{
				//=== put the remaining bits in the next byte ===//
				pu8Len[0]++;//increment chopped length
				pu8RetChoppedStr++;
				*pu8RetChoppedStr = ( uint16_t )( ucThisOffset << ( 16 - ( ucBitsPerOffset - 11 )));
				iBitsThisByte += 16;
			}
			break;
		case 12:
			if( ucBitsPerOffset >= 12 )
			{
				ucTempOffset = ucThisOffset >> ( ucBitsPerOffset - 12 );
			}
			else
			{
				ucTempOffset = ucThisOffset << ( 12 - ucBitsPerOffset );
			}
			*pu8RetChoppedStr = ucTempOffset;
			iBitsThisByte -= ( int ) ucBitsPerOffset;
			if( iBitsThisByte <= 0 )
			{
				//=== put the remaining bits in the next byte ===//
				pu8Len[0]++;//increment chopped length
				pu8RetChoppedStr++;
				*pu8RetChoppedStr = ( uint16_t )( ucThisOffset << ( 16 - ( ucBitsPerOffset - 12 )));
				iBitsThisByte += 16;
			}
			break;
		case 13:
			if( ucBitsPerOffset >= 13 )
			{
				ucTempOffset = ucThisOffset >> ( ucBitsPerOffset - 13 );
			}
			else
			{
				ucTempOffset = ucThisOffset << ( 13 - ucBitsPerOffset );
			}
			*pu8RetChoppedStr = ucTempOffset;
			iBitsThisByte -= ( int ) ucBitsPerOffset;
			if( iBitsThisByte <= 0 )
			{
				//=== put the remaining bits in the next byte ===//
				pu8Len[0]++;//increment chopped length
				pu8RetChoppedStr++;
				*pu8RetChoppedStr = ( uint16_t )( ucThisOffset << ( 16 - ( ucBitsPerOffset - 13 )));
				iBitsThisByte += 16;
			}
			break;
		case 14:
			if( ucBitsPerOffset >= 14 )
			{
				ucTempOffset = ucThisOffset >> ( ucBitsPerOffset - 14 );
			}
			else
			{
				ucTempOffset = ucThisOffset << ( 14 - ucBitsPerOffset );
			}
			*pu8RetChoppedStr = ucTempOffset;
			iBitsThisByte -= ( int ) ucBitsPerOffset;
			if( iBitsThisByte <= 0 )
			{
				//=== put the remaining bits in the next byte ===//
				pu8Len[0]++;//increment chopped length
				pu8RetChoppedStr++;
				*pu8RetChoppedStr = ( uint16_t )( ucThisOffset << ( 16 - ( ucBitsPerOffset - 14 )));
				iBitsThisByte += 16;
			}
			break;
		case 15:
			if( ucBitsPerOffset >= 15 )
			{
				ucTempOffset = ucThisOffset >> ( ucBitsPerOffset - 15 );
			}
			else
			{
				ucTempOffset = ucThisOffset << ( 15 - ucBitsPerOffset );
			}
			*pu8RetChoppedStr = ucTempOffset;
			iBitsThisByte -= ( int ) ucBitsPerOffset;
			if( iBitsThisByte <= 0 )
			{
				//=== put the remaining bits in the next byte ===//
				pu8Len[0]++;//increment chopped length
				pu8RetChoppedStr++;
				*pu8RetChoppedStr = ( uint16_t )( ucThisOffset << ( 16 - ( ucBitsPerOffset - 15 )));
				iBitsThisByte += 16;
			}
			break;
		case 16:
			if( ucBitsPerOffset >= 16 )
			{
				ucTempOffset = ucThisOffset >> ( ucBitsPerOffset - 16 );
			}
			else
			{
				ucTempOffset = ucThisOffset << ( 16 - ucBitsPerOffset );
			}
			*pu8RetChoppedStr = ucTempOffset;
			iBitsThisByte -= ( int ) ucBitsPerOffset;
			if( iBitsThisByte <= 0 )
			{
				//=== put the remaining bits in the next byte ===//
				pu8Len[0]++;//increment chopped length
				pu8RetChoppedStr++;
				*pu8RetChoppedStr = ( uint16_t )( ucThisOffset << ( 16 - ( ucBitsPerOffset - 16 )));
				iBitsThisByte += 16;
			}
			break;
		}
	}while( * ( pStr - 1) != ucBaseLevel );
	pucTemp[ iDataLen ] = 0; // re null terminate the string
#ifdef USE_MAPPING
	delete( pucTemp );
#endif
	return *pu8Len; //return the length of the chopped str
}
// format of chopped chunk of data 2 byte header
// Byte 1 = Length of uncompressed data
// Byte 2 Bits 15-4 represent Base Level of data chunk
// Byte 2- Bits 0-3 represent Number of Bits need to represent each offset from base line
//
//============================================================================
//UnChopStr returns the uncompressed data length
void VxUnchopStr(  uint16_t * pu8ChoppedStr, wchar_t * pRetUnchoppedStr )
{
	uint16_t ucTemp;
	if( 0 == * pu8ChoppedStr )
	{
		* pRetUnchoppedStr = 0;
		return;
	}
	uint16_t u8Len = *((uint16_t *)pu8ChoppedStr );
	pu8ChoppedStr++;
	uint16_t ucBitsPerOffset = (* pu8ChoppedStr & 0x0F) + 1;// number of bits per offset
	uint16_t ucBaseLevel = *pu8ChoppedStr & 0xfff0;
	pu8ChoppedStr++;
	uint16_t ucOffsetMask = 0xffff >> (16 - ucBitsPerOffset );
#ifdef USE_MAPPING
	wchar_t * pSavedRetUnchoppedStrPointer = pRetUnchoppedStr;
#endif

	//=== unpack the data ===//
	int iBitsThisByte = 16;//number of bits left to unpack in this byte
	vx_assert( ucBitsPerOffset );
	while( 1 )
	{
		switch( iBitsThisByte )
		{
		case 1:
			ucTemp =  (( uint16_t )* pu8ChoppedStr & 0x01) << ( ucBitsPerOffset - 1 );
			pu8ChoppedStr++;
			iBitsThisByte = 16;
			//=== got what we could from that byte, get the rest from the next one ===//.
			ucTemp |= (uint16_t )* pu8ChoppedStr >> ( 16 - ( ucBitsPerOffset - 1 ));
			ucTemp &= ucOffsetMask;
			* pRetUnchoppedStr = (char )( ucTemp + ucBaseLevel);
			if( ucBaseLevel == * pRetUnchoppedStr )
			{
				goto Done;
			}
			iBitsThisByte -= ( ucBitsPerOffset - 1);
			pRetUnchoppedStr++;
			break;
		case 2:
			if( 2 >= ucBitsPerOffset )
			{
				//=== we have enough bits so shift em into position ===//
				ucTemp = * pu8ChoppedStr >> ( 2 - ucBitsPerOffset);
				* pRetUnchoppedStr = (char )(( ucTemp & ucOffsetMask )+ ucBaseLevel);	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				pRetUnchoppedStr++;
				iBitsThisByte -= ( int ) ucBitsPerOffset;
				if( 0 == iBitsThisByte )
				{
					iBitsThisByte = 16;
					pu8ChoppedStr++;
				}
			}
			else
			{
				ucTemp =  ((uint16_t )* pu8ChoppedStr & 0x03) << ( ucBitsPerOffset - 2 );
				pu8ChoppedStr++;
				u8Len--;//decrement length
				iBitsThisByte = 16;
				//=== got what we could from that byte, get the rest from the next one ===//.
				ucTemp |= (uint16_t )* pu8ChoppedStr >> ( 16 - ( ucBitsPerOffset - 2 ));
				ucTemp &= ucOffsetMask;
				* pRetUnchoppedStr =  (char )( ucTemp + ucBaseLevel);	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				iBitsThisByte -= ( ucBitsPerOffset - 2 );
				pRetUnchoppedStr++;
			}
			break;
		case 3:
			if( 3 >= ucBitsPerOffset )
			{
				//=== we have enough bits so shift em into position ===//
				ucTemp = ((uint16_t )* pu8ChoppedStr & 0x07) >> ( 3 - ucBitsPerOffset);
				* pRetUnchoppedStr = ( ucTemp & ucOffsetMask )+ ucBaseLevel;	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				pRetUnchoppedStr++;
				iBitsThisByte -= ( int ) ucBitsPerOffset;
				if( 0 == iBitsThisByte )
				{
					iBitsThisByte = 16;
					pu8ChoppedStr++;
				}
			}
			else
			{
				ucTemp =  ((uint16_t)* pu8ChoppedStr & 0x07) << ( ucBitsPerOffset - 3 );
				pu8ChoppedStr++;
				u8Len--;//decrement length
				iBitsThisByte = 16;
				//=== got what we could from that byte, get the rest from the next one ===//.
				ucTemp |= (uint16_t)* pu8ChoppedStr >> ( 16 - ( ucBitsPerOffset - 3 ));
				ucTemp &= ucOffsetMask;
				* pRetUnchoppedStr =  ( wchar_t )( ucTemp + ucBaseLevel);	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				iBitsThisByte -= ( ucBitsPerOffset - 3 );
				pRetUnchoppedStr++;
			}
			break;
		case 4:
			if( 4 >= ucBitsPerOffset )
			{
				//=== we have enough bits so shift em into position ===//
				ucTemp = ((uint16_t )* pu8ChoppedStr & 0x0f) >> ( 4 - ucBitsPerOffset);
				* pRetUnchoppedStr = ( ucTemp & ucOffsetMask )+ ucBaseLevel;	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				pRetUnchoppedStr++;
				iBitsThisByte -= ( int ) ucBitsPerOffset;
				if( 0 == iBitsThisByte )
				{
					iBitsThisByte = 16;
					pu8ChoppedStr++;
				}
			}
			else
			{
				ucTemp =  (( uint16_t )* pu8ChoppedStr & 0x0f) << ( ucBitsPerOffset - 4 );
				pu8ChoppedStr++;
				u8Len--;//decrement length
				iBitsThisByte = 16;
				//=== got what we could from that byte, get the rest from the next one ===//
				ucTemp |= (( uint16_t )* pu8ChoppedStr) >>  ( 16 - ( ucBitsPerOffset - 4 ) );
				ucTemp &= ucOffsetMask;
				* pRetUnchoppedStr =  (wchar_t )( ucTemp + ucBaseLevel);	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				pRetUnchoppedStr++;
				iBitsThisByte -= ( ucBitsPerOffset - 4 );
			}
			break;
		case 5:
			if( 5 >= ucBitsPerOffset )
			{
				//=== we have enough bits so shift em into position ===//
				ucTemp = ((uint16_t)* pu8ChoppedStr & 0x1f) >> ( 5 - ucBitsPerOffset);
				* pRetUnchoppedStr = ( ucTemp & ucOffsetMask )+ ucBaseLevel;	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				pRetUnchoppedStr++;
				iBitsThisByte -= ( int ) ucBitsPerOffset;
				if( 0 == iBitsThisByte )
				{
					iBitsThisByte = 16;
					pu8ChoppedStr++;
				}
			}
			else
			{
				ucTemp =  (( uint16_t )* pu8ChoppedStr & 0x1f) << ( ucBitsPerOffset - 5 );
				pu8ChoppedStr++;
				u8Len--;//decrement length
				iBitsThisByte = 16;
				//=== got what we could from that byte, get the rest from the next one ===//.
				ucTemp |= ( uint16_t )* pu8ChoppedStr >> ( 16 - ( ucBitsPerOffset - 5 ));
				ucTemp &= ucOffsetMask;
				* pRetUnchoppedStr =  (char )( ucTemp + ucBaseLevel);	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				iBitsThisByte -= ( ucBitsPerOffset - 5 );
				pRetUnchoppedStr++;
			}
			break;
		case 6:
			if( 6 >= ucBitsPerOffset )
			{
				//=== we have enough bits so shift em into position ===//
				ucTemp = ((uint16_t )* pu8ChoppedStr & 0x3f) >> ( 6 - ucBitsPerOffset);
				* pRetUnchoppedStr = ( ucTemp & ucOffsetMask )+ ucBaseLevel;	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				pRetUnchoppedStr++;
				iBitsThisByte -= ( int ) ucBitsPerOffset;
				if( 0 == iBitsThisByte )
				{
					iBitsThisByte = 16;
					pu8ChoppedStr++;
				}
			}
			else
			{
				ucTemp =  ((uint16_t )* pu8ChoppedStr & 0x3f) << ( ucBitsPerOffset - 6 );
				pu8ChoppedStr++;
				u8Len--;//decrement length
				iBitsThisByte = 16;
				//=== got what we could from that byte, get the rest from the next one ===//.
				ucTemp |= (uint16_t )* pu8ChoppedStr >> ( 16 - ( ucBitsPerOffset - 6 ));
				ucTemp &= ucOffsetMask;
				* pRetUnchoppedStr = ucTemp + ucBaseLevel;	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				iBitsThisByte -= ( ucBitsPerOffset - 6 );
				pRetUnchoppedStr++;
			}
			break;
		case 7:
			if( 7 >= ucBitsPerOffset )
			{
				//=== we have enough bits so shift em into position ===//
				ucTemp = ((uint16_t )* pu8ChoppedStr & 0x7f) >> ( 7 - ucBitsPerOffset);
				* pRetUnchoppedStr = ( ucTemp & ucOffsetMask ) + ucBaseLevel;	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				pRetUnchoppedStr++;
				iBitsThisByte -= ( int ) ucBitsPerOffset;
				if( 0 == iBitsThisByte )
				{
					iBitsThisByte = 16;
					pu8ChoppedStr++;
				}
			}
			else
			{
				ucTemp =  ((* pu8ChoppedStr) & 0x007f) << ( ucBitsPerOffset - 7 );
				pu8ChoppedStr++;
				u8Len--;//decrement length
				iBitsThisByte = 16;
				//=== got what we could from that byte, get the rest from the next one ===//.
				ucTemp |= (uint16_t )* pu8ChoppedStr >> ( 16 - ( ucBitsPerOffset - 7 ));
				ucTemp &= ucOffsetMask;
				* pRetUnchoppedStr = ucTemp  + ucBaseLevel;	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				iBitsThisByte -= ( ucBitsPerOffset - 7 );
				pRetUnchoppedStr++;
			}
			break;
		case 8:
			if( 8 >= ucBitsPerOffset )
			{
				//=== we have enough bits so shift em into position ===//
				ucTemp =  ((* pu8ChoppedStr) & 0x00ff) << ( ucBitsPerOffset - 8 );
				* pRetUnchoppedStr = ( ucTemp & ucOffsetMask )+ ucBaseLevel;	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				pRetUnchoppedStr++;
				iBitsThisByte -= ( int ) ucBitsPerOffset;
				if( 0 == iBitsThisByte )
				{
					iBitsThisByte = 16;
					pu8ChoppedStr++;
				}
			}
			else
			{
				ucTemp =  ((* pu8ChoppedStr) & 0x00ff) << ( ucBitsPerOffset - 8 );
				pu8ChoppedStr++;
				u8Len--;//decrement length
				iBitsThisByte = 16;
				//=== got what we could from that byte, get the rest from the next one ===//.
				ucTemp |= (uint16_t )* pu8ChoppedStr >> ucBitsPerOffset;
				ucTemp &= ucOffsetMask;
				* pRetUnchoppedStr = ucTemp + ucBaseLevel;	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				iBitsThisByte -= ( ucBitsPerOffset - 8 );
				pRetUnchoppedStr++;
			}
			break;
		case 9:
			if( 9 >= ucBitsPerOffset )
			{
				//=== we have enough bits so shift em into position ===//
				ucTemp = ((* pu8ChoppedStr) & 0x01ff) >> ( 9 - ucBitsPerOffset);
				* pRetUnchoppedStr = ( ucTemp & ucOffsetMask )+ ucBaseLevel;	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				pRetUnchoppedStr++;
				iBitsThisByte -= ( int ) ucBitsPerOffset;
				if( 0 == iBitsThisByte )
				{
					iBitsThisByte = 16;
					pu8ChoppedStr++;
				}
			}
			else
			{
				ucTemp =  ((* pu8ChoppedStr) & 0x01ff) << ( ucBitsPerOffset - 9 );
				pu8ChoppedStr++;
				u8Len--;//decrement length
				iBitsThisByte = 16;
				//=== got what we could from that byte, get the rest from the next one ===//.
				ucTemp |= (uint16_t )* pu8ChoppedStr >> ucBitsPerOffset;
				ucTemp &= ucOffsetMask;
				* pRetUnchoppedStr = ucTemp + ucBaseLevel;	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				iBitsThisByte -= ( ucBitsPerOffset - 9 );
				pRetUnchoppedStr++;
			}
			break;
		case 10:
			if( 10 >= ucBitsPerOffset )
			{
				//=== we have enough bits so shift em into position ===//
				ucTemp = ((* pu8ChoppedStr) & 0x03ff) >> ( 10 - ucBitsPerOffset);
				* pRetUnchoppedStr = ( ucTemp & ucOffsetMask )+ ucBaseLevel;	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				pRetUnchoppedStr++;
				iBitsThisByte -= ( int ) ucBitsPerOffset;
				if( 0 == iBitsThisByte )
				{
					iBitsThisByte = 16;
					pu8ChoppedStr++;
				}
			}
			else
			{
				ucTemp =  ((* pu8ChoppedStr) & 0x03ff) << ( ucBitsPerOffset - 10 );
				pu8ChoppedStr++;
				u8Len--;//decrement length
				iBitsThisByte = 16;
				//=== got what we could from that byte, get the rest from the next one ===//.
				ucTemp |= (uint16_t )* pu8ChoppedStr >> ucBitsPerOffset;
				ucTemp &= ucOffsetMask;
				* pRetUnchoppedStr = ucTemp + ucBaseLevel;	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				iBitsThisByte -= ( ucBitsPerOffset - 10 );
				pRetUnchoppedStr++;
			}
			break;
		case 11:
			if( 11 >= ucBitsPerOffset )
			{
				//=== we have enough bits so shift em into position ===//
				ucTemp = ((* pu8ChoppedStr) & 0x07ff) >> ( 11 - ucBitsPerOffset);
				* pRetUnchoppedStr = ( ucTemp & ucOffsetMask )+ ucBaseLevel;	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				pRetUnchoppedStr++;
				iBitsThisByte -= ( int ) ucBitsPerOffset;
				if( 0 == iBitsThisByte )
				{
					iBitsThisByte = 16;
					pu8ChoppedStr++;
				}
			}
			else
			{
				ucTemp =  ((* pu8ChoppedStr) & 0x07ff)<< ( ucBitsPerOffset - 11 );
				pu8ChoppedStr++;
				u8Len--;//decrement length
				iBitsThisByte = 16;
				//=== got what we could from that byte, get the rest from the next one ===//.
				ucTemp |= (uint16_t )* pu8ChoppedStr >> ucBitsPerOffset;
				ucTemp &= ucOffsetMask;
				* pRetUnchoppedStr = ucTemp + ucBaseLevel;	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				iBitsThisByte -= ( ucBitsPerOffset - 11 );
				pRetUnchoppedStr++;
			}
			break;
		case 12:
			if( 12 >= ucBitsPerOffset )
			{
				//=== we have enough bits so shift em into position ===//
				ucTemp = ((* pu8ChoppedStr) & 0x0fff) >> ( 12 - ucBitsPerOffset);
				* pRetUnchoppedStr = ( ucTemp & ucOffsetMask )+ ucBaseLevel;	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				pRetUnchoppedStr++;
				iBitsThisByte -= ( int ) ucBitsPerOffset;
				if( 0 == iBitsThisByte )
				{
					iBitsThisByte = 16;
					pu8ChoppedStr++;
				}
			}
			else
			{
				ucTemp =  ((* pu8ChoppedStr) & 0x0fff) << ( ucBitsPerOffset - 12 );
				pu8ChoppedStr++;
				u8Len--;//decrement length
				iBitsThisByte = 16;
				//=== got what we could from that byte, get the rest from the next one ===//.
				ucTemp |= (uint16_t )* pu8ChoppedStr >> ucBitsPerOffset;
				ucTemp &= ucOffsetMask;
				* pRetUnchoppedStr = ucTemp + ucBaseLevel;	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				iBitsThisByte -= ( ucBitsPerOffset - 12 );
				pRetUnchoppedStr++;
			}
			break;
		case 13:
			if( 13 >= ucBitsPerOffset )
			{
				//=== we have enough bits so shift em into position ===//
				ucTemp = ((* pu8ChoppedStr) & 0x1fff) >> ( 13 - ucBitsPerOffset);
				* pRetUnchoppedStr = ( ucTemp & ucOffsetMask )+ ucBaseLevel;	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				pRetUnchoppedStr++;
				iBitsThisByte -= ( int ) ucBitsPerOffset;
				if( 0 == iBitsThisByte )
				{
					iBitsThisByte = 16;
					pu8ChoppedStr++;
				}
			}
			else
			{
				ucTemp =  ((* pu8ChoppedStr) & 0x1fff) << ( ucBitsPerOffset - 13 );
				pu8ChoppedStr++;
				u8Len--;//decrement length
				iBitsThisByte = 16;
				//=== got what we could from that byte, get the rest from the next one ===//.
				ucTemp |= (uint16_t )* pu8ChoppedStr >> ucBitsPerOffset;
				ucTemp &= ucOffsetMask;
				* pRetUnchoppedStr = ucTemp + ucBaseLevel;	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				iBitsThisByte -= ( ucBitsPerOffset - 13 );
				pRetUnchoppedStr++;
			}
			break;
		case 14:
			if( 14 >= ucBitsPerOffset )
			{
				//=== we have enough bits so shift em into position ===//
				ucTemp = ((* pu8ChoppedStr) & 0x3fff) >> ( 14 - ucBitsPerOffset);
				* pRetUnchoppedStr = ( ucTemp & ucOffsetMask )+ ucBaseLevel;	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				pRetUnchoppedStr++;
				iBitsThisByte -= ( int ) ucBitsPerOffset;
				if( 0 == iBitsThisByte )
				{
					iBitsThisByte = 16;
					pu8ChoppedStr++;
				}
			}
			else
			{
				ucTemp =  ((* pu8ChoppedStr) & 0x3fff) << ( ucBitsPerOffset - 14 );
				pu8ChoppedStr++;
				u8Len--;//decrement length
				iBitsThisByte = 16;
				//=== got what we could from that byte, get the rest from the next one ===//.
				ucTemp |= (uint16_t )* pu8ChoppedStr >> ucBitsPerOffset;
				ucTemp &= ucOffsetMask;
				* pRetUnchoppedStr = ucTemp + ucBaseLevel;	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				iBitsThisByte -= ( ucBitsPerOffset - 14 );
				pRetUnchoppedStr++;
			}
			break;
		case 15:
			if( 15 >= ucBitsPerOffset )
			{
				//=== we have enough bits so shift em into position ===//
				ucTemp = ((* pu8ChoppedStr) & 0x7fff) >> ( 15 - ucBitsPerOffset);
				* pRetUnchoppedStr = ( ucTemp & ucOffsetMask )+ ucBaseLevel;	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				pRetUnchoppedStr++;
				iBitsThisByte -= ( int ) ucBitsPerOffset;
				if( 0 == iBitsThisByte )
				{
					iBitsThisByte = 16;
					pu8ChoppedStr++;
				}
			}
			else
			{
				ucTemp =  ((* pu8ChoppedStr) & 0x7fff) << ( ucBitsPerOffset - 15 );
				pu8ChoppedStr++;
				u8Len--;//decrement length
				iBitsThisByte = 16;
				//=== got what we could from that byte, get the rest from the next one ===//.
				ucTemp |= (uint16_t )* pu8ChoppedStr >> ucBitsPerOffset;
				ucTemp &= ucOffsetMask;
				* pRetUnchoppedStr = ucTemp + ucBaseLevel;	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				iBitsThisByte -= ( ucBitsPerOffset - 15 );
				pRetUnchoppedStr++;
			}
			break;
		case 16:
			if( 16 >= ucBitsPerOffset )
			{
				//=== we have enough bits so shift em into position ===//
				ucTemp = (uint16_t )* pu8ChoppedStr >> ( 16 - ucBitsPerOffset);
				* pRetUnchoppedStr = ( ucTemp & ucOffsetMask )+ ucBaseLevel;	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				pRetUnchoppedStr++;
				iBitsThisByte -= ( int ) ucBitsPerOffset;
				if( 0 == iBitsThisByte )
				{
					iBitsThisByte = 16;
					pu8ChoppedStr++;
				}
			}
			else
			{
				ucTemp =  (uint16_t )* pu8ChoppedStr << ( ucBitsPerOffset - 16 );
				pu8ChoppedStr++;
				u8Len--;//decrement length
				iBitsThisByte = 16;
				//=== got what we could from that byte, get the rest from the next one ===//.
				ucTemp |= (uint16_t )* pu8ChoppedStr >> ucBitsPerOffset;
				ucTemp &= ucOffsetMask;
				* pRetUnchoppedStr = ucTemp + ucBaseLevel;	// restore to original value
				if( ucBaseLevel == * pRetUnchoppedStr )
				{
					goto Done;
				}
				iBitsThisByte -= ( ucBitsPerOffset - 16 );
				pRetUnchoppedStr++;
			}
			break;
		}
	}
Done:
	*pRetUnchoppedStr = 0;

#ifdef USE_MAPPING
	VxUnMapStr( pSavedRetUnchoppedStrPointer );
#endif
}

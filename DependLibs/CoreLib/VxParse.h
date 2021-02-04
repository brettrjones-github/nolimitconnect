#pragma once
//============================================================================
// Copyright (C) 2013 Brett R. Jones
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
// http://www.nolimitconnect.com
//============================================================================

#include <CoreLib/config_corelib.h>

#include <string>
#include <vector>

//! replace \ with / and % with ?
int								MakeStringSafeForSprintf( char * pStr, int iLen );

bool							WildMatchString( std::string & cs, std::string& csWildStr );

bool							GetLine( std::string cs, unsigned int n, std::string &r );
bool							IsHexDecString( std::string& cs );
int								CountChars( const char * pStr, char cCharToLookFor );
int								CountChars( std::string& csStr, char cCharToLookFor );

bool							RemoveNonAlphaNums( std::string& cs );
bool							RemoveDoubleSpaces( std::string &cs );
std::string						GetSubstring( std::string& csStr, int iStartIdx, int iEndIdx );
bool							GetWord( std::string& csStr, int n, std::string& csRetToken );
//! Returns a pointer to the first occurrence of strSearch in strMain, or NULL if strSearch does not appear in str
const char *					StrStrNoCase( const char * strMain, const char * strSearch );
//! find position of string p2 in p1.. return -1 if not found
int								StrStrNoCasePosition( const char * strMain, const char * strSearch );
//! format into std::string using printf style format
void							StdStringFormat(	std::string & csStr,	// std::string to return formated results in
													const char * pFormat,	// string with format chars etc		
													... );					// vars
//! format into std::string using printf style format
void							StdStringFormat(	std::wstring & csStr,	// std::string to return formated results in
													const wchar_t * pFormat,	// string with format chars etc		
													... );					// vars
//!	find position of the given char.. if not found return -1			
int								StdStringFind( std::string & csStr, char cFindChar );
//!	find position of the given char..case insensitive if not found return -1			
int								StdStringFindCaseInsensitive( std::string & csStr, char cFindChar );
//!	find position of the given string if not found return -1			
int								StdStringFind( std::string & csStr, const char * pSubStr );
//!	find position of the given string case insensitive if not found return -1			
int								StdStringFindCaseInsensitive( std::string & csStr, const char * pSubStr );
//!	find position of the given char by searching from end to first if not found return -1			
int								StdStringReverseFind( std::string & csStr, char cFindChar );
//! make standard string upper case
void							StdStringMakeUpper( std::string & csStr );
//! make standard string lower case
void							StdStringMakeLower( std::string & csStr );
std::string						StdStringRight( std::string& cs, unsigned int n );
std::string						StdStringLeft( std::string& cs, unsigned int n );
std::string						StdStringRightOf( std::string& cs, char chr );
std::string						StdStringRightOfLast( std::string cs, char chr );
std::string						StdStringLeftOf( std::string cs, char chr );
std::string						StdStringLeftOfLast( std::string& cs, char chr);

//bool			StoreStdString( FILE * pgFile, std::string &cs );
//bool			LoadStdString( FILE * pgFile, std::string &cs );
//! copy string into buffer.. if string is to long then truncate
//void							SafeStringCopy(		std::string		&strSrc,		// string to copy from
//													char *			pBuf,			// buffer to copy into
//													unsigned int	uiMaxBufLen );	// max length of buffer	
////! copy string into buffer.. if string is to long then truncate
//void							SafeStringCopy(		const char *	pSrcString,		// string to copy from
//													char *			pBuf,			// buffer to copy into
//													unsigned int	uiMaxBufLen );	// max length of buffer	
//! Safe string copy.. logs error if buffer to short
int								SafeStrCopy( char * pDest, const char * pSrc, int iBufLen );
int								SafeStrCopy( wchar_t * pDest, const wchar_t * pSrc, int iBufLen );

//!	trim all at first instance of char		
void							StdStringTrim( std::string & csStr, const char cTrim );
//!	trim all at first instance of char		
void							StdStringTrim( std::wstring & csStr, const wchar_t cTrim );

//!	trim leading and trailing whitespace		
void							StdStringTrim( std::string & csStr );
//!	trim leading and trailing whitespace		
void							StdStringTrim( std::wstring & csStr );

bool							isWhiteSpace( char cChar );
bool							isWhiteSpace( wchar_t wChar );

//!	split string into tokens		
void							StdStringSplit( const std::string & csStr, const char cToken, std::vector<std::string>& aoRetTokens );
//!	split wide string into tokens		
void							StdStringSplit( const std::wstring & csStr, const wchar_t cToken, std::vector<std::wstring>& aoRetTokens );

// return true if prefix matched beginning characters of string
bool							StdStringBeginsWith( std::string & csStr, const char * pPrefix );
// return true if suffix matched end characters of string
bool							StdStringEndsWith( std::string & csStr, const char * pSuffix );
// fill string with integer value
const char *					StdStringFromInteger( std::string & strResult, int iValue );
int								StdStringToInteger( std::string& strValue );

//============================================================================
//!	return string at first instance of char		
const wchar_t *					wstrchr( const wchar_t * wStr, const wchar_t wChar );
//! same as strstr but case insensitive
char *							stristr( const char * pString, const char * pPattern );

int								wstrlen( const wchar_t * pString );
wchar_t *						wstrstr( const wchar_t * pString, const wchar_t * pPattern );
//! same as strstr but case insensitive
wchar_t *						wstristr( const wchar_t * pString, const wchar_t * pPattern );
void							wstrcpy( wchar_t * pwDest, const wchar_t * pwSrc );
void							wstrcat( wchar_t * pwDest, const wchar_t * pwSrc );
wchar_t *						wstrchr( wchar_t * pwStr, wchar_t tok );
wchar_t *						wstrrchr( wchar_t * pwStr, wchar_t tok );
int								wstrcmp( wchar_t * pwStr1, const wchar_t * pwStr2 );
void							wstrncpy( wchar_t * pwDest, const wchar_t * pwSrc, int maxDestLen );
wchar_t *						wstrpbrk( wchar_t * pwSrc, const wchar_t * pwTokens );
wchar_t *						wstrsep( wchar_t **ppStr, const wchar_t *pDelimiters );
#ifdef TARGET_OS_WINDOWS
// windows doesn't have strsep so do our own version
char *							strsep( char **ppStr, const char *pDelimiters );
#endif //TARGET_OS_WINDOWS

std::wstring					Utf8ToWide( const std::string utf8string );
std::string						WideToUtf8( const std::wstring utf16String );
std::wstring					AsciiToWide( const std::string& utf8string );
std::string						WideToAscii( const std::wstring& utf16String );

//! return pointer to first char that is not whitespace
char *							TrimLeadingSpaces( char * pString );
wchar_t *						TrimLeadingSpaces( wchar_t * pString );

// find substring (case insensitive) second param str2 is the substring to search str1 for
// returns -1 if not found
int                             CaseInsensitiveFindSubstr( const std::string& str1, const std::string& str2 );

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
// http://www.nolimitconnect.org
//============================================================================
//#include "config_corelib.h"
#include "VxParse.h"
#include "VxDefs.h"
#include "VxDebug.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#ifndef TARGET_OS_WINDOWS
    #include <ctype.h> // for toupper and tolower
    #include <wctype.h> // for towupper and towlower
#else
# include "windows.h"
#endif

#include <locale>
#include <iostream>
#include <algorithm>
using namespace std;

// templated version of case insensitive equal so it could work with both char and wchar_t
template<typename charT>
struct ci_equal {
    ci_equal( const std::locale& loc ) : loc_(loc) {}
    bool operator()(charT ch1, charT ch2) {
        return std::toupper(ch1, loc_) == std::toupper(ch2, loc_);
    }
private:
    const std::locale& loc_;
};

// find substring (case insensitive) second param str2 is the substring to search str1 for
template<typename T>
int ci_find_substr( const T& str1, const T& str2, const std::locale& loc = std::locale() )
{
    typename T::const_iterator it = std::search( str1.begin(), str1.end(), 
        str2.begin(), str2.end(), ci_equal<typename T::value_type>(loc) );
    if ( it != str1.end() ) return it - str1.begin();
    else return -1; // not found
}

//============================================================================
int CaseInsensitiveFindSubstr( const std::string& str1, const std::string& str2 )
{
    return ci_find_substr( str1, str2 );
}

//============================================================================
//! replace \ with / and % with ?
int MakeStringSafeForSprintf( char * pStr, int iLen )
{
	int iCount = 0;
	for( int i = 0; i < iLen; i++ )
	{
		if( '\\' == pStr[i] )
		{
			pStr[i] = '/';
			iCount++;
		}
		if( '%' == pStr[i] )
		{
			pStr[i] = '?';
			iCount++;
		}
	}
	return iCount;
}

//============================================================================
bool WildMatchString( std::string & cs, std::string& w)
{
	std::string s = cs;
	std::string t;
	int pos;

	for (;;)
	{
		if( StdStringLeft( w, (uint32_t)1 ) == std::string("?"))
		{
			w = StdStringRight( w, (uint32_t)w.size() - 1);
			if (s.size() >= 1)
				s = StdStringRight( s, (uint32_t)s.size() - 1);
			else
				return(false);
		}
		else if( StdStringLeft(w, 1) == std::string("*"))
		{
			w = StdStringRight( w, (uint32_t)w.size() - 1);
			if( !w.size() )
				return(true);
			t = StdStringLeftOf( w, '*');
			t = StdStringLeftOf( t, '?' );
			pos = StrStrNoCasePosition( s.c_str(), t.c_str() );
			if (pos == -1)
				return(false);
			s = StdStringRight( s, (uint32_t)s.size() - pos - t.size());
			w = StdStringRight( w, (uint32_t)w.size() - t.size());
		}
		else
		{
			t = StdStringLeftOf( w, '*' );
			t = StdStringLeftOf( t, '?' );
			if( StdStringLeft( s, (uint32_t)t.size() ) != t)
				return(false);
			s = StdStringRight( s, (uint32_t)s.size() - t.size());
			w = StdStringRight( w, (uint32_t)w.size() - t.size());
		}

		if (!s.size() || !w.size())
			return(true);
	}
}

//============================================================================
bool GetLine( std::string cs, unsigned int n, std::string &r )
{
	size_t i;
	size_t iLen = cs.size();
    int start=-1;
	size_t end=0;

	r = "";
	if (n == 0)
		start = 0;
	for( i = 0; i < iLen; i++ )
	{
		if( '\r' == ((const char *)cs.c_str())[i] )
		{
			if (n == 0)
			{
				end = i;
				break;
			}
			else
			{
				n--;
				if( '\n' == ((const char *)cs.c_str())[ i + 1 ] )
					i++;
				if (n == 0)
					start = i+1;
			}
		}
	}
	if( i == iLen )
	{
		if( -1 == start )
			return false;
		end = i;
	}
	r = GetSubstring( cs, (uint32_t)start, (uint32_t)end);

	return r.size() ? true : false;
}

//============================================================================
bool IsHexDecString( std::string& cs )
{
	const char * pData = (const char *)cs.c_str();
	for( int i = 0; i < (int)cs.size(); i++ )
	{
		if( pData[ i ] < '0' )
			return(false);
		if( pData[ i ] > 'f')
			return(false);
		if( pData[ i ] > '9' && pData[ i ] < 'A')
			return(false);
		if( pData[ i ] > 'F' && pData[ i ] < 'a')
			return(false);
	}
	return(true);
}

//============================================================================
int CountChars( const char * pStr, char cCharToLookFor )
{
	int iCnt = 0;
	int iLen = (int)strlen( pStr );

	for( int i = 0; i < iLen; i++ )
	{
		if( cCharToLookFor == pStr[i] )
		{
			iCnt++;
		}
	}
	return iCnt;
}
//============================================================================
int CountChars( std::string& csStr, char cCharToLookFor )
{
	int iCnt = 0;
	const char * pStr = (const char *)csStr.c_str();
	int iLen = (int)strlen( pStr );

	for( int i = 0; i < iLen; i++ )
	{
		if( cCharToLookFor == pStr[i] )
		{
			iCnt++;
		}
	}
	return iCnt;
}

//============================================================================
bool RemoveNonAlphaNums( std::string& cs )
{
	int iLen = (uint32_t)cs.size();

	for ( int i=0; i < iLen; i++ )
	{
		if( !isalnum( ((const char *)cs.c_str())[ i ] ) && ( '.' != ((const char *)cs.c_str())[ i ] ) )
			((char *)(const char *)cs.c_str())[ i ] = ' ';
	}
	return true;
}

//============================================================================
bool RemoveDoubleSpaces( std::string &cs )
{
	int newLength = (int)cs.size();
	int t=0;
	bool spaceAllowed = false;
	int iLen = (int)cs.size();

	for( int i = 0; i < iLen; i++ )
	{
		if( ((const char *)cs.c_str())[ i ] == ' ' )
		{
			if (!spaceAllowed)
			{
				t++;
				newLength--;
			}
			else
				spaceAllowed = false;
		}
		else
			spaceAllowed = true;
		((char *)(const char *)cs.c_str())[ i - t + 1 ] = ((const char *)cs.c_str())[ i + 1 ];
	}
	if( ((const char *)cs.c_str())[ newLength - 1 ] == ' ' )
		newLength--;
	iLen = newLength;
	((char *)(const char *)cs.c_str())[ iLen ] = '\0';

	return(true);
}

//============================================================================
std::string GetSubstring( std::string& csStr, int iStartIdx, int iEndIdx )
{
	std::string cs;

	char *tempstr = new char[ iEndIdx - iStartIdx + 1 ];
	memcpy(tempstr, (const char *)csStr.c_str() + iStartIdx, iEndIdx - iStartIdx );
	tempstr[ iEndIdx - iStartIdx ] = 0;
	cs = tempstr;
	delete [] tempstr;
	return cs;
}

//============================================================================
bool GetWord( std::string& csStr, int n, std::string& csRetToken )
{
	int iLength = (int)csStr.size();
	int i;
	int start=-1, end=0;

	csRetToken = "";
	if (n == 0)
		start = 0;
	for( i=0; i < iLength; i++ )
	{
		char c = (csStr.c_str())[i];
		if( ' ' == c )
		{
			if (n == 0)
			{
				end = i;
				break;
			}
			else
			{
				n--;
				if (n == 0)
					start = i+1;
			}
		}
	}
	if( i == iLength )
	{
		if (start == -1)
			return(false);
		end = i;
	}
	csRetToken = GetSubstring( csStr, start, end);

	return csRetToken.size() ? true : false;

}

//============================================================================
//! format into std::string using printf style format
void StdStringFormat(	std::string & csStr,	// std::string to return formated results in
						const char * pFormat,	// string with format chars etc		
						... )					// vars
{
	char szBuffer[4096];

    va_list argList;
    va_start(argList, pFormat);
	vsnprintf( szBuffer, sizeof( szBuffer ), pFormat, argList );
    va_end(argList);

    csStr = szBuffer;
}

//============================================================================
//! format into std::string using printf style format
void StdStringFormat(	std::wstring & csStr,	// std::string to return formated results in
						const wchar_t * pFormat,	// string with format chars etc		
						... )					// vars
{
	std::wstring strWFormat = pFormat;
	std::string strAFormat = WideToAscii( strWFormat );
	char szBuffer[4096];

	va_list argList;
	va_start(argList, pFormat);
	vsnprintf( szBuffer, sizeof( szBuffer ), strAFormat.c_str(), argList );
	va_end(argList);

	std::string strAResult = szBuffer;

	csStr = AsciiToWide( strAResult );
}

//============================================================================
//!	find position of the given char.. if not found return -1			
int	StdStringFind( std::string & csStr, char cFindChar )
{
	char * pTemp = (char *)csStr.c_str();
	int iLen = (int)csStr.size();
	for( int i = 0; i < iLen; i++ )
	{
		if( pTemp[i] == cFindChar )
		{
			return i;
		}
	}

	return -1;
}
//============================================================================
//!	find position of the given char case insensitive.. if not found return -1			
int StdStringFindCaseInsensitive( std::string & csStr, char cFindChar )
{
	cFindChar = tolower( cFindChar );
	char * pTemp = (char *)csStr.c_str();
	int iLen = (int)csStr.size();
	for( int i = 0; i < iLen; i++ )
	{
		if( tolower( pTemp[i] ) == cFindChar )
		{
			return i;
		}
	}

	return -1;
}

//============================================================================
//!	find position of the given string if not found return -1			
int StdStringFind( std::string & csStr, const char * pSubStr )
{
	if(!csStr.size())
	{
		return(false);
	}

	std::string cs1 = csStr;
	std::string cs2 = pSubStr;
	if( const char * pStr = strstr( (const char * ) cs1.c_str(), (const char *)cs2.c_str() ) )
	{
		return int(pStr-(const char * )cs1.c_str());
	}

	return -1;
}
//============================================================================
//!	find position of the given string if not found return -1			
int StdStringFindCaseInsensitive( std::string & csStr, const char * pSubStrIn )
{
	int iSubStrLen = (int)strlen( pSubStrIn );
	int iStrLen = (int)csStr.size();
	if( 0 == iSubStrLen )
	{	
		return -1;
	}

	if( 0 == iStrLen )
	{
		return -1;
	}

	char * pMainStr = new char[ iStrLen + 1];
	strcpy( pMainStr, csStr.c_str() );
	char * pSubStr = new char[ iStrLen + 1];
	strcpy( pSubStr, pSubStrIn );
	char * pFound = strstr( pMainStr, pSubStr );
	int iRetPos = -1;
	if( pFound )
	{
		iRetPos = (int)(pSubStr - pMainStr);
	}

	delete[] pMainStr;
	delete[] pSubStr;
	return iRetPos;
} 

//============================================================================
//!	find position of the given char by searching from end to first if not found return -1			
int StdStringReverseFind( std::string& csStr, char cFindChar )
{
	int iLen = (int)csStr.length();
	if( iLen )
	{
		char * pTemp = (char *)csStr.c_str();
		for( int i = iLen-1; i >= 0; i-- )
		{
			if( pTemp[i] == cFindChar )
			{
				return i;
			}
		}
	}

	return -1;
}

//============================================================================
//! make standard string upper case
void StdStringMakeUpper( std::string& csStr )
{
	int iLen = (int)csStr.size();
	if( iLen )
	{
		char * pTemp = (char *)csStr.c_str();
		for( int i = 0; i < iLen; i++ )
		{
			pTemp[i] = toupper( pTemp[i] );
		}
	}
}

//============================================================================
//! make standard string lower case
void StdStringMakeLower( std::string & csStr )
{
	int iLen = (int)csStr.size();
	if( iLen )
	{
		char * pTemp = (char *)csStr.c_str();
		for( int i = 0; i < iLen; i++ )
		{
			pTemp[i] = tolower( pTemp[i] );
		}
	}
}

//============================================================================
//! Returns a pointer to the first occurrence of strSearch in strMain, or NULL if strSearch does not appear in str
const char * StrStrNoCase( const char * strMain, const char * strSearch )
{
	if(!strlen( strMain))
		return(NULL);
	std::string cs1 = strMain;
	std::string cs2 = strSearch;
	StdStringMakeLower( cs1 );
	StdStringMakeLower( cs2 );
	if( const char * pStr = strstr( (const char * ) cs1.c_str(), (const char *)cs2.c_str() ) )
	{
		return strMain + (int)(pStr-(const char * )cs1.c_str());
	}

	return NULL;
}

//============================================================================
//! find position of string p2 in p1.. return -1 if not found
int StrStrNoCasePosition( const char * p1, const char * p2 )
{
	if(!strlen( p1))
		return(false);
	std::string cs1 = p1;
	std::string cs2 = p2;
	StdStringMakeLower(cs1);
	StdStringMakeLower(cs2);
	if( const char * pStr = strstr( (const char * ) cs1.c_str(), (const char *)cs2.c_str() ) )
	{
		return int(pStr-(const char * )cs1.c_str());
	}

	return -1;
}

//============================================================================
std::string StdStringRight( std::string& cs, unsigned int n )
{
	std::string r;
	int iLen = (int)cs.size();
	if( (int)n > iLen )
	{
		return r;
	}
	unsigned int pos = iLen - n;
	r = (const char *)cs.c_str() + pos;
	return(r);
}

//============================================================================
std::string StdStringLeft( std::string& cs, unsigned int n )
{
	std::string r;
	int iLen = (int)cs.size();
	if( (int)n > iLen )
	{
		return r;
	}
	char *copybuf = new char[ n + 1 ];
	memcpy( copybuf, (const char *)cs.c_str(), n );
	copybuf[ n ] = '\0';
	r = copybuf;
	delete [] copybuf;
	return r;
}

//============================================================================
std::string StdStringRightOf( std::string& cs, char chr )
{
	int newlength;
	std::string r;
	int iLen =(int) cs.size();

	if( -1 == ( newlength = StdStringFind( cs, chr ) ) )
	{
		r = "";
	}
	else
	{
		newlength = iLen - newlength - 1;
		char *temp = new char[ newlength + 1 ];
		strcpy(temp, (const char *)cs.c_str() + iLen - newlength );
		r = temp;
		delete [] temp;
	}

	return r;
}

//============================================================================
std::string StdStringRightOfLast( std::string cs, char chr )
{
	int newlength;
	int iLen = (int)cs.size();

	if( -1 == ( newlength = StdStringReverseFind(cs, chr ) ) )
	{
		return std::string("");
	}
	newlength = iLen - newlength - 1;
	
	char *temp = new char[newlength+1];
	strcpy(temp, (const char *)cs.c_str() + iLen - newlength);
	std::string s(temp);
	delete[] temp;
	return s;
}

//============================================================================
std::string StdStringLeftOf( std::string cs, char chr )
{
	int newlength;
	if( -1 == (newlength = StdStringFind( cs, chr ) ) )
	{
		return cs;
	}
	char *temp = new char[ newlength + 1 ];
	memcpy(temp, (const char *)cs.c_str(), newlength);
	temp[ newlength ] = '\0';
	std::string s(temp);
	delete[] temp;
	return s;
}

//============================================================================
std::string StdStringLeftOfLast( std::string& cs, char chr)
{
	int newlength;
	if( -1 == ( newlength = StdStringReverseFind( cs, chr ) ) )
	{
		return std::string("");
	}
	char *temp = new char[ newlength + 1 ];
	memcpy( temp, (const char *)cs.c_str(), newlength);
	temp[ newlength ] = '\0';
	std::string s( temp );
	delete[] temp;
	return s ;
}

//============================================================================
//!	trim all at first instance of char		
void StdStringTrim( std::string & csStr, const char cTrim )
{
	const char * pTemp = strchr( csStr.c_str(), cTrim );
	if( pTemp )
	{
		*((char *)pTemp) = 0;
	}
}
//============================================================================
//!	return string at first instance of char		
const wchar_t * wstrchr(  const wchar_t * wStr, const wchar_t wChar )
{
	int len = wstrlen( wStr );
	for( int i = 0; i < len; i++ )
	{
		if(wChar == wStr[i])
		{
			return &wStr[i];
		}
	}
	return NULL;
}

//============================================================================
//!	trim all at first instance of char		
void StdStringTrim( std::wstring & csStr, const wchar_t cTrim )
{
	const wchar_t * pTemp = wstrchr( csStr.c_str(), cTrim );
	if( pTemp )
	{
		*((wchar_t *)pTemp) = 0;
	}
}

//============================================================================
bool isWhiteSpace( char cChar )
{
	return (	( ' ' == cChar ) ||
				( '\n' == cChar ) ||
				( '\t' == cChar ) ||
				( '\r' == cChar ) );
}

//============================================================================
bool isWhiteSpace( wchar_t wChar )
{
	return (	( ' ' == wChar ) ||
		( '\n' == wChar ) ||
		( '\t' == wChar ) ||
		( '\r' == wChar ) );
}

//============================================================================
//!	trim leading and trailing whitespace		
void StdStringTrim( std::string & csStr )
{
	if( csStr.length() )
	{
		char * pStrBuf = new char[ csStr.length() + 2 ];
		strcpy( pStrBuf, csStr.c_str() );
		int iNotWhiteSpaceLeadingIdx = 0;
		while( isWhiteSpace(pStrBuf[ iNotWhiteSpaceLeadingIdx ]))
		{
			iNotWhiteSpaceLeadingIdx++;
		}
		if( iNotWhiteSpaceLeadingIdx )
		{
			strcpy( pStrBuf, &pStrBuf[iNotWhiteSpaceLeadingIdx] );
		}
		int iTrailIdx = strlen( pStrBuf ) - 1;
		bool hadTrailingWhiteSpace = false;
		if( 0 < iTrailIdx )
		{
			while( isWhiteSpace(pStrBuf[ iTrailIdx ]) && (iTrailIdx > 0) )
			{
				hadTrailingWhiteSpace = true;
				iTrailIdx--;
			}
		}

		if( hadTrailingWhiteSpace )
		{
			pStrBuf[iTrailIdx + 1] = 0;
		}

		csStr = pStrBuf;
		delete[] pStrBuf;
	}
}

//============================================================================
//!	trim leading and trailing whitespace		
void StdStringTrim( std::wstring & csStr )
{
	if( csStr.length() )
	{
		wchar_t * pStrBuf = new wchar_t[ csStr.length() + 2 ];
		wstrcpy( pStrBuf, csStr.c_str() );
		int iNotWhiteSpaceLeadingIdx = 0;
		while( isWhiteSpace(pStrBuf[ iNotWhiteSpaceLeadingIdx ]))
		{
			iNotWhiteSpaceLeadingIdx++;
		}
		if( iNotWhiteSpaceLeadingIdx )
		{
			wstrcpy( pStrBuf, &pStrBuf[iNotWhiteSpaceLeadingIdx] );
		}
		int iTrailIdx = wstrlen( pStrBuf ) - 1;
		bool hadTrailingWhiteSpace = false;
		if( 0 < iTrailIdx )
		{
			while( isWhiteSpace(pStrBuf[ iTrailIdx ]) && (iTrailIdx > 0) )
			{
				hadTrailingWhiteSpace = true;
				iTrailIdx--;
			}
		}

		if( hadTrailingWhiteSpace )
		{
			pStrBuf[iTrailIdx + 1] = 0;
		}

		csStr = pStrBuf;
		delete[] pStrBuf;
	}
}

//============================================================================
//!	split string into tokens		
void StdStringSplit( const std::string & csStr, const char cToken, std::vector<std::string>& aoRetTokens )
{
	if( csStr.length() )
	{
		char * pStrBuf = new char[csStr.length() + 2];
		strcpy( pStrBuf, csStr.c_str() );
		char * pBufToDelete = pStrBuf;
		char * pTemp = pStrBuf;
		while( 0 != *pTemp )
		{
			if( cToken == *pTemp )
			{
				*pTemp = 0;
				std::string strTok = pStrBuf;
				aoRetTokens.push_back( strTok );
				pStrBuf = pTemp + 1;			
			}
			pTemp++;
		}
		if( 0 != *pStrBuf )
		{
			std::string strTok = pStrBuf;
			aoRetTokens.push_back( strTok );
		}
		delete pBufToDelete;
	}
}

//============================================================================
//!	split wide string into tokens		
void StdStringSplit( const std::wstring & csStr, const wchar_t cToken, std::vector<std::wstring>& aoRetTokens )
{
	if( csStr.length() )
	{
		wchar_t * pStrBuf = new wchar_t[csStr.length() + 2];
		wstrcpy( pStrBuf, csStr.c_str() );
		wchar_t * pBufToDelete = pStrBuf;
		wchar_t * pTemp = pStrBuf;
		while( 0 != *pTemp )
		{
			if( cToken == *pTemp )
			{
				*pTemp = 0;
				std::wstring strTok = pStrBuf;
				aoRetTokens.push_back( strTok );
				pStrBuf = pTemp + 1;			
			}
			pTemp++;
		}
		if( 0 != *pStrBuf )
		{
			std::wstring strTok = pStrBuf;
			aoRetTokens.push_back( strTok );
		}
		delete pBufToDelete;
	}
}

//============================================================================
//! same as strstr but case insensitive
char * stristr( const char * pString, const char * pPattern )
{
	if( !pString || !pPattern )
		return 0;
	int slen = ( int )strlen( pString );
	int plen = ( int )strlen( pPattern );
	if( (0 == slen ) && (0 == plen ) )
		return (char *)pString; // both empty strings
	
	char *pptr, *sptr, *start;
	for( start = (char *)pString, pptr = (char *)pPattern; slen >= plen; start++, slen--)
	{
		// find start of pattern in string 
		while( toupper(*start) != toupper(*pPattern) )
		{
			start++;
			slen--;
			// if pattern longer than string then cannot be matched
			if (slen < plen)
				return(NULL);
		}

		sptr = start;
		pptr = (char *)pPattern;
		while (toupper(*sptr) == toupper(*pptr))
		{
			sptr++;
			pptr++;
			if ('\0' == *pptr)
				return (start);
		}
	}

	return(NULL);
}

//============================================================================
void wstrcpy( wchar_t * pwDest, const wchar_t * pwSrc )
{
	while( 0 != *pwSrc )
	{
		*pwDest = *pwSrc;
		pwDest++;
		pwSrc++;
	}
	*pwDest = 0;
}

//============================================================================
void wstrcat( wchar_t * pwDest, const wchar_t * pwSrc )
{
	while( 0 != *pwDest )
	{
		pwDest++;
	}

	wstrcpy( pwDest, pwSrc );
}

//============================================================================
wchar_t * wstrchr( wchar_t * pwStr, wchar_t tok )
{
	if( pwStr )
	{
		while( 0 != *pwStr )
		{
			if( tok == *pwStr )
			{
				return pwStr;
			}
			
			pwStr++;
		}
	}

	return 0;
}

//============================================================================
wchar_t * wstrrchr( wchar_t * pwStr, wchar_t tok )
{
	wchar_t * pwTemp = pwStr;
	if( pwStr && ( 0 != *pwStr ) )
	{
		while( 0 != *pwTemp )
		{
			pwTemp++;
		}

		while( pwTemp != pwStr )
		{
			if( tok == *pwTemp )
			{
				return pwTemp;
			}

			pwTemp--;
		}
	}

	return 0;
}

//============================================================================
int wstrcmp( wchar_t * pwStr1, const wchar_t * pwStr2 )
{
	while( ( 0 != *pwStr1 )
		&& ( 0 != *pwStr2 ))
	{
		if( *pwStr1 != *pwStr2 )
		{
			if( *pwStr1 > *pwStr2 )
			{
				return 1;
			}
			else
			{
				return -1;
			}
		}

		pwStr1++;
		pwStr2++;
	}

	if( *pwStr1 == *pwStr2 )
	{
		return 0;
	}

	if( 0 == *pwStr2 )
	{
		return 1;
	}

	return -1;
}

//============================================================================
void wstrncpy( wchar_t * pwDest, const wchar_t * pwSrc, int maxDestLen )
{
	int charCnt = 0;
	while( ( 0 != *pwSrc )
		&& ( charCnt < maxDestLen ))
	{
		*pwDest = *pwSrc;
		pwDest++;
		pwSrc++;
		charCnt++;
	}

	if( charCnt < maxDestLen )
	{
		*pwDest = 0;
	}
}

//============================================================================
wchar_t * wstrpbrk( wchar_t * pwSrc, const wchar_t * pwTokens )
{
	while( 0 != *pwSrc )
	{
		const wchar_t * pTemp = pwTokens;
		while( 0 != *pTemp )
		{
			if( *pTemp == *pwSrc )
			{
				return pwSrc;
			}

			pTemp++;
		}

		pwSrc++;
	}

	return 0;
}

//============================================================================
int wstrlen( const wchar_t * pString )
{
	int iStrLen = 0;
	while( 0 != *pString++ )
	{
		iStrLen++;
	}

	return iStrLen;
}

//============================================================================
wchar_t * wstrstr( const wchar_t * pString, const wchar_t * pPattern )
{
	wchar_t *pptr, *sptr, *start;
	int slen;
	int plen;
	for (start = (wchar_t *)pString,
		pptr = (wchar_t *)pPattern,
		slen = (int)wstrlen(pString),
		plen = (int)wstrlen(pPattern); slen >= plen; start++, slen--)
	{
		// find start of pattern in string 
		while( *start != *pPattern )
		{
			start++;
			slen--;
			// if pattern longer than string then cannot be matched
			if (slen < plen)
				return(NULL);
		}

		sptr = start;
		pptr = (wchar_t *)pPattern;
		while( *sptr == *pptr )
		{
			sptr++;
			pptr++;
			if ('\0' == *pptr)
				return (start);
		}
	}

	return(NULL);
}

//============================================================================
//! same as strstr but case insensitive
wchar_t * wstristr( const wchar_t * pString, const wchar_t * pPattern )
{
	wchar_t *pptr, *sptr, *start;
	int slen;
	int plen;
	for (start = (wchar_t *)pString,
		pptr = (wchar_t *)pPattern,
		slen = (int)wstrlen(pString),
		plen = (int)wstrlen(pPattern); slen >= plen; start++, slen--)
	{
		// find start of pattern in string 
		while( towupper(*start) != towupper(*pPattern) )
		{
			start++;
			slen--;
			// if pattern longer than string then cannot be matched
			if (slen < plen)
				return(NULL);
		}
		sptr = start;
		pptr = (wchar_t *)pPattern;
		while (towupper(*sptr) == towupper(*pptr))
		{
			sptr++;
			pptr++;
			if ('\0' == *pptr)
				return (start);
		}
	}

	return(NULL);
}

//============================================================================
std::wstring Utf8ToWide(const std::string utf8string)
{
#ifdef TARGET_OS_WINDOWS
	std::wstring convertedString;
	int requiredSize = MultiByteToWideChar(CP_UTF8, 0, utf8string.c_str(), -1, 0, 0);
	if(requiredSize > 0)
	{
		std::vector<wchar_t> buffer(requiredSize);
		MultiByteToWideChar(CP_UTF8, 0, utf8string.c_str(), -1, &buffer[0], requiredSize);
		convertedString.assign(buffer.begin(), buffer.end() - 1);
	}

	return convertedString;
#else
	//TODO Linux version
	size_t asciiSize = utf8string.length();
	wchar_t * buf = new wchar_t[ asciiSize + 2 ];
	buf[0] = 0;
	const char * pTemp = utf8string.c_str();
	for( unsigned int i = 0; i < asciiSize + 1; i++ )
	{
		buf[i] = (wchar_t)pTemp[i];
	}

	std::wstring strResult = buf;
	delete[] buf;
	return strResult;
#endif // TARGET_OS_WINDOWS
}

//============================================================================
std::string WideToUtf8( const std::wstring utf16String )
{
#ifdef TARGET_OS_WINDOWS
	std::string convertedString;
	int requiredSize = WideCharToMultiByte(CP_UTF8, 0, utf16String.c_str(), -1, 0, 0, 0, 0);
	if(requiredSize > 0)
	{
		std::vector<char> buffer(requiredSize);
		WideCharToMultiByte(CP_UTF8, 0, utf16String.c_str(), -1, &buffer[0], requiredSize, 0, 0);
		convertedString.assign(buffer.begin(), buffer.end() - 1);
	}

	return convertedString;
#else
	//TODO Linux version
	size_t widesize = utf16String.length();
	char * buf = new char[ widesize + 2 ];
	buf[0] = 0;
	const wchar_t * pTemp = utf16String.c_str();
	for( unsigned int i = 0; i < widesize + 1; i++ )
	{
		buf[i] = (char)pTemp[i];
	}

	std::string strResult = buf;
	delete[] buf;
	return strResult;
#endif // TARGET_OS_WINDOWS
}

//============================================================================
std::wstring AsciiToWide(const std::string& utf8string)
{
	//TODO Linux version
	size_t asciiSize = utf8string.length();
	wchar_t * buf = new wchar_t[ asciiSize + 2 ];
	buf[0] = 0;
	const char * pTemp = utf8string.c_str();
	for( unsigned int i = 0; i < asciiSize + 1; i++ )
	{
		buf[i] = (wchar_t)pTemp[i];
	}

	std::wstring strResult = buf;
	delete[] buf;
	return strResult;
}

//============================================================================
std::string WideToAscii(const std::wstring& utf16String)
{
	size_t widesize = utf16String.length();
	char * buf = new char[ widesize + 2 ];
	buf[0] = 0;
	const wchar_t * pTemp = utf16String.c_str();
	for( unsigned int i = 0; i < widesize + 1; i++ )
	{
		buf[i] = (char)pTemp[i];
	}

	std::string strResult = buf;
	delete[] buf;
	return strResult;
}

//============================================================================
// return true if prefix matched beginning characters of string
bool StdStringBeginsWith( std::string & csStr, const char * pPrefix )
{
	if( ( NULL == pPrefix ) ||
		( strlen( pPrefix ) > csStr.length() ) )
	{
		return false;
	}
	const char * pStr = csStr.c_str();
	return ( 0 == strncmp( pPrefix, pStr, strlen( pPrefix ) ) );

}

//============================================================================
// return true if suffix matched end characters of string
bool StdStringEndsWith( std::string & csStr, const char * pSuffix )
{
	if( ( NULL == pSuffix ) ||
		( strlen( pSuffix ) > csStr.length() ) )
	{
		return false;
	}
	const char * pStr = csStr.c_str();
	return ( 0 == strcmp( pSuffix, &pStr[ csStr.length() - strlen( pSuffix ) ] ) );

}

//============================================================================
// fill string with integer value
const char * StdStringFromInteger( std::string & strResult, int iValue )
{
	char as8Buf[ 36 ];
	sprintf( as8Buf, "%d", iValue );
	strResult = as8Buf;
	return strResult.c_str();
}

//============================================================================
int StdStringToInteger( std::string& strValue )
{
	if( strValue.length() )
	{
		return atoi( strValue.c_str() );
	}
	return 0;
}
////============================================================================
////! copy string into buffer.. if string is to long then truncate
//void			SafeStringCopy(		std::string		&strSrc,		// string to copy from
//							   char *			pBuf,			// buffer to copy into
//							   unsigned int	uiMaxBufLen )	// max length of buffer	
//{
//	SafeStringCopy(	strSrc.c_str(),		// string to copy from
//		pBuf,			// buffer to copy into
//		uiMaxBufLen );	// max length of buffer	
//}
//
////============================================================================
////! copy string into buffer.. if string is to long then truncate
//void			SafeStringCopy(		const char *	pSrcString,		// string to copy from
//							   char *			pBuf,			// buffer to copy into
//							   unsigned int	uiMaxBufLen )	// max length of buffer	
//{
//	vx_assert( pSrcString );
//	size_t iLen = strlen( pSrcString );
//	if( 0 == uiMaxBufLen )
//	{
//		LogMsg( LOG_INFO, "SafeStringCopy: uiMaxBufLen = 0\n" );
//		return;
//	}
//	if( 0 == iLen )
//	{
//		pBuf[0] = 0;
//		return;
//	}
//	if( iLen > uiMaxBufLen )
//	{
//		memcpy( pBuf, pSrcString, uiMaxBufLen - 1 );
//		pBuf[ uiMaxBufLen - 1 ] = 0; 
//	}
//	else
//	{
//		strcpy( pBuf, pSrcString );
//	}
//}

//============================================================================
//! Safe string copy.. logs error if buffer to short
int SafeStrCopy( char * pDest, const char * pSrc, int iBufLen )
{
	vx_assert( pDest );
	vx_assert( pSrc );

	int iLen = strlen( pSrc );
	if( 0 == iBufLen )
	{
		LogMsg( LOG_INFO, "SafeStringCopy: uiMaxBufLen = 0\n" );
		return -1;
	}

	if( 0 == iLen )
	{
		pDest[0] = 0;
		return 0;
	}	
	
	if( iLen < iBufLen )
	{
		// safe to do string copy
		strcpy( pDest, pSrc );
		return 0;
	}

	// string is to long
	strncpy( pDest, pSrc, iBufLen - 1 );
	pDest[ iBufLen - 1] = 0;
	//LogMsg( LOG_INFO, "CoreLib:Unsafe string copy to 0x%x\n", pDest );
	return -1;
}

//============================================================================
//! Safe string copy.. logs error if buffer to short
int SafeStrCopy( wchar_t * pDest, const wchar_t * pSrc, int iBufLen )
{
	vx_assert( pDest );
	vx_assert( pSrc );
	int iLen = wstrlen( pSrc );
	if( 0 == iBufLen )
	{
		LogMsg( LOG_INFO, "SafeStringCopy: uiMaxBufLen = 0\n" );
		return -1;
	}

	if( 0 == iLen )
	{
		pDest[0] = 0;
		return 0;
	}	

	if( iLen < iBufLen )
	{
		// safe to do string copy
		wstrcpy( pDest, pSrc );
		return 0;
	}
	// string is to long
	wstrncpy( pDest, pSrc, iBufLen - 1 );
	pDest[ iBufLen - 1] = 0;
	LogMsg( LOG_INFO, "CoreLib:Unsafe string copy to 0x%x\n", pDest );
	return -1;
}

//============================================================================
//! return pointer to first char that is not whitespace
char * TrimLeadingSpaces( char *pString )
{
	while(0x20 == pString[0] || '\t' == pString[0] )
	{
		pString++;
		if(('\0' == pString[0]) || ('\n' == pString[0]))
		{
			return NULL;
		}
	}
	return pString;
}

//============================================================================
//! return pointer to first char that is not whitespace
wchar_t * TrimLeadingSpaces( wchar_t *pString )
{
	while(0x20 == pString[0] || '\t' == pString[0] )
	{
		pString++;
		if(('\0' == pString[0]) || ('\n' == pString[0]))
		{
			return NULL;
		}
	}
	return pString;
}

#ifdef TARGET_OS_WINDOWS
//============================================================================
/// strsep does not exist in windows and only some other platforms so
/// make our own
char *strsep(char **ppStr, const char *pDelimiters)
{
	char *pStart;
	char *pEnd;

	pStart = *ppStr;
	if (pStart == NULL)
	{
		return NULL;
	}
	if (pDelimiters[0] == '\0' || pDelimiters[1] == '\0')
	{
		char ch = pDelimiters[0];

		if (ch == '\0')
		{
			pEnd = NULL;
		}
		else
		{
			if (*pStart == ch)
			{
				pEnd = pStart;
			}
			else if (*pStart == '\0')
			{
				pEnd = NULL;
			}
			else
			{
				pEnd = strchr (pStart + 1, ch);
			}
		}
	}
	else
	{
		pEnd = strpbrk (pStart, pDelimiters);
	}
	if (pEnd)
	{
		*pEnd++ = '\0';
		*ppStr = pEnd;
	}
	else
	{
		*ppStr = NULL;
	}
	return pStart;

}

//============================================================================
/// strsep does not exist in windows and only some other platforms so
/// make our own
wchar_t * wstrsep(wchar_t **ppStr, const wchar_t *pDelimiters)
{
	wchar_t *pStart;
	wchar_t *pEnd;

	pStart = *ppStr;
	if (pStart == NULL)
	{
		return NULL;
	}
	if (pDelimiters[0] == '\0' || pDelimiters[1] == '\0')
	{
		wchar_t ch = pDelimiters[0];

		if (ch == '\0')
		{
			pEnd = NULL;
		}
		else
		{
			if (*pStart == ch)
			{
				pEnd = pStart;
			}
			else if (*pStart == '\0')
			{
				pEnd = NULL;
			}
			else
			{
				pEnd = (wchar_t *)wstrchr (pStart + 1, ch);
			}
		}
	}
	else
	{
		pEnd = wstrpbrk (pStart, pDelimiters);
	}
	if (pEnd)
	{
		*pEnd++ = '\0';
		*ppStr = pEnd;
	}
	else
	{
		*ppStr = NULL;
	}
	return pStart;

}

#endif //TARGET_OS_WINDOWS



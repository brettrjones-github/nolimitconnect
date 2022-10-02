// from code project

#include <AppDependLibrariesConfig.h>

#include <memory.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "AltXmlParser.h"
#include <CoreLib/VxLinuxOnly.h>

#ifdef _MSC_VER
# pragma warning( disable: 4996 ) //'strcpy': This function or variable may be unsafe. Consider using strcpy_s instead.
#endif // _MSC_VER


namespace AltXmlParser
{

XMLNode XMLNode::emptyXMLNode;
XMLClear XMLNode::emptyXMLClear={ NULL, NULL, NULL};
XMLAttribute XMLNode::emptyXMLAttribute={ NULL, NULL};

#ifndef TARGET_OS_WINDOWS
int strlen(const char*c)   { return strlen(c); }
int strnicmp(const char*c1, const char*c2, int l) { return strncasecmp(c1,c2,l); }
int strcmp(const char*c1, const char*c2) { return strcasecmp(c1,c2); }
char *_tcsstr(const char*c1, const char*c2) { return (char*)strstr(c1,c2); }
char *_tcschr(const char*c1, int c2) { return (char*)strchr(c1,c2); }
char *strcpy(char *c1, const char*c2) { return (char*)strcpy(c1,c2); }
#endif

inline int mmin( const int t1, const int t2 ) { return t1 < t2 ? t1 : t2; }

// Enumeration used to decipher what type a token is
enum TokenTypeTag
{
	eTokenText = 0,
	eTokenQuotedText,
	eTokenTagStart,         /* "<"            */
	eTokenTagEnd,           /* "</"           */
	eTokenCloseTag,         /* ">"            */
	eTokenEquals,           /* "="            */
	eTokenDeclaration,      /* "<?"           */
	eTokenShortHandClose,   /* "/>"           */
	eTokenClear,
	eTokenError
};

#define INDENTCHAR    _T('\t')

typedef struct ClearTag
{
	const char*				lpszOpen;
	const char*				lpszClose;
} ClearTag;

// Main structure used for parsing XML
typedef struct XML
{
	const char*                lpXML;
	int							nIndex;
	enum XMLError				error;
	const char*                lpEndTag;
	int							cbEndTag;
	const char*                lpNewElement;
	int							cbNewElement;
	int							nFirst;
	ClearTag *					pClrTags;
} XML;

typedef struct
{
	ClearTag *					pClr;
	const char*				pStr;
} NextToken;

// Enumeration used when parsing attributes
typedef enum Attrib
{
	eAttribName = 0,
	eAttribEquals,
	eAttribValue
} Attrib;

// Enumeration used when parsing elements to dictate whether we are currently
// inside a tag
typedef enum Status
{
	eInsideTag = 0,
	eOutsideTag
} Status;

// private:
const char* toXMLString(const char* destIn,const char* source)
{
	char * dest = (char *) destIn;
	char * dd = dest;
	while (*source)
	{
		switch (*source)
		{
		case '<' : strcpy(dest, "&lt;" ); dest+=4; break;
		case '>' : strcpy(dest, "&gt;" ); dest+=4; break;
		case '&' : strcpy(dest, "&amp;" ); dest+=5; break;
		case '\'': strcpy(dest, "&apos;" ); dest+=6; break;
		case '"' : strcpy(dest, "&quot;" ); dest+=6; break;
		default:  *dest=*source; dest++; break;
		}
		source++;
	}
	*dest=0;
	return dd;
}

// private:
int lengthXMLString(const char* source)
{
	int r=0;
	while (*source)
	{
		switch (*source)
		{
		case '<':  r+=3; break;
		case '>' : r+=3; break;
		case '&' : r+=4; break;
		case '\'': r+=5; break;
		case '"' : r+=5; break;
		}
		source++; r++;
	}
	return r;
}

const char* toXMLString(const char* source)
{
	const char* dest=(const char*)malloc((lengthXMLString(source)+1)*sizeof(char));
	return toXMLString(dest,source);
}

const char* toXMLStringFast(const char* *dest,int *destSz, const char* source)
{
	int l=lengthXMLString(source)+1;
	if (l>*destSz) { *destSz=l; *dest=(const char*)realloc( (void *)*dest, l*sizeof(char) ); }
	return toXMLString(*dest,source);
}

// private:
const char* fromXMLString(const char* s, int lo)
{
	// This function is the opposite of the function "toXMLString". It decodes the escape
	// sequences &amp;, &quot;, &apos;, &lt;, &gt; and replace them by the characters
	// &,",',<,>. This function is used internally by the XML Parser. All the calls to
	// the XML library will always gives you back "decoded" strings.
	//
	// in: string (s) and length (lo) of string
	// out:  new allocated string converted from xml
	if (!s) return NULL;

	int ll=0;
	char * d;
	const char* ss=s;
	while (((lo--)>0)&&(*s))
	{
		if (*s==_T('&'))
		{
			s++;
			if ( strnicmp(s, "lt;"  , 3)==0) { s+=2; lo-=3; }
			else if (strnicmp(s, "gt;", 3)==0) { s+=2; lo-=3; }
			else if (strnicmp(s, "amp;", 4)==0) { s+=3; lo-=4; }
			else if (strnicmp(s, "apos;", 5)==0) { s+=4; lo-=5; }
			else if (strnicmp(s, "quot;", 5)==0) { s+=4; lo-=5; }
			else
			{
				ll=0; while (s[ll]&&(s[ll]!=_T(';'))&&(ll<10)) ll++; ll++;
				d=(char *)malloc((ll+1)*sizeof(char));
				d[ll]=0;
				while(ll--) d[ll]=s[ll];
#ifdef _UNICODE
				printf("unknown escape character: '&%S'",d);
#else
				printf("unknown escape character: '&%s'",d);
#endif
				free(d);
				exit(255);
			}
		};
		ll++; s++;
	}

	d=(char *)malloc((ll+1)*sizeof(char));
	s=d;
	while (ll--)
	{
		if (*ss==_T('&'))
		{
			ss++;
			if (strnicmp(ss, "lt;",3)==0) { *(d++)= '<' ; ss+=3; }
			else if (strnicmp(ss, "gt;",3)==0) { *(d++)= '>' ; ss+=3; }
			else if (strnicmp(ss, "amp;",4)==0) { *(d++)= '&' ; ss+=4; }
			else if (strnicmp(ss, "apos;",5)==0) { *(d++)= '\''; ss+=5; }
			else                                     {
				*(d++)= '"' ; ss+=5; }
		} else { *(d++)=*ss; ss++; }
	}
	*d=0;
	return (const char*)s;
}

// private:
char myTagCompare(const char* cclose, const char* copen)
// !!!! WARNING strange convention&:
// return 0 if equals
// return 1 if different
{
	if (!cclose) return 1;
	int l=(int)strlen(cclose);
	if (strnicmp(cclose, copen, l)!=0) return 1;
	const char c=copen[l];
	if ((c== '\n')||
		(c== ' ' )||
		(c== '\t')||
		(c== '\r')||
		(c== '/' )||
		(c== '<' )||
		(c== '>' )||
		(c== '=' )) return 0;
	return 1;
}

// private:
// update "order" information when deleting a content of a XMLNode
void XMLNode::removeOrderElement(XMLNodeData *d, XMLElementType t, int index)
{
	int j=(int)((index<<2)+t),i=0,n=nElement(d)+1, *o=d->pOrder;
	while ((o[i]!=j)&&(i<n)) i++;
	n--;
	memmove(o+i, o+i+1, (n-i)*sizeof(int));
	for (;i<n;i++)
		if ((o[i]&3)==(int)t) o[i]-=4;
	// We should normally do:
	// d->pOrder=(int)realloc(d->pOrder,n*sizeof(int));
	// but we skip reallocation because it's too time consuming.
	// Anyway, at the end, it will be free'd completely at once.
}

// Obtain the next character from the string.
static inline char getNextChar(XML *pXML)
{
	char ch = pXML->lpXML[pXML->nIndex];
	if (ch!=0) pXML->nIndex++;
	return ch;
}

// Find next non-white space character.
static char FindNonWhiteSpace(XML *pXML)
{
	char ch;
	int nExit = false;

	assert(pXML);

	// Iterate through characters in the string until we find a NULL or a
	// non-white space character
	while((nExit == false) && (ch = getNextChar(pXML)))
	{
		switch(ch)
		{
			// Ignore white space
		case _T('\n'):
		case _T(' '):
		case _T('\t'):
		case _T('\r'): continue;
		default: nExit = TRUE;
		}
	}
	return ch;
}

// Find the next token in a string.
// pcbToken contains the number of characters that have been read.
static NextToken GetNextToken(XML *pXML, int *pcbToken, enum TokenTypeTag *pType)
{
	NextToken        result;
	const char*          lpXML;
	char            ch;
	char            chTemp;
	int              nSize;
	int              nFoundMatch;
	int              nExit;
	int              n;
	const char*          lpszOpen;
	int              cbOpen;
	int              nIsText = false;

	// Find next non-white space character
	ch = FindNonWhiteSpace(pXML);

	if (ch)
	{
		// Cache the current string pointer
		lpXML = pXML->lpXML;
		result.pStr = &lpXML[pXML->nIndex-1];

		// First check whether the token is in the clear tag list (meaning it
		// does not need formatting).
		n = 0;

		while(TRUE)
		{
			// Obtain the name of the open part of the clear tag
			lpszOpen = pXML->pClrTags[n].lpszOpen;

			if (lpszOpen)
			{
				// Compare the open tag with the current token
				cbOpen = (int)strlen(lpszOpen);
				// if (myTagCompare(lpszOpen, result.pStr) == 0)
				if (strnicmp(lpszOpen, result.pStr, cbOpen)==0)
				{
					result.pClr = &pXML->pClrTags[n];
					pXML->nIndex += (int)(strlen(lpszOpen)-1);
					*pType  = eTokenClear;
					return result;
				}
				n++;
			}
			else break;
		}
		// If we didn't find a clear tag then check for standard tokens
		chTemp = 0;
		lpXML = pXML->lpXML;

		switch(ch)
		{
			// Check for quotes
		case _T('\''):
		case _T('\"'):
			// Type of token
			*pType = eTokenQuotedText;
			chTemp = ch;
			n=pXML->nIndex;

			// Set the size
			nSize = 1;
			nFoundMatch = false;

			// Search through the string to find a matching quote
			while((ch = getNextChar(pXML)))
			{
				nSize++;
				if (ch==chTemp) { nFoundMatch = TRUE; break; }
				if (ch==_T('<')) break;
			}

			// If we failed to find a matching quote
			if (nFoundMatch == false)
			{
				pXML->nIndex=n-1;
				ch=getNextChar(pXML);
				nIsText=TRUE;
				break;
			}

			//  4.02.2002
			if (FindNonWhiteSpace(pXML))
			{
				pXML->nIndex--;
			}

			break;

			// Equals (used with attribute values)
		case _T('='):
			nSize = 1;
			*pType = eTokenEquals;
			break;

			// Close tag
		case _T('>'):
			nSize = 1;
			*pType = eTokenCloseTag;
			break;

			// Check for tag start and tag end
		case _T('<'):

			// Peek at the next character to see if we have an end tag '</',
			// or an xml declaration '<?'
			chTemp = pXML->lpXML[pXML->nIndex];

			// If we have a tag end...
			if (chTemp == _T('/'))
			{
				// Set the type and ensure we point at the next character
				getNextChar(pXML);
				*pType = eTokenTagEnd;
				nSize = 2;
			}

			// If we have an XML declaration tag
			else if (chTemp == _T('?'))
			{

				// Set the type and ensure we point at the next character
				getNextChar(pXML);
				*pType = eTokenDeclaration;
				nSize = 2;
			}

			// Otherwise we must have a start tag
			else
			{
				*pType = eTokenTagStart;
				nSize = 1;
			}
			break;

			// Check to see if we have a short hand type end tag ('/>').
		case _T('/'):

			// Peek at the next character to see if we have a short end tag '/>'
			chTemp = pXML->lpXML[pXML->nIndex];

			// If we have a short hand end tag...
			if (chTemp == _T('>'))
			{
				// Set the type and ensure we point at the next character
				getNextChar(pXML);
				*pType = eTokenShortHandClose;
				nSize = 2;
				break;
			}

			// If we haven't found a short hand closing tag then drop into the
			// text process

			// Other characters
		default:
			nIsText = TRUE;
		}

		// If this is a TEXT node
		if (nIsText)
		{
			// Indicate we are dealing with text
			*pType = eTokenText;
			nSize = 1;
			nExit = false;

			while((nExit == false) && (ch = getNextChar(pXML)))
			{
				switch(ch)
				{
					// Break when we find white space
				case _T('\n'):
				case _T(' '):
				case _T('\t'):
				case _T('\r'):
					nExit = TRUE;
					break;

					// If we find a slash then this maybe text or a short hand end tag.
				case _T('/'):

					// Peek at the next character to see it we have short hand end tag
					chTemp = pXML->lpXML[pXML->nIndex];

					// If we found a short hand end tag then we need to exit the loop
					if (chTemp == _T('>'))
					{
						pXML->nIndex--; //  03.02.2002
						nExit = TRUE;
					}
					else
					{
						nSize++;
					}
					break;

					// Break when we find a terminator and decrement the index and
					// column count so that we are pointing at the right character
					// the next time we are called.
				case _T('<'):
				case _T('>'):
				case _T('='):
					pXML->nIndex--;
					nExit = TRUE;
					break;

				case 0:
					nExit = TRUE;
					break;

				default:
					nSize++;
				}
			}
		}
		*pcbToken = nSize;
	} else
	{
		// If we failed to obtain a valid character
		*pcbToken = 0;
		*pType = eTokenError;
		result.pStr=NULL;
	}

	return result;
}

// Parse XML errors into a user friendly string.
const char* XMLNode::getError(XMLError error)
{
	switch (error)
	{
	case eXMLErrorNone:              return  "No error";
	case eXMLErrorMissingEndTag:     return "Warning: Unmatched end tag";
	case eXMLErrorEmpty:             return "Error: No XML data";
	case eXMLErrorFirstNotStartTag:  return "Error: First token not start tag";
	case eXMLErrorMissingTagName:    return "Error: Missing start tag name";
	case eXMLErrorMissingEndTagName: return "Error: Missing end tag name";
	case eXMLErrorNoMatchingQuote:   return "Error: Unmatched quote";
	case eXMLErrorUnmatchedEndTag:   return "Error: Unmatched end tag";
	case eXMLErrorUnexpectedToken:   return "Error: Unexpected token found";
	case eXMLErrorInvalidTag:        return "Error: Invalid tag found";
	case eXMLErrorNoElements:        return "Error: No elements found";
	case eXMLErrorFileNotFound:      return "Error: File not found";
	case eXMLErrorTagNotFound:       return "Error: First Tag not found";
	};
	return "Unknown";
}

// private:
XMLNode::XMLNode(XMLNode *pParent, const char* lpszName, int isDeclaration)
{
	d=(XMLNodeData*)malloc(sizeof(XMLNodeData));
	d->ref_count=1;

	d->lpszName=lpszName;

	d->nChild= 0;
	d->nText = 0;
	d->nClear = 0;
	d->nAttribute = 0;

	d->isDeclaration = isDeclaration;

	d->pParent = pParent;
	d->pChild= NULL;
	d->pText= NULL;
	d->pClear= NULL;
	d->pAttribute= NULL;
	d->pOrder= NULL;
}

const int memoryIncrease=50;

static void *myRealloc(void *p, int newsize, int memInc, int sizeofElem)
{
	if (p==NULL) { return malloc(memInc*sizeofElem); }
	if ((newsize%memInc)==0) p=realloc(p,(newsize+memInc)*sizeofElem);
	//    if (!p)
	//    {
	//        printf("XMLParser Error: Not enough memory! Aborting...\n"); exit(220);
	//    }
	return p;
}

void XMLNode::addToOrder(int index, int type)
{
	int n=nElement();
	d->pOrder=(int*)myRealloc(d->pOrder,n+1,memoryIncrease*3,sizeof(int));
	d->pOrder[n]=(index<<2)+type;
}

// Add a child node to the given element.
XMLNode XMLNode::addChild(const char* lpszName, int isDeclaration)
{
	if (!lpszName) return emptyXMLNode;
	int nc=d->nChild;
	d->pChild=(XMLNode*)myRealloc(d->pChild,(nc+1),memoryIncrease,sizeof(XMLNode));
	d->pChild[nc].d=NULL;
	d->pChild[nc]=XMLNode(this,lpszName,isDeclaration);
	addToOrder(nc,eNodeChild);
	d->nChild++;
	return d->pChild[nc];
}

XMLNode XMLNode::createXMLTopNode() { return XMLNode(NULL,NULL,FALSE); }

// Add an attribute to an element.
XMLAttribute *XMLNode::addAttribute(const char* lpszName, const char* lpszValuev)
{
	if (!lpszName) return &emptyXMLAttribute;
	int na=d->nAttribute;
	d->pAttribute=(XMLAttribute*)myRealloc(d->pAttribute,(na+1),memoryIncrease,sizeof(XMLAttribute));
	XMLAttribute *pAttr=d->pAttribute+na;
	pAttr->lpszName = lpszName;
	pAttr->lpszValue = lpszValuev;
	addToOrder(na,eNodeAttribute);
	d->nAttribute++;
	return pAttr;
}

// Add text to the element.
const char* XMLNode::addText(const char* lpszValue)
{
	if (!lpszValue) return NULL;
	int nt=d->nText;
	d->pText=(const char**)myRealloc(d->pText,(nt+1),memoryIncrease,sizeof(const char*));
	d->pText[nt]=lpszValue;
	addToOrder(nt,eNodeText);
	d->nText++;
	return d->pText[nt];
}

// Add clear (unformatted) text to the element.
XMLClear *XMLNode::addClear(const char* lpszValue, const char* lpszOpen, const char* lpszClose)
{
	if (!lpszValue) return &emptyXMLClear;
	int nc=d->nClear;
	d->pClear=(XMLClear *)myRealloc(d->pClear,(nc+1),memoryIncrease,sizeof(XMLClear));
	XMLClear *pNewClear=d->pClear+nc;
	pNewClear->lpszValue = lpszValue;
	pNewClear->lpszOpenTag = lpszOpen;
	pNewClear->lpszCloseTag = lpszClose;
	addToOrder(nc,eNodeClear);
	d->nClear++;
	return pNewClear;
}

// Trim the end of the text to remove white space characters.
static void FindEndOfText(const char* lpszToken, int *pcbText)
{
	char   ch;
	int     cbText;
	assert(lpszToken);
	assert(pcbText);
	cbText = (*pcbText)-1;
	while(TRUE)
	{
		assert(cbText >= 0);
		ch = lpszToken[cbText];
		switch(ch)
		{
		case _T('\r'):
		case _T('\n'):
		case _T('\t'):
		case _T(' '): cbText--; break;
		default: *pcbText = cbText+1; return;
		}
	}
}

// Duplicate a given string.
const char* stringDup(const char* lpszData, int cbData)
{
	if (lpszData==NULL) return NULL;

	char * lpszNew;
	if (cbData==0) cbData=(int)strlen(lpszData);
	lpszNew = (char *)malloc((cbData+1) * sizeof(char));
	if (lpszNew)
	{
		memcpy(lpszNew, lpszData, (cbData) * sizeof(char));
		lpszNew[cbData] = (char)NULL;
	}
	return lpszNew;
}

// private:
// Parse a clear (unformatted) type node.
int XMLNode::ParseClearTag(void *px, void *pa)
{
	XML *pXML=(XML *)px;
	ClearTag *pClear=(ClearTag *)pa;
	int cbTemp = 0;
	const char* lpszTemp;
	const char* lpszXML = &pXML->lpXML[pXML->nIndex];

	// Find the closing tag
	lpszTemp = (char *)strstr(lpszXML, pClear->lpszClose);

	// Iterate through the tokens until we find the closing tag.
	if (lpszTemp)
	{
		// Cache the size and increment the index
		cbTemp = (int)(lpszTemp - lpszXML);

		pXML->nIndex += cbTemp;
		pXML->nIndex += (int)strlen(pClear->lpszClose);

		// Add the clear node to the current element
		addClear(stringDup(lpszXML,cbTemp), pClear->lpszOpen, pClear->lpszClose);
		return TRUE;
	}

	// If we failed to find the end tag
	pXML->error = eXMLErrorUnmatchedEndTag;
	return FALSE;
}

void XMLNode::exactMemory(XMLNodeData *d)
{
	if (memoryIncrease==1) return;
	d->pOrder=(int*)realloc(d->pOrder,(d->nChild+d->nAttribute+d->nText+d->nClear)*sizeof(int));
	d->pChild=(XMLNode*)realloc(d->pChild,d->nChild*sizeof(XMLNode));
	d->pAttribute=(XMLAttribute*)realloc(d->pAttribute,d->nAttribute*sizeof(XMLAttribute));
	d->pText=(const char**)realloc(d->pText,d->nText*sizeof(const char*));
	d->pClear=(XMLClear *)realloc(d->pClear,d->nClear*sizeof(XMLClear));
}

// private:
// Recursively parse an XML element.
int XMLNode::ParseXMLElement(void *pa)
{
	XML *pXML=(XML *)pa;
	int cbToken;
	enum TokenTypeTag type;
	NextToken token;
	const char* lpszTemp;
	int cbTemp;
	int nDeclaration;
	const char* lpszText = NULL;
	XMLNode pNew;
	enum Status status; // inside or outside a tag
	enum Attrib attrib = eAttribName;

	assert(pXML);

	// If this is the first call to the function
	if (pXML->nFirst)
	{
		// Assume we are outside of a tag definition
		pXML->nFirst = FALSE;
		status = eOutsideTag;
	} else
	{
		// If this is not the first call then we should only be called when inside a tag.
		status = eInsideTag;
	}

	// Iterate through the tokens in the document
	while(TRUE)
	{
		// Obtain the next token
		token = GetNextToken(pXML, &cbToken, &type);

		if (type != eTokenError)
		{
			// Check the current status
			switch(status)
			{

				// If we are outside of a tag definition
			case eOutsideTag:

				// Check what type of token we obtained
				switch(type)
				{
					// If we have found text or quoted text
				case eTokenText:
				case eTokenQuotedText:
				case eTokenEquals:
					if (!lpszText)
					{
						lpszText = token.pStr;
					}

					break;

					// If we found a start tag '<' and declarations '<?'
				case eTokenTagStart:
				case eTokenDeclaration:

					// Cache whether this new element is a declaration or not
					nDeclaration = type == eTokenDeclaration;

					// If we have node text then add this to the element
					if (lpszText)
					{
						cbTemp = (int)(token.pStr - lpszText);
						FindEndOfText(lpszText, &cbTemp);
						addText(stringDup(lpszText,cbTemp));
						lpszText = NULL;
					}

					// Find the name of the tag
					token = GetNextToken(pXML, &cbToken, &type);

					// Return an error if we couldn't obtain the next token or
					// it wasnt text
					if (type != eTokenText)
					{
						pXML->error = eXMLErrorMissingTagName;
						return false;
					}

					// If we found a new element which is the same as this
					// element then we need to pass this back to the caller..

#ifdef APPROXIMATE_PARSING
					if (d->lpszName &&
						myTagCompare(d->lpszName, token.pStr) == 0)
					{
						// Indicate to the caller that it needs to create a
						// new element.
						pXML->lpNewElement = token.pStr;
						pXML->cbNewElement = cbToken;
						return TRUE;
					} else
#endif
					{
						// If the name of the new element differs from the name of
						// the current element we need to add the new element to
						// the current one and recurse
						pNew = addChild(stringDup(token.pStr,cbToken), nDeclaration);

						while (!pNew.isEmpty())
						{
							// Callself to process the new node.  If we return
							// FALSE this means we dont have any more
							// processing to do...

							if (!pNew.ParseXMLElement(pXML)) return FALSE;
							else
							{
								// If the call to recurse this function
								// evented in a end tag specified in XML then
								// we need to unwind the calls to this
								// function until we find the appropriate node
								// (the element name and end tag name must
								// match)
								if (pXML->cbEndTag)
								{
									// If we are back at the root node then we
									// have an unmatched end tag
									if (!d->lpszName)
									{
										pXML->error=eXMLErrorUnmatchedEndTag;
										return FALSE;
									}

									// If the end tag matches the name of this
									// element then we only need to unwind
									// once more...

									if (myTagCompare(d->lpszName, pXML->lpEndTag)==0)
									{
										pXML->cbEndTag = 0;
									}

									return TRUE;
								} else
									if (pXML->cbNewElement)
									{
										// If the call indicated a new element is to
										// be created on THIS element.

										// If the name of this element matches the
										// name of the element we need to create
										// then we need to return to the caller
										// and let it process the element.

										if (myTagCompare(d->lpszName, pXML->lpNewElement)==0)
										{
											return TRUE;
										}

										// Add the new element and recurse
										pNew = addChild(stringDup(pXML->lpNewElement,pXML->cbNewElement));
										pXML->cbNewElement = 0;
									}
									else
									{
										// If we didn't have a new element to create
										pNew = emptyXMLNode;

									}
							}
						}
					}
					break;

					// If we found an end tag
				case eTokenTagEnd:

					// If we have node text then add this to the element
					if (lpszText)
					{
						cbTemp = (int)(token.pStr - lpszText);
						FindEndOfText(lpszText, &cbTemp);
						addText(fromXMLString(lpszText,cbTemp));
						lpszText = NULL;
					}

					// Find the name of the end tag
					token = GetNextToken(pXML, &cbTemp, &type);

					// The end tag should be text
					if (type != eTokenText)
					{
						pXML->error = eXMLErrorMissingEndTagName;
						return false;
					}
					lpszTemp = token.pStr;

					// After the end tag we should find a closing tag
					token = GetNextToken(pXML, &cbToken, &type);
					if (type != eTokenCloseTag)
					{
						pXML->error = eXMLErrorMissingEndTagName;
						return false;
					}

					// We need to return to the previous caller.  If the name
					// of the tag cannot be found we need to keep returning to
					// caller until we find a match
					if (myTagCompare(d->lpszName, lpszTemp) != 0)
#ifdef STRICT_PARSING
					{
						pXML->error=eXMLErrorUnmatchedEndTag;
						return false;
					}
#else
					{
						pXML->error=eXMLErrorMissingEndTag;
						pXML->lpEndTag = lpszTemp;
						pXML->cbEndTag = cbTemp;
					}
#endif

					// Return to the caller
					exactMemory(d);
					return TRUE;

					// If we found a clear (unformatted) token
				case eTokenClear:
					// If we have node text then add this to the element
					if (lpszText)
					{
						cbTemp = (int)(token.pStr - lpszText);
						FindEndOfText(lpszText, &cbTemp);
						addText(stringDup(lpszText,cbTemp));
						lpszText = NULL;
					}

					if (!ParseClearTag(pXML, token.pClr))
					{
						return false;
					}
					break;

					// Errors...
				case eTokenCloseTag:          /* '>'         */
				case eTokenShortHandClose:    /* '/>'        */
					pXML->error = eXMLErrorUnexpectedToken;
					return false;
				default:
					break;
				}
				break;

				// If we are inside a tag definition we need to search for attributes
			case eInsideTag:

				// Check what part of the attribute (name, equals, value) we
				// are looking for.
				switch(attrib)
				{
					// If we are looking for a new attribute
				case eAttribName:

					// Check what the current token type is
					switch(type)
					{
						// If the current type is text...
						// Eg.  'attribute'
					case eTokenText:
						// Cache the token then indicate that we are next to
						// look for the equals
						lpszTemp = token.pStr;
						cbTemp = cbToken;
						attrib = eAttribEquals;
						break;

						// If we found a closing tag...
						// Eg.  '>'
					case eTokenCloseTag:
						// We are now outside the tag
						status = eOutsideTag;
						break;

						// If we found a short hand '/>' closing tag then we can
						// return to the caller
					case eTokenShortHandClose:
						exactMemory(d);
						return TRUE;

						// Errors...
					case eTokenQuotedText:    /* '"SomeText"'   */
					case eTokenTagStart:      /* '<'            */
					case eTokenTagEnd:        /* '</'           */
					case eTokenEquals:        /* '='            */
					case eTokenDeclaration:   /* '<?'           */
					case eTokenClear:
						pXML->error = eXMLErrorUnexpectedToken;
						return false;
					default: break;
					}
					break;

					// If we are looking for an equals
				case eAttribEquals:
					// Check what the current token type is
					switch(type)
					{
						// If the current type is text...
						// Eg.  'Attribute AnotherAttribute'
					case eTokenText:
						// Add the unvalued attribute to the list
						addAttribute(stringDup(lpszTemp,cbTemp), NULL);
						// Cache the token then indicate.  We are next to
						// look for the equals attribute
						lpszTemp = token.pStr;
						cbTemp = cbToken;
						break;

						// If we found a closing tag 'Attribute >' or a short hand
						// closing tag 'Attribute />'
					case eTokenShortHandClose:
					case eTokenCloseTag:
						// If we are a declaration element '<?' then we need
						// to remove extra closing '?' if it exists
						if (d->isDeclaration &&
							(lpszTemp[cbTemp-1]) == _T('?'))
						{
							cbTemp--;
						}

						if (cbTemp)
						{
							// Add the unvalued attribute to the list
							addAttribute(stringDup(lpszTemp,cbTemp), NULL);
						}

						// If this is the end of the tag then return to the caller
						if (type == eTokenShortHandClose)
						{
							exactMemory(d);
							return TRUE;
						}

						// We are now outside the tag
						status = eOutsideTag;
						break;

						// If we found the equals token...
						// Eg.  'Attribute ='
					case eTokenEquals:
						// Indicate that we next need to search for the value
						// for the attribute
						attrib = eAttribValue;
						break;

						// Errors...
					case eTokenQuotedText:    /* 'Attribute "InvalidAttr"'*/
					case eTokenTagStart:      /* 'Attribute <'            */
					case eTokenTagEnd:        /* 'Attribute </'           */
					case eTokenDeclaration:   /* 'Attribute <?'           */
					case eTokenClear:
						pXML->error = eXMLErrorUnexpectedToken;
						return false;
					default: break;
					}
					break;

					// If we are looking for an attribute value
				case eAttribValue:
					// Check what the current token type is
					switch(type)
					{
						// If the current type is text or quoted text...
						// Eg.  'Attribute = "Value"' or 'Attribute = Value' or
						// 'Attribute = 'Value''.
					case eTokenText:
					case eTokenQuotedText:
						// If we are a declaration element '<?' then we need
						// to remove extra closing '?' if it exists
						if (d->isDeclaration &&
							(token.pStr[cbToken-1]) == _T('?'))
						{
							cbToken--;
						}

						if (cbTemp)
						{
							// Add the valued attribute to the list
							if (type==eTokenQuotedText)
							{ token.pStr++; cbToken-=2; }
							addAttribute(stringDup(lpszTemp,cbTemp),fromXMLString(token.pStr,cbToken));
						}

						// Indicate we are searching for a new attribute
						attrib = eAttribName;
						break;

						// Errors...
					case eTokenTagStart:        /* 'Attr = <'          */
					case eTokenTagEnd:          /* 'Attr = </'         */
					case eTokenCloseTag:        /* 'Attr = >'          */
					case eTokenShortHandClose:  /* "Attr = />"         */
					case eTokenEquals:          /* 'Attr = ='          */
					case eTokenDeclaration:     /* 'Attr = <?'         */
					case eTokenClear:
						pXML->error = eXMLErrorUnexpectedToken;
						return false;
						break;
					default: break;
					}
				}
			}
		}
		// If we failed to obtain the next token
		else
		{
			return false;
		}
	}
}

// Count the number of lines and columns in an XML string.
static void CountLinesAndColumns(const char* lpXML, int nUpto, XMLResults *pResults)
{
	char ch;
	int n;

	assert(lpXML);
	assert(pResults);

	pResults->nLine = 1;
	pResults->nColumn = 1;
	for(n=0; n<nUpto; n++)
	{
		ch = lpXML[n];
		assert(ch);
		if (ch == _T('\n'))
		{
			pResults->nLine++;
			pResults->nColumn = 1;
		}
		else pResults->nColumn++;
	}
}

// Parse XML and return the root element.
XMLNode XMLNode::parseString(const char* lpszXML, const char* tag, XMLResults *pResults)
{
	if (!lpszXML)
	{
		if (pResults)
		{
			pResults->error=eXMLErrorNoElements;
			pResults->nLine=0;
			pResults->nColumn=0;
		}
		return emptyXMLNode;
	}
	static struct ClearTag tags[] =
	{
		{    "<![CDATA[",    "]]>"       },
		{    "<PRE>",        "</PRE>"    },
		{    "<Script>",     "</Script>" },
		{    "<!--",         "-->"       },
		{    "<!DOCTYPE",    ">"         },
		{    NULL,               NULL            }
	};
	enum XMLError error;
	XMLNode xnode(NULL,NULL,FALSE);
	struct XML xml={ NULL, 0, eXMLErrorNone, NULL, 0, NULL, 0, TRUE , NULL};

	xml.lpXML = lpszXML;
	xml.pClrTags = tags;

	// Create header element
	xnode.ParseXMLElement(&xml);
	error = xml.error;

	// If an error occurred
	if ((error==eXMLErrorNone)||(error==eXMLErrorMissingEndTag))
	{
		if (tag&&strlen(tag))
		{
			XMLNode nodeTmp;
			int i=0;
			while (i<xnode.nChildNode())
			{
				nodeTmp=xnode.getChildNode(i);
				if (stricmp(nodeTmp.getName(),tag)==0) break;
				if (nodeTmp.isDeclaration()) { xnode=nodeTmp; i=0; } else i++;
			}
			if (i<0)
			{
				if (pResults)
				{
					pResults->error=eXMLErrorTagNotFound;
					pResults->nLine=0;
					pResults->nColumn=0;
				}
				return emptyXMLNode;
			}
			xnode=nodeTmp;
		}
	} else
	{
		// Cleanup: this will destroy all the nodes
		xnode = emptyXMLNode;
	}


	// If we have been given somewhere to place results
	if (pResults)
	{
		pResults->error = error;

		// If we have an error
		if ((error!=eXMLErrorNone)&&(error!=eXMLErrorMissingEndTag))
		{
			// Find which line and column it starts on.
			CountLinesAndColumns(xml.lpXML, xml.nIndex, pResults);
		}
	}
	return xnode;
}

XMLNode XMLNode::parseFile(const char*filename, const char* tag, XMLResults *pResults)
{
	FILE *f=fopen(filename,"rb");
	if (f==NULL)
	{
		if (pResults)
		{
			pResults->error=eXMLErrorFileNotFound;
			pResults->nLine=0;
			pResults->nColumn=0;
		}
		return emptyXMLNode;
	}
	fseek(f,0,SEEK_END);
	int l=ftell(f);
	fseek(f,0,SEEK_SET);
	char *buf=(char*)malloc(l+1);
	fread(buf,l,1,f);
	fclose(f);
	buf[l]=0;
	XMLNode x = parseString( (const char*)buf, tag, pResults );
	free(buf);
	return x;
}

XMLNode XMLNode::openFileHelper(const char*lpszXML, const char* tag)
{
	XMLResults pResults;
	XMLNode xnode=XMLNode::parseFile(lpszXML, tag, &pResults);
	if (pResults.error != eXMLErrorNone)
	{
		printf(
			"XML Parsing error inside file '%s'.\n"
#ifdef _UNICODE
			"Error: %S\n"
#else
			"Error: %s\n"
#endif
			"At line %i, column %i.\n",lpszXML,
			XMLNode::getError(pResults.error),pResults.nLine,pResults.nColumn);
		if (pResults.error==eXMLErrorTagNotFound)
		{
#ifdef _UNICODE
			printf("Tag is '%S'.\n",tag);
#else
			printf("Tag is '%s'.\n",tag);
#endif
		}
		exit(255);
	}
	return xnode;
}


XMLNodeContents XMLNode::enumContents(int i)
{
	XMLNodeContents c;
	if (!d) { c.type=eNodeNULL; return c; }
	c.type=(XMLElementType)(d->pOrder[i]&3);
	i=(d->pOrder[i])>>2;
	switch (c.type)
	{
	case eNodeChild:     c.child = d->pChild[i];      break;
	case eNodeAttribute: c.attrib= d->pAttribute[i];  break;
	case eNodeText:      c.text  = d->pText[i];       break;
	case eNodeClear:     c.clear = d->pClear[i];      break;
	default: break;
	}
	return c;
}

// private:
void *XMLNode::enumContent(XMLNodeData *pEntry, int i, XMLElementType *nodeType)
{
	XMLElementType j=(XMLElementType)(pEntry->pOrder[i]&3);
	*nodeType=j;
	i=(pEntry->pOrder[i])>>2;
	switch (j)
	{
	case eNodeChild:      return pEntry->pChild[i].d;
	case eNodeAttribute:  return pEntry->pAttribute+i;
	case eNodeText:       return (void*)(pEntry->pText[i]);
	case eNodeClear:      return pEntry->pClear+i;
	default: break;
	}
	return NULL;
}

// private:
int XMLNode::nElement(XMLNodeData *pEntry)
{
	return pEntry->nChild+pEntry->nText+pEntry->nClear+pEntry->nAttribute;
}

static inline void charmemset( const char* destIn, char c, int l) 
{ 
	char * dest = (char *)destIn;
	while (l--) *(dest++)=c; 
}
// private:
// Creates an user friendly XML string from a given element with
// appropriate white space and carriage returns.
//
// This recurses through all subnodes then adds contents of the nodes to the
// string.
int XMLNode::CreateXMLStringR(XMLNodeData *pEntry, char * lpszMarker, int nFormat)
{
	int nResult = 0;
	int cb;
	int cbElement;
	int nIndex;
	int nChildFormat=-1;
	int bHasChildren=FALSE;
	int i;
	XMLAttribute * pAttr;

	assert(pEntry);

#define LENSTR(lpsz) (lpsz ? strlen(lpsz) : 0)

	// If the element has no name then assume this is the head node.
	cbElement = (int)LENSTR(pEntry->lpszName);

	if (cbElement)
	{
		// "<elementname "
		cb = nFormat == -1 ? 0 : nFormat;

		if (lpszMarker)
		{
			if (cb) charmemset(lpszMarker, INDENTCHAR, sizeof(char)*cb);
			nResult = cb;
			lpszMarker[nResult++]=_T('<');
			if (pEntry->isDeclaration) lpszMarker[nResult++]=_T('?');
			strcpy(&lpszMarker[nResult], pEntry->lpszName);
			nResult+=cbElement;
			lpszMarker[nResult++]=_T(' ');

		} else
		{
			nResult+=cbElement+2+cb;
			if (pEntry->isDeclaration) nResult++;
		}

		// Enumerate attributes and add them to the string
		nIndex = pEntry->nAttribute; pAttr=pEntry->pAttribute;
		for (i=0; i<nIndex; i++)
		{
			// "Attrib
			cb = (int)LENSTR(pAttr->lpszName);
			if (cb)
			{
				if (lpszMarker) strcpy(&lpszMarker[nResult], pAttr->lpszName);
				nResult += cb;
				// "Attrib=Value "
				cb = (int)lengthXMLString(pAttr->lpszValue);
				if (lpszMarker)
				{
					lpszMarker[nResult]=_T('=');
					lpszMarker[nResult+1]=_T('"');
					if (cb) toXMLString(&lpszMarker[nResult+2],pAttr->lpszValue);
					lpszMarker[nResult+cb+2]=_T('"');
				}
				nResult+=cb+3;
				if (lpszMarker) lpszMarker[nResult] = _T(' ');
				nResult++;
			}
			pAttr++;
		}

		bHasChildren=(pEntry->nAttribute!=nElement(pEntry));
		if (pEntry->isDeclaration)
		{
			if (lpszMarker)
			{
				lpszMarker[nResult-1]=_T('?');
				lpszMarker[nResult]=_T('>');
			}
			nResult++;
			if (nFormat!=-1)
			{
				if (lpszMarker) lpszMarker[nResult]=_T('\n');
				nResult++;
			}
		} else
			// If there are child nodes we need to terminate the start tag
			if (bHasChildren)
			{
				if (lpszMarker) lpszMarker[nResult-1]=_T('>');
				if (nFormat!=-1)
				{
					if (lpszMarker) lpszMarker[nResult]=_T('\n');
					nResult++;
				}
			} else nResult--;
	}

	// Calculate the child format for when we recurse.  This is used to
	// determine the number of spaces used for prefixes.
	if (nFormat!=-1)
	{
		if (cbElement&&(!pEntry->isDeclaration)) nChildFormat=nFormat+1;
		else nChildFormat=nFormat;
	}

	// Enumerate through remaining children
	nIndex = nElement(pEntry);
	XMLElementType nodeType;
	void *pChild;
	for (i=0; i<nIndex; i++)
	{
		pChild=enumContent(pEntry, i, &nodeType);
		switch(nodeType)
		{
			// Text nodes
		case eNodeText:
			// "Text"
			cb = (int)lengthXMLString((const char*)pChild);
			if (cb)
			{
				if (nFormat!=-1)
				{
					if (lpszMarker)
					{
						charmemset(&lpszMarker[nResult],INDENTCHAR,sizeof(char)*(nFormat + 1));
						toXMLString(&lpszMarker[nResult+nFormat+1],(const char*)pChild);
						lpszMarker[nResult+nFormat+1+cb]=_T('\n');
					}
					nResult+=cb+nFormat+2;
				} else
				{
					if (lpszMarker) toXMLString(&lpszMarker[nResult], (const char*)pChild);
					nResult += cb;
				}
			}
			break;


			// Clear type nodes
		case eNodeClear:
			// "OpenTag"
			cb = (int)LENSTR(((XMLClear*)pChild)->lpszOpenTag);
			if (cb)
			{
				if (nFormat!=-1)
				{
					if (lpszMarker)
					{
						charmemset(&lpszMarker[nResult], INDENTCHAR, sizeof(char)*(nFormat + 1));
						strcpy(&lpszMarker[nResult+nFormat+1], ((XMLClear*)pChild)->lpszOpenTag);
					}
					nResult+=cb+nFormat+1;
				}
				else
				{
					if (lpszMarker)strcpy(&lpszMarker[nResult], ((XMLClear*)pChild)->lpszOpenTag);
					nResult += cb;
				}
			}

			// "OpenTag Value"
			cb = (int)LENSTR(((XMLClear*)pChild)->lpszValue);
			if (cb)
			{
				if (lpszMarker) strcpy(&lpszMarker[nResult], ((XMLClear*)pChild)->lpszValue);
				nResult += cb;
			}

			// "OpenTag Value CloseTag"
			cb = (int)LENSTR(((XMLClear*)pChild)->lpszCloseTag);
			if (cb)
			{
				if (lpszMarker) strcpy(&lpszMarker[nResult], ((XMLClear*)pChild)->lpszCloseTag);
				nResult += cb;
			}

			if (nFormat!=-1)
			{
				if (lpszMarker) lpszMarker[nResult] = _T('\n');
				nResult++;
			}
			break;

			// Element nodes
		case eNodeChild:

			// Recursively add child nodes
			nResult += CreateXMLStringR((XMLNodeData*)pChild,
				lpszMarker ? lpszMarker + nResult : 0, nChildFormat);
			break;
		default: break;
		}
	}

	if ((cbElement)&&(!pEntry->isDeclaration))
	{
		// If we have child entries we need to use long XML notation for
		// closing the element - "<elementname>blah blah blah</elementname>"
		if (bHasChildren)
		{
			// "</elementname>\0"
			if (lpszMarker)
			{
				if (nFormat != -1)
				{
					if (nFormat)
					{
						charmemset(&lpszMarker[nResult], INDENTCHAR,sizeof(char)*nFormat);
						nResult+=nFormat;
					}
				}

				strcpy(&lpszMarker[nResult], "</");
				nResult += 2;
				strcpy(&lpszMarker[nResult], pEntry->lpszName);
				nResult += cbElement;

				if (nFormat == -1)
				{
					strcpy(&lpszMarker[nResult], ">");
					nResult++;
				} else
				{
					strcpy(&lpszMarker[nResult], ">\n");
					nResult+=2;
				}
			} else
			{
				if (nFormat != -1) nResult+=cbElement+4+nFormat;
				else nResult+=cbElement+3;
			}
		} else
		{
			// If there are no children we can use shorthand XML notation -
			// "<elementname/>"
			// "/>\0"
			if (lpszMarker)
			{
				if (nFormat == -1)
				{
					strcpy(&lpszMarker[nResult], "/>");
					nResult += 2;
				}
				else
				{
					strcpy(&lpszMarker[nResult], "/>\n");
					nResult += 3;
				}
			}
			else
			{
				nResult += nFormat == -1 ? 2 : 3;
			}
		}
	}

	return nResult;
}

#undef LENSTR

// Create an XML string from the head element.
// @param       XMLElement * pHead      - head element
// @param       int nFormat             - 0 if no formatting is required
//                                        otherwise nonzero for formatted text
//                                        with carriage returns and indentation.
// @param       int *pnSize             - [out] pointer to the size of the
//                                        returned string not including the
//                                        NULL terminator.
//
// @return      const char*                  - Allocated XML string, you must free
//                                        this with free().
const char* XMLNode::createXMLString(int nFormat, int *pnSize)
{
	if (!d) { if (pnSize) *pnSize=0; return NULL; }

	char * lpszResult = NULL;
	int cbStr;

	// Recursively Calculate the size of the XML string
	nFormat = nFormat ? 0 : -1;
	cbStr = CreateXMLStringR(d, 0, nFormat);
	assert(cbStr);
	// Allocate memory for the XML string + the NULL terminator and
	// create the recursively XML string.
	lpszResult=(char *)malloc((cbStr+1)*sizeof(char));
	CreateXMLStringR(d, lpszResult, nFormat);
	if (pnSize) *pnSize = cbStr;
	return lpszResult;
}

XMLNode::~XMLNode()
{
	destroyCurrentBuffer(d);
};

void XMLNode::destroyCurrentBuffer(XMLNodeData *d)
{
	if (!d) return;
	(d->ref_count) --;
	if (d->ref_count==0)
	{
		int i=0;

		if (d->pParent)
		{
			XMLNode *pa=d->pParent->d->pChild;
			while (((void*)(pa[i].d))!=((void*)d)) i++;
			d->pParent->d->nChild--;
			memmove(pa+i,pa+i+1,(d->pParent->d->nChild-i)*sizeof(XMLNode));
			removeOrderElement(d->pParent->d,eNodeChild,i);
		}

		for(i=0; i<d->nChild; i++) { d->pChild[i].d->pParent=NULL; destroyCurrentBuffer(d->pChild[i].d); }
		free(d->pChild);
		for(i=0; i<d->nText; i++) free((void*)d->pText[i]);
		free(d->pText);
		for(i=0; i<d->nClear; i++) free((void*)d->pClear[i].lpszValue);
		free(d->pClear);
		for(i=0; i<d->nAttribute; i++)
		{
			free((void*)d->pAttribute[i].lpszName);
			if (d->pAttribute[i].lpszValue) free((void*)d->pAttribute[i].lpszValue);
		}
		free(d->pAttribute);
		free(d->pOrder);
		free((void*)d->lpszName);
		free(d);
	}
}

XMLNode& XMLNode::operator=( const XMLNode& A )
{
	// shallow copy
	if (this != &A)
	{
		destroyCurrentBuffer(d);
		d=A.d;
		if (d) (d->ref_count) ++ ;
	}
	return *this;
}

XMLNode::XMLNode(const XMLNode &A)
{
	// shallow copy
	d=A.d;
	if (d) (d->ref_count)++ ;
}

int XMLNode::nChildNode(const char* name)
{
	if (!d) return 0;
	int i,j=0,n=d->nChild;
	XMLNode *pc=d->pChild;
	for (i=0; i<n; i++)
	{
		if (strcmp(pc->d->lpszName, name)==0) j++;
		pc++;
	}
	return j;
}

XMLNode XMLNode::getChildNode(const char* name, int *j)
{
	char temp[1024];
	char *p = NULL;
	if (!d) return emptyXMLNode;
	int i=0,n=d->nChild;
	if (j) i=*j;
	XMLNode *pc=d->pChild+i;
	for (; i<n; i++)
	{
		p = (char *)strstr(pc->d->lpszName, ":");
		if(p){
			memset(temp, 0, sizeof(temp));
			strcpy(temp, p+1);
		}

		if(p){
			if (strcmp(temp, name)==0){
				if (j) *j=i+1;
				return *pc;
			}
			pc++;
		}else{
			if (strcmp(pc->d->lpszName, name)==0){
				if (j) *j=i+1;
				return *pc;
			}
			pc++;
		}

	}
	return emptyXMLNode;
}

XMLNode XMLNode::getChildNode(const char* name, int j)
{
	if (!d) return emptyXMLNode;
	int i=0;
	while (j-->0) getChildNode(name,&i);
	return getChildNode(name,&i);
}

// Find an attribute on an node.
const char* XMLNode::getAttribute(const char* lpszAttrib, int *j)
{
	if (!d) return NULL;
	int i=0,n=d->nAttribute;
	if (j) i=*j;
	XMLAttribute *pAttr=d->pAttribute+i;
	for (; i<n; i++)
	{
		if (strcmp(pAttr->lpszName, lpszAttrib)==0)
		{
			if (j) *j=i+1;
			return pAttr->lpszValue;
		}
		pAttr++;
	}
	return NULL;
}

char XMLNode::isAttributeSet(const char* lpszAttrib)
{
	if (!d) return false;
	int i,n=d->nAttribute;
	XMLAttribute *pAttr=d->pAttribute;
	for (i=0; i<n; i++)
	{
		if (strcmp(pAttr->lpszName, lpszAttrib)==0)
		{
			return true;
		}
		pAttr++;
	}
	return false;
}

const char* XMLNode::getAttribute(const char* name, int j)
{
	if (!d) return NULL;
	int i=0;
	while (j-->0) getAttribute(name,&i);
	return getAttribute(name,&i);
}

const char* XMLNode::getName(){ if (!d) return NULL; return d->lpszName;   }
int XMLNode::nText()      { if (!d) return 0; return d->nText;      }
int XMLNode::nChildNode() { if (!d) return 0; return d->nChild;     }
int XMLNode::nAttribute() { if (!d) return 0; return d->nAttribute; }
int XMLNode::nClear()     { if (!d) return 0; return d->nClear;     }
XMLClear     XMLNode::getClear     (int i) { if (!d) return emptyXMLClear;     if (i>=d->nClear    ) return emptyXMLClear;     return d->pClear[i];     }
XMLAttribute XMLNode::getAttribute (int i) { if (!d) return emptyXMLAttribute; if (i>=d->nAttribute) return emptyXMLAttribute; return d->pAttribute[i]; }
const char*      XMLNode::getText      (int i) { if (!d) return NULL;              if (i>=d->nText     ) return NULL;              return d->pText[i];      }
XMLNode      XMLNode::getChildNode (int i) { if (!d) return emptyXMLNode;      if (i>=d->nChild    ) return emptyXMLNode;      return d->pChild[i];     }
char         XMLNode::isDeclaration(     ) { if (!d) return 0;                 return d->isDeclaration; }
char         XMLNode::isEmpty      (     ) { return (d==NULL); }
int          XMLNode::nElement     (     ) { if (!d) return 0; return d->nChild+d->nText+d->nClear+d->nAttribute; }

} // namespace AltXmlParser
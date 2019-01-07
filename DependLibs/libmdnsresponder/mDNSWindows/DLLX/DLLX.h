

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Wed Apr 12 22:02:10 2017
 */
/* Compiler settings for DLLX.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __DLLX_h__
#define __DLLX_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ITXTRecord_FWD_DEFINED__
#define __ITXTRecord_FWD_DEFINED__
typedef interface ITXTRecord ITXTRecord;

#endif 	/* __ITXTRecord_FWD_DEFINED__ */


#ifndef __IDNSSDRecord_FWD_DEFINED__
#define __IDNSSDRecord_FWD_DEFINED__
typedef interface IDNSSDRecord IDNSSDRecord;

#endif 	/* __IDNSSDRecord_FWD_DEFINED__ */


#ifndef __IDNSSDEventManager_FWD_DEFINED__
#define __IDNSSDEventManager_FWD_DEFINED__
typedef interface IDNSSDEventManager IDNSSDEventManager;

#endif 	/* __IDNSSDEventManager_FWD_DEFINED__ */


#ifndef __IDNSSDService_FWD_DEFINED__
#define __IDNSSDService_FWD_DEFINED__
typedef interface IDNSSDService IDNSSDService;

#endif 	/* __IDNSSDService_FWD_DEFINED__ */


#ifndef ___IDNSSDEvents_FWD_DEFINED__
#define ___IDNSSDEvents_FWD_DEFINED__
typedef interface _IDNSSDEvents _IDNSSDEvents;

#endif 	/* ___IDNSSDEvents_FWD_DEFINED__ */


#ifndef __DNSSDService_FWD_DEFINED__
#define __DNSSDService_FWD_DEFINED__

#ifdef __cplusplus
typedef class DNSSDService DNSSDService;
#else
typedef struct DNSSDService DNSSDService;
#endif /* __cplusplus */

#endif 	/* __DNSSDService_FWD_DEFINED__ */


#ifndef __TXTRecord_FWD_DEFINED__
#define __TXTRecord_FWD_DEFINED__

#ifdef __cplusplus
typedef class TXTRecord TXTRecord;
#else
typedef struct TXTRecord TXTRecord;
#endif /* __cplusplus */

#endif 	/* __TXTRecord_FWD_DEFINED__ */


#ifndef __DNSSDRecord_FWD_DEFINED__
#define __DNSSDRecord_FWD_DEFINED__

#ifdef __cplusplus
typedef class DNSSDRecord DNSSDRecord;
#else
typedef struct DNSSDRecord DNSSDRecord;
#endif /* __cplusplus */

#endif 	/* __DNSSDRecord_FWD_DEFINED__ */


#ifndef __DNSSDEventManager_FWD_DEFINED__
#define __DNSSDEventManager_FWD_DEFINED__

#ifdef __cplusplus
typedef class DNSSDEventManager DNSSDEventManager;
#else
typedef struct DNSSDEventManager DNSSDEventManager;
#endif /* __cplusplus */

#endif 	/* __DNSSDEventManager_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_DLLX_0000_0000 */
/* [local] */ 

typedef /* [uuid] */  DECLSPEC_UUID("4085DD59-D0E1-4efe-B6EE-DDBF7631B9C0") 
enum DNSSDFlags
    {
        kDNSSDFlagsMoreComing	= 0x1,
        kDNSSDFlagsDefault	= 0x4,
        kDNSSDFlagsNoAutoRename	= 0x8,
        kDNSSDFlagsShared	= 0x10,
        kDNSSDFlagsUnique	= 0x20,
        kDNSSDFlagsBrowseDomains	= 0x40,
        kDNSSDFlagsRegistrationDomains	= 0x80,
        kDNSSDFlagsLongLivedQuery	= 0x100,
        kDNSSDFlagsAllowRemoteQuery	= 0x200,
        kDNSSDFlagsForceMulticast	= 0x400,
        kDNSSDFlagsForce	= 0x800,
        kDNSSDFlagsReturnIntermediates	= 0x1000,
        kDNSSDFlagsNonBrowsable	= 0x2000
    } 	DNSSDFlags;

typedef /* [uuid] */  DECLSPEC_UUID("30CDF335-CA52-4b17-AFF2-E83C64C450D4") 
enum DNSSDAddressFamily
    {
        kDNSSDAddressFamily_IPv4	= 0x1,
        kDNSSDAddressFamily_IPv6	= 0x2
    } 	DNSSDAddressFamily;

typedef /* [uuid] */  DECLSPEC_UUID("98FB4702-7374-4b16-A8DB-AD35BFB8364D") 
enum DNSSDProtocol
    {
        kDNSSDProtocol_UDP	= 0x10,
        kDNSSDProtocol_TCP	= 0x20
    } 	DNSSDProtocol;

typedef /* [uuid] */  DECLSPEC_UUID("72BF3EC3-19BC-47e5-8D95-3B73FF37D893") 
enum DNSSDRRClass
    {
        kDNSSDClass_IN	= 1
    } 	DNSSDRRClass;

typedef /* [uuid] */  DECLSPEC_UUID("08E362DF-5468-4c9a-AC66-FD4747B917BD") 
enum DNSSDRRType
    {
        kDNSSDType_A	= 1,
        kDNSSDType_NS	= 2,
        kDNSSDType_MD	= 3,
        kDNSSDType_MF	= 4,
        kDNSSDType_CNAME	= 5,
        kDNSSDType_SOA	= 6,
        kDNSSDType_MB	= 7,
        kDNSSDType_MG	= 8,
        kDNSSDType_MR	= 9,
        kDNSSDType_NULL	= 10,
        kDNSSDType_WKS	= 11,
        kDNSSDType_PTR	= 12,
        kDNSSDType_HINFO	= 13,
        kDNSSDType_MINFO	= 14,
        kDNSSDType_MX	= 15,
        kDNSSDType_TXT	= 16,
        kDNSSDType_RP	= 17,
        kDNSSDType_AFSDB	= 18,
        kDNSSDType_X25	= 19,
        kDNSSDType_ISDN	= 20,
        kDNSSDType_RT	= 21,
        kDNSSDType_NSAP	= 22,
        kDNSSDType_NSAP_PTR	= 23,
        kDNSSDType_SIG	= 24,
        kDNSSDType_KEY	= 25,
        kDNSSDType_PX	= 26,
        kDNSSDType_GPOS	= 27,
        kDNSSDType_AAAA	= 28,
        kDNSSDType_LOC	= 29,
        kDNSSDType_NXT	= 30,
        kDNSSDType_EID	= 31,
        kDNSSDType_NIMLOC	= 32,
        kDNSSDType_SRV	= 33,
        kDNSSDType_ATMA	= 34,
        kDNSSDType_NAPTR	= 35,
        kDNSSDType_KX	= 36,
        kDNSSDType_CERT	= 37,
        kDNSSDType_A6	= 38,
        kDNSSDType_DNAME	= 39,
        kDNSSDType_SINK	= 40,
        kDNSSDType_OPT	= 41,
        kDNSSDType_APL	= 42,
        kDNSSDType_DS	= 43,
        kDNSSDType_SSHFP	= 44,
        kDNSSDType_IPSECKEY	= 45,
        kDNSSDType_RRSIG	= 46,
        kDNSSDType_NSEC	= 47,
        kDNSSDType_DNSKEY	= 48,
        kDNSSDType_DHCID	= 49,
        kDNSSDType_NSEC3	= 50,
        kDNSSDType_NSEC3PARAM	= 51,
        kDNSSDType_HIP	= 55,
        kDNSSDType_SPF	= 99,
        kDNSSDType_UINFO	= 100,
        kDNSSDType_UID	= 101,
        kDNSSDType_GID	= 102,
        kDNSSDType_UNSPEC	= 103,
        kDNSSDType_TKEY	= 249,
        kDNSSDType_TSIG	= 250,
        kDNSSDType_IXFR	= 251,
        kDNSSDType_AXFR	= 252,
        kDNSSDType_MAILB	= 253,
        kDNSSDType_MAILA	= 254,
        kDNSSDType_ANY	= 255
    } 	DNSSDRRType;

typedef /* [uuid] */  DECLSPEC_UUID("3B0059E7-5297-4301-9AAB-1522F31EC8A7") 
enum DNSSDError
    {
        kDNSSDError_NoError	= 0,
        kDNSSDError_Unknown	= -65537,
        kDNSSDError_NoSuchName	= -65538,
        kDNSSDError_NoMemory	= -65539,
        kDNSSDError_BadParam	= -65540,
        kDNSSDError_BadReference	= -65541,
        kDNSSDError_BadState	= -65542,
        kDNSSDError_BadFlags	= -65543,
        kDNSSDError_Unsupported	= -65544,
        kDNSSDError_NotInitialized	= -65545,
        kDNSSDError_AlreadyRegistered	= -65547,
        kDNSSDError_NameConflict	= -65548,
        kDNSSDError_Invalid	= -65549,
        kDNSSDError_Firewall	= -65550,
        kDNSSDError_Incompatible	= -65551,
        kDNSSDError_BadInterfaceIndex	= -65552,
        kDNSSDError_Refused	= -65553,
        kDNSSDError_NoSuchRecord	= -65554,
        kDNSSDError_NoAuth	= -65555,
        kDNSSDError_NoSuchKey	= -65556,
        kDNSSDError_NATTraversal	= -65557,
        kDNSSDError_DoubleNAT	= -65558,
        kDNSSDError_BadTime	= -65559,
        kDNSSDError_BadSig	= -65560,
        kDNSSDError_BadKey	= -65561,
        kDNSSDError_Transient	= -65562,
        kDNSSDError_ServiceNotRunning	= -65563,
        kDNSSDError_NATPortMappingUnsupported	= -65564,
        kDNSSDError_NATPortMappingDisabled	= -65565,
        kDNSSDError_NoRouter	= -65566,
        kDNSSDError_PollingMode	= -65567
    } 	DNSSDError;



extern RPC_IF_HANDLE __MIDL_itf_DLLX_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_DLLX_0000_0000_v0_0_s_ifspec;

#ifndef __ITXTRecord_INTERFACE_DEFINED__
#define __ITXTRecord_INTERFACE_DEFINED__

/* interface ITXTRecord */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ITXTRecord;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8FA0889C-5973-4FC9-970B-EC15C925D0CE")
    ITXTRecord : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetValue( 
            /* [in] */ BSTR key,
            /* [in] */ VARIANT value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RemoveValue( 
            /* [in] */ BSTR key) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ContainsKey( 
            /* [in] */ BSTR key,
            /* [retval][out] */ VARIANT_BOOL *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetValueForKey( 
            /* [in] */ BSTR key,
            /* [retval][out] */ VARIANT *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetCount( 
            /* [retval][out] */ ULONG *count) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetKeyAtIndex( 
            /* [in] */ ULONG index,
            /* [retval][out] */ BSTR *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetValueAtIndex( 
            /* [in] */ ULONG index,
            /* [retval][out] */ VARIANT *retval) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ITXTRecordVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITXTRecord * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITXTRecord * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITXTRecord * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITXTRecord * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITXTRecord * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITXTRecord * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITXTRecord * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetValue )( 
            ITXTRecord * This,
            /* [in] */ BSTR key,
            /* [in] */ VARIANT value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RemoveValue )( 
            ITXTRecord * This,
            /* [in] */ BSTR key);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ContainsKey )( 
            ITXTRecord * This,
            /* [in] */ BSTR key,
            /* [retval][out] */ VARIANT_BOOL *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetValueForKey )( 
            ITXTRecord * This,
            /* [in] */ BSTR key,
            /* [retval][out] */ VARIANT *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetCount )( 
            ITXTRecord * This,
            /* [retval][out] */ ULONG *count);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetKeyAtIndex )( 
            ITXTRecord * This,
            /* [in] */ ULONG index,
            /* [retval][out] */ BSTR *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetValueAtIndex )( 
            ITXTRecord * This,
            /* [in] */ ULONG index,
            /* [retval][out] */ VARIANT *retval);
        
        END_INTERFACE
    } ITXTRecordVtbl;

    interface ITXTRecord
    {
        CONST_VTBL struct ITXTRecordVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITXTRecord_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITXTRecord_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITXTRecord_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITXTRecord_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ITXTRecord_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ITXTRecord_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ITXTRecord_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ITXTRecord_SetValue(This,key,value)	\
    ( (This)->lpVtbl -> SetValue(This,key,value) ) 

#define ITXTRecord_RemoveValue(This,key)	\
    ( (This)->lpVtbl -> RemoveValue(This,key) ) 

#define ITXTRecord_ContainsKey(This,key,retval)	\
    ( (This)->lpVtbl -> ContainsKey(This,key,retval) ) 

#define ITXTRecord_GetValueForKey(This,key,value)	\
    ( (This)->lpVtbl -> GetValueForKey(This,key,value) ) 

#define ITXTRecord_GetCount(This,count)	\
    ( (This)->lpVtbl -> GetCount(This,count) ) 

#define ITXTRecord_GetKeyAtIndex(This,index,retval)	\
    ( (This)->lpVtbl -> GetKeyAtIndex(This,index,retval) ) 

#define ITXTRecord_GetValueAtIndex(This,index,retval)	\
    ( (This)->lpVtbl -> GetValueAtIndex(This,index,retval) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITXTRecord_INTERFACE_DEFINED__ */


#ifndef __IDNSSDRecord_INTERFACE_DEFINED__
#define __IDNSSDRecord_INTERFACE_DEFINED__

/* interface IDNSSDRecord */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IDNSSDRecord;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9CE603A0-3365-4DA0-86D1-3F780ECBA110")
    IDNSSDRecord : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Update( 
            /* [in] */ DNSSDFlags flags,
            /* [in] */ VARIANT rdata,
            /* [in] */ ULONG ttl) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Remove( 
            /* [in] */ DNSSDFlags flags) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IDNSSDRecordVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDNSSDRecord * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDNSSDRecord * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDNSSDRecord * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDNSSDRecord * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDNSSDRecord * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDNSSDRecord * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDNSSDRecord * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Update )( 
            IDNSSDRecord * This,
            /* [in] */ DNSSDFlags flags,
            /* [in] */ VARIANT rdata,
            /* [in] */ ULONG ttl);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Remove )( 
            IDNSSDRecord * This,
            /* [in] */ DNSSDFlags flags);
        
        END_INTERFACE
    } IDNSSDRecordVtbl;

    interface IDNSSDRecord
    {
        CONST_VTBL struct IDNSSDRecordVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDNSSDRecord_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDNSSDRecord_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDNSSDRecord_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDNSSDRecord_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDNSSDRecord_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDNSSDRecord_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDNSSDRecord_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDNSSDRecord_Update(This,flags,rdata,ttl)	\
    ( (This)->lpVtbl -> Update(This,flags,rdata,ttl) ) 

#define IDNSSDRecord_Remove(This,flags)	\
    ( (This)->lpVtbl -> Remove(This,flags) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDNSSDRecord_INTERFACE_DEFINED__ */


#ifndef __IDNSSDEventManager_INTERFACE_DEFINED__
#define __IDNSSDEventManager_INTERFACE_DEFINED__

/* interface IDNSSDEventManager */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IDNSSDEventManager;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7FD72324-63E1-45AD-B337-4D525BD98DAD")
    IDNSSDEventManager : public IDispatch
    {
    public:
    };
    
    
#else 	/* C style interface */

    typedef struct IDNSSDEventManagerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDNSSDEventManager * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDNSSDEventManager * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDNSSDEventManager * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDNSSDEventManager * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDNSSDEventManager * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDNSSDEventManager * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDNSSDEventManager * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } IDNSSDEventManagerVtbl;

    interface IDNSSDEventManager
    {
        CONST_VTBL struct IDNSSDEventManagerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDNSSDEventManager_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDNSSDEventManager_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDNSSDEventManager_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDNSSDEventManager_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDNSSDEventManager_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDNSSDEventManager_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDNSSDEventManager_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDNSSDEventManager_INTERFACE_DEFINED__ */


#ifndef __IDNSSDService_INTERFACE_DEFINED__
#define __IDNSSDService_INTERFACE_DEFINED__

/* interface IDNSSDService */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IDNSSDService;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("29DE265F-8402-474F-833A-D4653B23458F")
    IDNSSDService : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EnumerateDomains( 
            /* [in] */ DNSSDFlags flags,
            /* [in] */ ULONG ifIndex,
            /* [in] */ IDNSSDEventManager *eventManager,
            /* [retval][out] */ IDNSSDService **service) = 0;
        
        virtual /* [local][helpstring][id] */ HRESULT STDMETHODCALLTYPE Browse( 
            /* [in] */ DNSSDFlags flags,
            /* [in] */ ULONG interfaceIndex,
            /* [in] */ BSTR regtype,
            /* [in] */ BSTR domain,
            /* [in] */ IDNSSDEventManager *eventManager,
            /* [retval][out] */ IDNSSDService **sdref) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Resolve( 
            /* [in] */ DNSSDFlags flags,
            /* [in] */ ULONG ifIndex,
            /* [in] */ BSTR serviceName,
            /* [in] */ BSTR regType,
            /* [in] */ BSTR domain,
            /* [in] */ IDNSSDEventManager *eventManager,
            /* [retval][out] */ IDNSSDService **service) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Register( 
            /* [in] */ DNSSDFlags flags,
            /* [in] */ ULONG ifIndex,
            /* [in] */ BSTR name,
            /* [in] */ BSTR regType,
            /* [in] */ BSTR domain,
            /* [in] */ BSTR host,
            /* [in] */ USHORT port,
            /* [in] */ ITXTRecord *record,
            /* [in] */ IDNSSDEventManager *eventManager,
            /* [retval][out] */ IDNSSDService **service) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE QueryRecord( 
            /* [in] */ DNSSDFlags flags,
            /* [in] */ ULONG ifIndex,
            /* [in] */ BSTR fullname,
            /* [in] */ DNSSDRRType rrtype,
            /* [in] */ DNSSDRRClass rrclass,
            /* [in] */ IDNSSDEventManager *eventManager,
            /* [retval][out] */ IDNSSDService **service) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RegisterRecord( 
            /* [in] */ DNSSDFlags flags,
            /* [in] */ ULONG ifIndex,
            /* [in] */ BSTR fullname,
            /* [in] */ DNSSDRRType rrtype,
            /* [in] */ DNSSDRRClass rrclass,
            /* [in] */ VARIANT rdata,
            /* [in] */ ULONG ttl,
            /* [in] */ IDNSSDEventManager *eventManager,
            /* [retval][out] */ IDNSSDRecord **record) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddRecord( 
            /* [in] */ DNSSDFlags flags,
            /* [in] */ DNSSDRRType rrtype,
            /* [in] */ VARIANT rdata,
            /* [in] */ ULONG ttl,
            /* [retval][out] */ IDNSSDRecord **record) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ReconfirmRecord( 
            /* [in] */ DNSSDFlags flags,
            /* [in] */ ULONG ifIndex,
            /* [in] */ BSTR fullname,
            /* [in] */ DNSSDRRType rrtype,
            /* [in] */ DNSSDRRClass rrclass,
            /* [in] */ VARIANT rdata) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetProperty( 
            /* [in] */ BSTR prop,
            /* [out][in] */ VARIANT *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetAddrInfo( 
            /* [in] */ DNSSDFlags flags,
            /* [in] */ ULONG ifIndex,
            /* [in] */ DNSSDAddressFamily addressFamily,
            /* [in] */ BSTR hostname,
            /* [in] */ IDNSSDEventManager *eventManager,
            /* [retval][out] */ IDNSSDService **service) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE NATPortMappingCreate( 
            /* [in] */ DNSSDFlags flags,
            /* [in] */ ULONG ifIndex,
            /* [in] */ DNSSDAddressFamily addressFamily,
            /* [in] */ DNSSDProtocol protocol,
            /* [in] */ USHORT internalPort,
            /* [in] */ USHORT externalPort,
            /* [in] */ ULONG ttl,
            /* [in] */ IDNSSDEventManager *eventManager,
            /* [retval][out] */ IDNSSDService **service) = 0;
        
        virtual /* [local][helpstring][id] */ HRESULT STDMETHODCALLTYPE Stop( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IDNSSDServiceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDNSSDService * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDNSSDService * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDNSSDService * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDNSSDService * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDNSSDService * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDNSSDService * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDNSSDService * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EnumerateDomains )( 
            IDNSSDService * This,
            /* [in] */ DNSSDFlags flags,
            /* [in] */ ULONG ifIndex,
            /* [in] */ IDNSSDEventManager *eventManager,
            /* [retval][out] */ IDNSSDService **service);
        
        /* [local][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Browse )( 
            IDNSSDService * This,
            /* [in] */ DNSSDFlags flags,
            /* [in] */ ULONG interfaceIndex,
            /* [in] */ BSTR regtype,
            /* [in] */ BSTR domain,
            /* [in] */ IDNSSDEventManager *eventManager,
            /* [retval][out] */ IDNSSDService **sdref);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Resolve )( 
            IDNSSDService * This,
            /* [in] */ DNSSDFlags flags,
            /* [in] */ ULONG ifIndex,
            /* [in] */ BSTR serviceName,
            /* [in] */ BSTR regType,
            /* [in] */ BSTR domain,
            /* [in] */ IDNSSDEventManager *eventManager,
            /* [retval][out] */ IDNSSDService **service);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Register )( 
            IDNSSDService * This,
            /* [in] */ DNSSDFlags flags,
            /* [in] */ ULONG ifIndex,
            /* [in] */ BSTR name,
            /* [in] */ BSTR regType,
            /* [in] */ BSTR domain,
            /* [in] */ BSTR host,
            /* [in] */ USHORT port,
            /* [in] */ ITXTRecord *record,
            /* [in] */ IDNSSDEventManager *eventManager,
            /* [retval][out] */ IDNSSDService **service);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *QueryRecord )( 
            IDNSSDService * This,
            /* [in] */ DNSSDFlags flags,
            /* [in] */ ULONG ifIndex,
            /* [in] */ BSTR fullname,
            /* [in] */ DNSSDRRType rrtype,
            /* [in] */ DNSSDRRClass rrclass,
            /* [in] */ IDNSSDEventManager *eventManager,
            /* [retval][out] */ IDNSSDService **service);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RegisterRecord )( 
            IDNSSDService * This,
            /* [in] */ DNSSDFlags flags,
            /* [in] */ ULONG ifIndex,
            /* [in] */ BSTR fullname,
            /* [in] */ DNSSDRRType rrtype,
            /* [in] */ DNSSDRRClass rrclass,
            /* [in] */ VARIANT rdata,
            /* [in] */ ULONG ttl,
            /* [in] */ IDNSSDEventManager *eventManager,
            /* [retval][out] */ IDNSSDRecord **record);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddRecord )( 
            IDNSSDService * This,
            /* [in] */ DNSSDFlags flags,
            /* [in] */ DNSSDRRType rrtype,
            /* [in] */ VARIANT rdata,
            /* [in] */ ULONG ttl,
            /* [retval][out] */ IDNSSDRecord **record);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ReconfirmRecord )( 
            IDNSSDService * This,
            /* [in] */ DNSSDFlags flags,
            /* [in] */ ULONG ifIndex,
            /* [in] */ BSTR fullname,
            /* [in] */ DNSSDRRType rrtype,
            /* [in] */ DNSSDRRClass rrclass,
            /* [in] */ VARIANT rdata);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetProperty )( 
            IDNSSDService * This,
            /* [in] */ BSTR prop,
            /* [out][in] */ VARIANT *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetAddrInfo )( 
            IDNSSDService * This,
            /* [in] */ DNSSDFlags flags,
            /* [in] */ ULONG ifIndex,
            /* [in] */ DNSSDAddressFamily addressFamily,
            /* [in] */ BSTR hostname,
            /* [in] */ IDNSSDEventManager *eventManager,
            /* [retval][out] */ IDNSSDService **service);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *NATPortMappingCreate )( 
            IDNSSDService * This,
            /* [in] */ DNSSDFlags flags,
            /* [in] */ ULONG ifIndex,
            /* [in] */ DNSSDAddressFamily addressFamily,
            /* [in] */ DNSSDProtocol protocol,
            /* [in] */ USHORT internalPort,
            /* [in] */ USHORT externalPort,
            /* [in] */ ULONG ttl,
            /* [in] */ IDNSSDEventManager *eventManager,
            /* [retval][out] */ IDNSSDService **service);
        
        /* [local][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Stop )( 
            IDNSSDService * This);
        
        END_INTERFACE
    } IDNSSDServiceVtbl;

    interface IDNSSDService
    {
        CONST_VTBL struct IDNSSDServiceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDNSSDService_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDNSSDService_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDNSSDService_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDNSSDService_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDNSSDService_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDNSSDService_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDNSSDService_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDNSSDService_EnumerateDomains(This,flags,ifIndex,eventManager,service)	\
    ( (This)->lpVtbl -> EnumerateDomains(This,flags,ifIndex,eventManager,service) ) 

#define IDNSSDService_Browse(This,flags,interfaceIndex,regtype,domain,eventManager,sdref)	\
    ( (This)->lpVtbl -> Browse(This,flags,interfaceIndex,regtype,domain,eventManager,sdref) ) 

#define IDNSSDService_Resolve(This,flags,ifIndex,serviceName,regType,domain,eventManager,service)	\
    ( (This)->lpVtbl -> Resolve(This,flags,ifIndex,serviceName,regType,domain,eventManager,service) ) 

#define IDNSSDService_Register(This,flags,ifIndex,name,regType,domain,host,port,record,eventManager,service)	\
    ( (This)->lpVtbl -> Register(This,flags,ifIndex,name,regType,domain,host,port,record,eventManager,service) ) 

#define IDNSSDService_QueryRecord(This,flags,ifIndex,fullname,rrtype,rrclass,eventManager,service)	\
    ( (This)->lpVtbl -> QueryRecord(This,flags,ifIndex,fullname,rrtype,rrclass,eventManager,service) ) 

#define IDNSSDService_RegisterRecord(This,flags,ifIndex,fullname,rrtype,rrclass,rdata,ttl,eventManager,record)	\
    ( (This)->lpVtbl -> RegisterRecord(This,flags,ifIndex,fullname,rrtype,rrclass,rdata,ttl,eventManager,record) ) 

#define IDNSSDService_AddRecord(This,flags,rrtype,rdata,ttl,record)	\
    ( (This)->lpVtbl -> AddRecord(This,flags,rrtype,rdata,ttl,record) ) 

#define IDNSSDService_ReconfirmRecord(This,flags,ifIndex,fullname,rrtype,rrclass,rdata)	\
    ( (This)->lpVtbl -> ReconfirmRecord(This,flags,ifIndex,fullname,rrtype,rrclass,rdata) ) 

#define IDNSSDService_GetProperty(This,prop,value)	\
    ( (This)->lpVtbl -> GetProperty(This,prop,value) ) 

#define IDNSSDService_GetAddrInfo(This,flags,ifIndex,addressFamily,hostname,eventManager,service)	\
    ( (This)->lpVtbl -> GetAddrInfo(This,flags,ifIndex,addressFamily,hostname,eventManager,service) ) 

#define IDNSSDService_NATPortMappingCreate(This,flags,ifIndex,addressFamily,protocol,internalPort,externalPort,ttl,eventManager,service)	\
    ( (This)->lpVtbl -> NATPortMappingCreate(This,flags,ifIndex,addressFamily,protocol,internalPort,externalPort,ttl,eventManager,service) ) 

#define IDNSSDService_Stop(This)	\
    ( (This)->lpVtbl -> Stop(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDNSSDService_INTERFACE_DEFINED__ */



#ifndef __Bonjour_LIBRARY_DEFINED__
#define __Bonjour_LIBRARY_DEFINED__

/* library Bonjour */
/* [helpstring][version][uuid] */ 








EXTERN_C const IID LIBID_Bonjour;

#ifndef ___IDNSSDEvents_DISPINTERFACE_DEFINED__
#define ___IDNSSDEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IDNSSDEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IDNSSDEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("21AE8D7F-D5FE-45cf-B632-CFA2C2C6B498")
    _IDNSSDEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IDNSSDEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IDNSSDEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IDNSSDEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IDNSSDEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IDNSSDEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IDNSSDEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IDNSSDEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IDNSSDEvents * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } _IDNSSDEventsVtbl;

    interface _IDNSSDEvents
    {
        CONST_VTBL struct _IDNSSDEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IDNSSDEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _IDNSSDEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _IDNSSDEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _IDNSSDEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _IDNSSDEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _IDNSSDEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _IDNSSDEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IDNSSDEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_DNSSDService;

#ifdef __cplusplus

class DECLSPEC_UUID("24CD4DE9-FF84-4701-9DC1-9B69E0D1090A")
DNSSDService;
#endif

EXTERN_C const CLSID CLSID_TXTRecord;

#ifdef __cplusplus

class DECLSPEC_UUID("AFEE063C-05BA-4248-A26E-168477F49734")
TXTRecord;
#endif

EXTERN_C const CLSID CLSID_DNSSDRecord;

#ifdef __cplusplus

class DECLSPEC_UUID("5E93C5A9-7516-4259-A67B-41A656F6E01C")
DNSSDRecord;
#endif

EXTERN_C const CLSID CLSID_DNSSDEventManager;

#ifdef __cplusplus

class DECLSPEC_UUID("BEEB932A-8D4A-4619-AEFE-A836F988B221")
DNSSDEventManager;
#endif
#endif /* __Bonjour_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif





/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Fri Apr 10 05:54:14 2020
 */
/* Compiler settings for trustlet_debug.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.00.0603 
    protocol : ndr64 , ms_ext, c_ext, robust
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


#ifndef __trustlet_debug_h__
#define __trustlet_debug_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __TrustletDebug_INTERFACE_DEFINED__
#define __TrustletDebug_INTERFACE_DEFINED__

/* interface TrustletDebug */
/* [version][uuid] */ 

typedef /* [public][public] */ struct __MIDL_TrustletDebug_0001
    {
    unsigned long BufferLength;
    /* [size_is][unique] */ byte *Buffer;
    } 	IDENTY_KEY;

void Terminate( 
    /* [in] */ handle_t IDL_handle,
    /* [in] */ unsigned long Code);

error_status_t GetIdentyKey( 
    /* [in] */ handle_t IDL_handle,
    /* [out] */ IDENTY_KEY *pKey);



extern RPC_IF_HANDLE TrustletDebug_v1_0_c_ifspec;
extern RPC_IF_HANDLE TrustletDebug_v1_0_s_ifspec;
#endif /* __TrustletDebug_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif



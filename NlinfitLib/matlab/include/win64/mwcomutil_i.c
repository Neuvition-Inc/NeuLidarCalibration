

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Mon Jan 18 22:14:07 2038
 */
/* Compiler settings for win64\mwcomutil.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=IA64 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        EXTERN_C __declspec(selectany) const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif // !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IMWUtil,0xC47EA90E,0x56D1,0x11d5,0xB1,0x59,0x00,0xD0,0xB7,0xBA,0x75,0x44);


MIDL_DEFINE_GUID(IID, LIBID_MWComUtil,0xF2B62F8A,0x4A20,0x4D45,0xA8,0xE0,0xFA,0x61,0x93,0x4B,0x8C,0x0D);


MIDL_DEFINE_GUID(CLSID, CLSID_MWField,0xD98C2A06,0x8FD5,0x476F,0x99,0xD8,0xA2,0x27,0xFA,0x42,0x30,0x12);


MIDL_DEFINE_GUID(CLSID, CLSID_MWStruct,0x5935632C,0x927E,0x4EAB,0xB8,0x28,0x9C,0xCC,0xE3,0xEA,0xFF,0xEE);


MIDL_DEFINE_GUID(CLSID, CLSID_MWComplex,0xD452AD82,0xF797,0x4BD8,0xB0,0xFF,0x55,0xFF,0x5C,0x26,0xD0,0x8A);


MIDL_DEFINE_GUID(CLSID, CLSID_MWSparse,0xC748A3CC,0x7936,0x4CBE,0xAB,0x73,0xEA,0xA8,0x51,0x57,0x31,0x90);


MIDL_DEFINE_GUID(CLSID, CLSID_MWArg,0x18E13726,0x34F7,0x4A22,0x85,0x85,0xAC,0xFF,0xA2,0xC6,0x55,0x95);


MIDL_DEFINE_GUID(CLSID, CLSID_MWArrayFormatFlags,0x9C4AA7E2,0x7FDD,0x477C,0x9D,0x78,0xA4,0x0F,0x62,0xBD,0x98,0x90);


MIDL_DEFINE_GUID(CLSID, CLSID_MWDataConversionFlags,0x1D7ADAFE,0x3A73,0x4FA6,0xBC,0x98,0xC6,0xF7,0xAC,0x1C,0x36,0xC9);


MIDL_DEFINE_GUID(CLSID, CLSID_MWUtil,0x27463FB9,0xA79A,0x4B39,0x8B,0x63,0x17,0x83,0x8C,0x33,0x86,0x70);


MIDL_DEFINE_GUID(CLSID, CLSID_MWFlags,0x152396A2,0x8C4D,0x4FEE,0xAF,0x50,0x40,0x69,0x4C,0x1E,0x02,0x9F);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif




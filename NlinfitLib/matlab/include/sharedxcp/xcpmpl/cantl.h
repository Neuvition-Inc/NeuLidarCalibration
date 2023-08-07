/* Copyright 2012-2017 The MathWorks, Inc. */

#ifndef CANTL_H
#define CANTL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#if defined(MATLAB_MEX_FILE)
    #define CANAPI __declspec(dllimport)
#elif defined(XCP_DLL_EXPORT)
    #define CANAPI __declspec(dllexport)
#else
    #define CANAPI
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef void * HCTL;

CANAPI HCTL     ctlGetHandle(const char* tag);
CANAPI void     ctlReleaseHandle(const char* tag);
CANAPI int32_t  ctlExecuteTx(HCTL handle, void* out_buf, uint32_t buf_size);
CANAPI int32_t  ctlExecuteRx(HCTL handle, void* in_buf,  uint32_t rx_count);

#ifdef __cplusplus
}
#endif

#endif


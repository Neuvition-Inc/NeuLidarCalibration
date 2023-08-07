//
// MATLAB Compiler: 8.2 (R2021a)
// Date: Wed Jan  4 09:10:04 2023
// Arguments:
// "-B""macro_default""-W""cpplib:NlinfitSolve""-T""link:lib""NlinfitSolve.m"
//

#ifndef NlinfitSolve_h
#define NlinfitSolve_h 1

#if defined(__cplusplus) && !defined(mclmcrrt_h) && defined(__linux__)
#  pragma implementation "mclmcrrt.h"
#endif
#include "mclmcrrt.h"
#include "mclcppclass.h"
#ifdef __cplusplus
extern "C" { // sbcheck:ok:extern_c
#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_NlinfitSolve_C_API 
#define LIB_NlinfitSolve_C_API /* No special import/export declaration */
#endif

/* GENERAL LIBRARY FUNCTIONS -- START */

extern LIB_NlinfitSolve_C_API 
bool MW_CALL_CONV NlinfitSolveInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_NlinfitSolve_C_API 
bool MW_CALL_CONV NlinfitSolveInitialize(void);

extern LIB_NlinfitSolve_C_API 
void MW_CALL_CONV NlinfitSolveTerminate(void);

extern LIB_NlinfitSolve_C_API 
void MW_CALL_CONV NlinfitSolvePrintStackTrace(void);

/* GENERAL LIBRARY FUNCTIONS -- END */

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

extern LIB_NlinfitSolve_C_API 
bool MW_CALL_CONV mlxNlinfitSolve(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */

#ifdef __cplusplus
}
#endif


/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__MINGW64__)

#ifdef EXPORTING_NlinfitSolve
#define PUBLIC_NlinfitSolve_CPP_API __declspec(dllexport)
#else
#define PUBLIC_NlinfitSolve_CPP_API __declspec(dllimport)
#endif

#define LIB_NlinfitSolve_CPP_API PUBLIC_NlinfitSolve_CPP_API

#else

#if !defined(LIB_NlinfitSolve_CPP_API)
#if defined(LIB_NlinfitSolve_C_API)
#define LIB_NlinfitSolve_CPP_API LIB_NlinfitSolve_C_API
#else
#define LIB_NlinfitSolve_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_NlinfitSolve_CPP_API void MW_CALL_CONV NlinfitSolve(int nargout, mwArray& bt, mwArray& MSE, const mwArray& X);

/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */
#endif

#endif

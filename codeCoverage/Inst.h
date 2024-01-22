#pragma once

#if !defined(LIB_EXPORT)
#if defined(_MSC_VER)
#define LIB_EXPORT __declspec(dllexport)
#else
#define LIB_EXPORT __attribute__((visibility("default")))
#endif
#endif

LIB_EXPORT void initCoverage(int, int);
LIB_EXPORT void registerFunc(int, char const*, char const*);
LIB_EXPORT void registerBB(int, char const*, char const*, unsigned long);
LIB_EXPORT void incFuncCoverage(int);
LIB_EXPORT void incBBCoverage(int);
LIB_EXPORT void exitCoverage(int, int, int);

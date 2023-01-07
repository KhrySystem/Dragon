#pragma once


/* Other compilers that emulate this one need to be detected first. */

#include "clang.h"

#include "../version_number.h"
#include "../make.h"

/* tag::reference[]
= `BOOST_COMP_LLVM`
http://en.wikipedia.org/wiki/LLVM[LLVM] compiler.
[options="header"]
|===
| {predef_symbol} | {predef_version}
| `+__llvm__+` | {predef_detection}
|===
*/ // end::reference[]

#define BOOST_COMP_LLVM BOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__llvm__)
    #define BOOST_COMP_LLVM_DETECTION BOOST_VERSION_NUMBER_AVAILABLE
#endif

#ifdef BOOST_COMP_LLVM_DETECTION
    #if defined(BOOST_PREDEF_DETAIL_COMP_DETECTED)
        #define BOOST_COMP_LLVM_EMULATED BOOST_COMP_LLVM_DETECTION
    #else
        #undef BOOST_COMP_LLVM
        #define BOOST_COMP_LLVM BOOST_COMP_LLVM_DETECTION
    #endif
    #define BOOST_COMP_LLVM_AVAILABLE
    #include "../detail/comp_detected.h"
#endif

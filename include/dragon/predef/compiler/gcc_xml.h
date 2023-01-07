#pragma once

#include "../version_number.h"
#include "../make.h"

/* tag::reference[]
= `BOOST_COMP_GCCXML`
http://www.gccxml.org/[GCC XML] compiler.
[options="header"]
|===
| {predef_symbol} | {predef_version}
| `+__GCCXML__+` | {predef_detection}
|===
*/ // end::reference[]

#define BOOST_COMP_GCCXML BOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__GCCXML__)
    #define BOOST_COMP_GCCXML_DETECTION BOOST_VERSION_NUMBER_AVAILABLE
#endif

#ifdef BOOST_COMP_GCCXML_DETECTION
    #if defined(BOOST_PREDEF_DETAIL_COMP_DETECTED)
        #define BOOST_COMP_GCCXML_EMULATED BOOST_COMP_GCCXML_DETECTION
    #else
        #undef BOOST_COMP_GCCXML
        #define BOOST_COMP_GCCXML BOOST_COMP_GCCXML_DETECTION
    #endif
    #define BOOST_COMP_GCCXML_AVAILABLE
    #include "../detail/comp_detected.h"
#endif
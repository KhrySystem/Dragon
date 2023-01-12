#pragma once

/*
Copyright Rene Rivera 2011-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include "../version_number.h"
#include "../make.h"

/* tag::reference[]
= `BOOST_LANG_STDCPP`

http://en.wikipedia.org/wiki/C%2B%2B[Standard {CPP}] language.
If available, the year of the standard is detected as YYYY.MM.1 from the Epoch date.
Because of the way the {CPP} standardization process works the
defined version year will not be the commonly known year of the standard.
Specifically the defined versions are:

.Detected Version Number vs. {CPP} Standard Year
[options="header"]
|===
| Detected Version Number | Standard Year | {CPP} Standard
| 27.11.1 | 1998 | ISO/IEC 14882:1998
| 41.3.1 | 2011 | ISO/IEC 14882:2011
| 44.2.1 | 2014 | ISO/IEC 14882:2014
| 47.3.1 | 2017 | ISO/IEC 14882:2017
|===

[options="header"]
|===
| {predef_symbol} | {predef_version}

| `+__cplusplus+` | {predef_detection}

| `+__cplusplus+` | YYYY.MM.1
|===
*/ // end::reference[]

#define BOOST_LANG_STDCPP BOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__cplusplus)
#   undef BOOST_LANG_STDCPP
#   if (__cplusplus > 100)
#       define BOOST_LANG_STDCPP BOOST_PREDEF_MAKE_YYYYMM(__cplusplus)
#   else
#       define BOOST_LANG_STDCPP BOOST_VERSION_NUMBER_AVAILABLE
#   endif
#endif

#if BOOST_LANG_STDCPP
#   define BOOST_LANG_STDCPP_AVAILABLE
#endif

/* tag::reference[]
= `BOOST_LANG_STDCPPCLI`

http://en.wikipedia.org/wiki/C%2B%2B/CLI[Standard {CPP}/CLI] language.
If available, the year of the standard is detected as YYYY.MM.1 from the Epoch date.

[options="header"]
|===
| {predef_symbol} | {predef_version}

| `+__cplusplus_cli+` | {predef_detection}

| `+__cplusplus_cli+` | YYYY.MM.1
|===
*/ // end::reference[]

#define BOOST_LANG_STDCPPCLI BOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__cplusplus_cli)
#   undef BOOST_LANG_STDCPPCLI
#   if (__cplusplus_cli > 100)
#       define BOOST_LANG_STDCPPCLI BOOST_PREDEF_MAKE_YYYYMM(__cplusplus_cli)
#   else
#       define BOOST_LANG_STDCPPCLI BOOST_VERSION_NUMBER_AVAILABLE
#   endif
#endif

#if BOOST_LANG_STDCPPCLI
#   define BOOST_LANG_STDCPPCLI_AVAILABLE
#endif
/* tag::reference[]
= `BOOST_LANG_STDECPP`

http://en.wikipedia.org/wiki/Embedded_C%2B%2B[Standard Embedded {CPP}] language.

[options="header"]
|===
| {predef_symbol} | {predef_version}

| `+__embedded_cplusplus+` | {predef_detection}
|===
*/ // end::reference[]

#define BOOST_LANG_STDECPP BOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__embedded_cplusplus)
#   undef BOOST_LANG_STDECPP
#   define BOOST_LANG_STDECPP BOOST_VERSION_NUMBER_AVAILABLE
#endif

#if BOOST_LANG_STDECPP
#   define BOOST_LANG_STDECPP_AVAILABLE
#endif
#pragma once

/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include "../version_number.h"
#include "../make.h"


/* tag::reference[]
= `BOOST_OS_LINUX`

http://en.wikipedia.org/wiki/Linux[Linux] operating system.

[options="header"]
|===
| {predef_symbol} | {predef_version}

| `linux` | {predef_detection}
| `+__linux+` | {predef_detection}
| `+__linux__+` | {predef_detection}
| `+__gnu_linux__+` | {predef_detection}
|===
*/ // end::reference[]

#define BOOST_OS_LINUX BOOST_VERSION_NUMBER_NOT_AVAILABLE

#if !defined(BOOST_PREDEF_DETAIL_OS_DETECTED) && ( \
    defined(linux) || defined(__linux) || \
    defined(__linux__) || defined(__gnu_linux__) \
    )
#   undef BOOST_OS_LINUX
#   define BOOST_OS_LINUX BOOST_VERSION_NUMBER_AVAILABLE
#endif

#if BOOST_OS_LINUX
#   define BOOST_OS_LINUX_AVAILABLE
    #include "../detail/os_detected.h"
#endif
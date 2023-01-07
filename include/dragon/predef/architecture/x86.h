#pragma once

#include "../version_number.h"
#include "../make.h"

#define BOOST_ARCH_X86 BOOST_VERSION_NUMBER_NOT_AVAILABLE

#if BOOST_ARCH_X86_32 || BOOST_ARCH_X86_64
#   undef BOOST_ARCH_X86
#   define BOOST_ARCH_X86 BOOST_VERSION_NUMBER_AVAILABLE
#endif

#if BOOST_ARCH_X86
#   define BOOST_ARCH_X86_AVAILABLE
#endif

#define BOOST_ARCH_X86_NAME "Intel x86"
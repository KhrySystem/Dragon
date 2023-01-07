#pragma once

#include "../version_number.h"
#include "../make.h"

#define BOOST_ARCH_ARM BOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__ARM_ARCH) || defined(__TARGET_ARCH_ARM) || defined(__TARGET_ARCH_THUMB) || defined(_M_ARM) || defined(__arm__) || defined(__arm64) || defined(__thumb__) || defined(_M_ARM64) || defined(__aarch64__) || defined(__AARCH64EL__) || defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6KZ__) || defined(__ARM_ARCH_6T2__) || defined(__ARM_ARCH_5TE__) || defined(__ARM_ARCH_5TEJ__) || defined(__ARM_ARCH_4T__) || defined(__ARM_ARCH_4__)
    #undef BOOST_ARCH_ARM
    #if !defined(BOOST_ARCH_ARM) && defined(__ARM_ARCH)
        #define BOOST_ARCH_ARM BOOST_VERSION_NUMBER(__ARM_ARCH,0,0)
    #endif
    #if !defined(BOOST_ARCH_ARM) && defined(__TARGET_ARCH_ARM)
        #define BOOST_ARCH_ARM BOOST_VERSION_NUMBER(__TARGET_ARCH_ARM,0,0)
    #endif
    #if !defined(BOOST_ARCH_ARM) && defined(__TARGET_ARCH_THUMB)
        #define BOOST_ARCH_ARM BOOST_VERSION_NUMBER(__TARGET_ARCH_THUMB,0,0)
    #endif
    #if !defined(BOOST_ARCH_ARM) && defined(_M_ARM)
        #define BOOST_ARCH_ARM BOOST_VERSION_NUMBER(_M_ARM,0,0)
    #endif
    #if !defined(BOOST_ARCH_ARM) && (defined(__arm64) || defined(_M_ARM64) || defined(__aarch64__) || defined(__AARCH64EL__) )
        #define BOOST_ARCH_ARM BOOST_VERSION_NUMBER(8,0,0)
    #endif
    #if !defined(BOOST_ARCH_ARM) && ( defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) )
        #define BOOST_ARCH_ARM BOOST_VERSION_NUMBER(7,0,0)
    #endif
    #if !defined(BOOST_ARCH_ARM) && ( defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6KZ__) || defined(__ARM_ARCH_6T2__) )
        #define BOOST_ARCH_ARM BOOST_VERSION_NUMBER(6,0,0)
    #endif
    #if !defined(BOOST_ARCH_ARM) && ( defined(__ARM_ARCH_5TE__) || defined(__ARM_ARCH_5TEJ__) )
        #define BOOST_ARCH_ARM BOOST_VERSION_NUMBER(5,0,0)
    #endif
    #if !defined(BOOST_ARCH_ARM) && ( defined(__ARM_ARCH_4T__) || defined(__ARM_ARCH_4__) )
        #define BOOST_ARCH_ARM BOOST_VERSION_NUMBER(4,0,0)
    #endif
    #if !defined(BOOST_ARCH_ARM)
        #define BOOST_ARCH_ARM BOOST_VERSION_NUMBER_AVAILABLE
    #endif
#endif

#if BOOST_ARCH_ARM
    #define BOOST_ARCH_ARM_AVAILABLE
#endif

#if BOOST_ARCH_ARM
    #if BOOST_ARCH_ARM >= BOOST_VERSION_NUMBER(8,0,0)
        #undef BOOST_ARCH_WORD_BITS_64
        #define BOOST_ARCH_WORD_BITS_64 BOOST_VERSION_NUMBER_AVAILABLE
    #else
        #undef BOOST_ARCH_WORD_BITS_32
        #define BOOST_ARCH_WORD_BITS_32 BOOST_VERSION_NUMBER_AVAILABLE
    #endif
#endif
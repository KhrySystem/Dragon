#pragma once

#include "platform.h"
#include "../internal/vector.h"

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <fenv.h>
#include <float.h>
#include <inttypes.h>
#include <iso646.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#if !defined(DG_PLAT_WINDOWS)
	#include <pthread.h>
#endif
#include <time.h>
#include <uchar.h>
#include <wchar.h>
#include <wctype.h>

/*****************************************************************************
 * $Id: gfx_os.h,v 1.22 2004/03/11 17:05:19 jimb Exp $
 * Author:	Jim E. Brooks  http://www.jimbrooks.org
 * Desc:	Operating-system specific header file.
 * Notes:
 * LEGAL:	COPYRIGHT (C) JIM E. BROOKS 2004
 *          THIS SOURCE CODE IS RELEASED UNDER THE TERMS
 *          OF THE GNU GENERAL PUBLIC LICENSE (GPL).
 *****************************************************************************/

#ifndef GFX_OS_H
#define GFX_OS_H 1

#if defined(__BEOS__)	/* BeOS */
#include <OS.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#elif defined(_WIN32)	/* Win32 */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/timeb.h>
#include <fcntl.h>
#include <io.h>
#else					/* GNU (default) */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/times.h>
#include <fcntl.h>
#include <limits.h>
#endif

// Universal header files.
#include <math.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
// disable data conversion warnings
#pragma warning(disable : 4244)     // MIPS
#pragma warning(disable : 4136)     // X86
#pragma warning(disable : 4305)     // VC++ 6 truncation warning
#pragma warning(disable : 4051)     // ALPHA
#pragma warning(disable : 4800)		// bool vs. int
void Win32CreateConsole( void );
#endif /* _WIN32 */

#if __GNUC__
#define GCC_VERSION (__GNUC__*10000 + __GNUC_MINOR__*100 + __GNUC_PATCHLEVEL__)
#else
#define GCC_VERSION 0 /* if nothing will cause error on Visual C++ */
#endif

#if 0
#if defined(__GNUC__) && GCC_VERSION >= 30000 && GCC_VERSION < 30100
#error "gcc-3.0.x cannot compile this program correctly"
#error "Compile with 2.9.x or 3.1.x or later."
#endif
#endif

//------------------------------------------------------------------------------

void
Snooze( unsigned int milsec );

#endif /* GFX_OS_H */

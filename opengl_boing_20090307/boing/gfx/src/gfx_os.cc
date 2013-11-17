/*****************************************************************************
 * $Id: gfx_os.cc,v 1.7 2004/02/27 16:30:30 jimb Exp $
 * Author:	Jim E. Brooks  http://www.jimbrooks.org
 * Desc:	Operating-system specific functions.
 * Notes:
 * LEGAL:	COPYRIGHT (C) JIM E. BROOKS 2004
 *          THIS SOURCE CODE IS RELEASED UNDER THE TERMS
 *          OF THE GNU GENERAL PUBLIC LICENSE (GPL).
 *****************************************************************************/

#define GFX_OS_CC 1
#include "gfx.h"

/*****************************************************************************
 * Relinquish CPU for specified milli-seconds.
 *****************************************************************************/
void
Snooze( unsigned int milsec )
{
#if defined(__BEOS__)			// BeOS
	snooze( milsec * 1000 );
#elif defined(_WIN32)			// Win32
	Sleep( milsec );
#else							// GNU (default)
	usleep( milsec * 1000 );
#endif
	return;
}

/*****************************************************************************
 * (Win32) Create a console so that printf() will work.
 *****************************************************************************/
#ifdef _WIN32
void
Win32CreateConsole( void )
{
	int		hCrt;
	FILE*	hf;
	AllocConsole();
	hCrt = _open_osfhandle( (long) GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT );
	hf = _fdopen( hCrt, "w" );
	*stdout = *hf;
	setvbuf( stdout, NULL, _IONBF, 0 );
}
#endif /* _WIN32 */

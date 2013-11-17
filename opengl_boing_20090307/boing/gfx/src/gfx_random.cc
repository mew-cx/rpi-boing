/******************************************************************************
 * $Id: gfx_random.cc,v 1.13 2004/04/07 00:44:54 jimb Exp $
 * Author:	Jim E. Brooks  http://www.jimbrooks.org
 * Desc:	Random number functions.
 * Notes:
 * LEGAL:	COPYRIGHT (C) JIM E. BROOKS 2004
 *          THIS SOURCE CODE IS RELEASED UNDER THE TERMS
 *          OF THE GNU GENERAL PUBLIC LICENSE (GPL).
 ******************************************************************************/

#define GFX_RANDOM_CC 1
#include "gfx.h"

// --- ANSI C rand() functions are used.  Avoid mixing BSD random() functions. ---

/*****************************************************************************
 * Seed the sequence of psuedo random numbers.
 *****************************************************************************/
void
randomize( unsigned int seed )
{
	srand( seed );
}
void
randomize( void )
{
	randomize( random_true() );
}

/*****************************************************************************
 * Return a psuedo random number in range {0,..,RAND_MAX} or {0,..,scale}.
 * "ui" means unsigned int.
 *****************************************************************************/
unsigned int
random_ui( void )
{
	return rand();
}
unsigned int
random_ui( unsigned int scale )
{
	return rand() % (scale+1);
}

/*****************************************************************************
 * Return a psuedo random float in range {0,..,1} or {0,..,scale}.
 *****************************************************************************/
float
random_f( void )
{
	return float(rand()) / float(RAND_MAX);
}
float
random_f( float scale )
{
	return float(rand()) / float(RAND_MAX) * scale;
}

/*****************************************************************************
 * Return a true random unsigned integer.
 * NOTE: this function is slow.
 * An alternative is to call randomize( random_true() ) once and thereafter
 * call the pseudo random function.s
 *****************************************************************************/
unsigned int
random_true( void )
{
#ifndef linux
	srand( (unsigned int)time(NULL) );
	return rand();
#else
	static int fd = -1;
	unsigned int val32 = 0;
	if ( fd == -1 ) fd = open( "/dev/random", O_RDONLY|O_NONBLOCK );
	if ( fd == -1 || read( fd, &val32, sizeof(val32) ) != sizeof(val32) )
	{
		// Failed to open /dev/random.  Try an alternative.
		DebugMsg( "can't open or read /dev/random \n" );
		srand( (unsigned int)time(NULL) );
		val32 = rand();
	}
	return val32;
#endif
}

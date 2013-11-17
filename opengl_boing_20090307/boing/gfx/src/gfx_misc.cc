/******************************************************************************
 * $Id: gfx_misc.cc,v 1.9 2004/03/09 05:40:47 jimb Exp $
 * Author:	Jim E. Brooks  http://www.jimbrooks.org
 * Desc:	Misc functions.
 * Notes:
 * LEGAL:	COPYRIGHT (C) JIM E. BROOKS 2004
 *          THIS SOURCE CODE IS RELEASED UNDER THE TERMS
 *          OF THE GNU GENERAL PUBLIC LICENSE (GPL).
 ******************************************************************************/

#define GFX_MISC_CC 1
#include "gfx.h"

/*****************************************************************************
 * Self-check.  An executable's main() should call this and exit if this fails.
 * Ensure assumptions of this code hold on this platform.
 *****************************************************************************/
bool
SelfCheck( void )
{
	if ( sizeof(int) < 4 )
	{
		ErrorMsg( "sizeof(int) < 4 \n" );
		return FALSE;
	}

#if GFXSYS_OPENGL
	if ( sizeof(float) != sizeof(GLfloat) )
	{
		ErrorMsg( "sizeof(float) != sizeof(GLfloat) \n" );
		return FALSE;
	}
	if ( sizeof(double) != sizeof(GLdouble) )
	{
		ErrorMsg( "sizeof(double) != sizeof(GLdouble) \n" );
		return FALSE;
	}
#endif

#if 0 // not necessary for Boing, causes compiler warnings [2007/05]
#if ! ALTVERTEX
#	if TYPESIGS
	struct vertexEye_ { fp x; fp y; fp z; };
#	else
	typedef vertexEye vertexEye_;
#	endif
	// Test if vertexEye and fp[3] are equivalent.
	vertexEye_	v;
	fp			ar[3];
	v.x = ar[0] =  1234.0;
	v.y = ar[1] = -5678.0;
	v.z = ar[2] = -1234567890.0; 
	if (    sizeof(ar) != sizeof(v)
         || ((vertexEye_*)ar)->x     != v.x   // x vs. [0]
         || ((fp*)&v)[1]            != ar[1] // y vs. [1]
         || ((vertexEye_*)&ar[0])->z != v.z ) // z vs. [2]
	{
		ErrorMsg( "struct vertexEye != fp[3] \n" );
		ErrorMsg( "Compiling with ALTVERTEX=1 is required on this system. \n" );
		ErrorMsg( "make ALTVERTEX=1 \n" );
		ErrorMsg( "nmake -f makefile.w32 \"ALTVERTEX = 1 \" (Win32)\n" );
		ErrorMsg( "Or avoid FPDOUBLE=1 or FPLONGDOUBLE=1 \n" );
		return FALSE;
	}
#endif /* ! ALTVERTEX */
#endif // 0

	// Warn about unpacked structs (packed structs conserve memory).
	if ( sizeof(Dlink<int>) != sizeof(void*)*3 + 1 )
	{
		DebugMsg( "structs that ideally should be packed aren't (ok though) \n" );
		// keep going
	}

	// Ok.
	return TRUE;
}

/*****************************************************************************
 * Safely convert a cmd-line arg to a number.
 * Returns zero if arg idx is invalid.
 *****************************************************************************/
int
Argi( int idx, int argc_, char** argv_ )
{
	if ( idx >= argc_ ) return 0;
	return atoi( argv_[idx] );
}
float
Argf( int idx, int argc_, char** argv_ )
{
	if ( idx >= argc_ ) return 0;
	return atof( argv_[idx] );
}

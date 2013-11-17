/*****************************************************************************
 * $Id: gfx_misc.h,v 1.3 2004/02/27 16:30:30 jimb Exp $
 * Author:	Jim E. Brooks  http://www.jimbrooks.org
 * Desc:	Misc functions.
 * Notes:
 * LEGAL:	COPYRIGHT (C) JIM E. BROOKS 2004
 *          THIS SOURCE CODE IS RELEASED UNDER THE TERMS
 *          OF THE GNU GENERAL PUBLIC LICENSE (GPL).
 *****************************************************************************/

#ifndef GFX_MISC_H
#define GFX_MISC_H 1

bool
SelfCheck( void );

int
Argi( int idx, int argc_, char** argv_ );

float
Argf( int idx, int argc_, char** argv_ );

#endif /* GFX_MISC_H */

/******************************************************************************
 * $Id: gfx_random.h,v 1.8 2004/04/06 22:00:50 jimb Exp $
 * Author:	Jim E. Brooks  http://www.jimbrooks.org
 * Desc:	Random number functions.
 * Notes:
 * LEGAL:	COPYRIGHT (C) JIM E. BROOKS 2004
 *          THIS SOURCE CODE IS RELEASED UNDER THE TERMS
 *          OF THE GNU GENERAL PUBLIC LICENSE (GPL).
 ******************************************************************************/

#ifndef GFX_RANDOM_H
#define GFX_RANDOM_H 1

void
randomize( unsigned int seed );

void
randomize( void );

unsigned int
random_ui( void );

unsigned int
random_ui( unsigned int scale );

float
random_f( void );

float
random_f( float scale );

unsigned int
random_true( void );

#endif /* GFX_RANDOM_H */

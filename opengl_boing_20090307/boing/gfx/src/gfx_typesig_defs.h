/*****************************************************************************
 * $Id: gfx_typesig_defs.h,v 1.9 2004/03/13 19:08:58 jimb Exp $
 * Author:	Jim E. Brooks  http://www.jimbrooks.org
 * Desc:	Type signature definitions.
 * Notes:
 * LEGAL:	COPYRIGHT (C) JIM E. BROOKS 2004
 *          THIS SOURCE CODE IS RELEASED UNDER THE TERMS
 *          OF THE GNU GENERAL PUBLIC LICENSE (GPL).
 *****************************************************************************/

#ifndef GFX_TYPESIG_DEFS_H
#define GFX_TYPESIG_DEFS_H 1

typedef unsigned char typesig_t; // one byte to conserve memory

// Code outside of this library should add TYPESIG_USER to its enums.
// Enums below TYPESIG_USER are reserved for the gfx library.
enum
{
	// ===================================================
	// enums reserved for gfx library
	// ---------------------------------------------------
	TYPESIG_INVALID         = 0,

	TYPESIG_LIB_DLINK		= 2, // skip 0,1 which aren't distinct values
	TYPESIG_LIB_DLIST,
	TYPESIG_LIB_CACHE_OPQ,
	TYPESIG_LIB_CACHE_LINK,
	TYPESIG_LIB_CACHE,
	TYPESIG_LIB_ITER,
	TYPESIG_LIB_VERTEX_LOCAL,
	TYPESIG_LIB_VERTEX_WORLD,
	TYPESIG_LIB_VERTEX_EYE,
	TYPESIG_LIB_VERTEX_NORMAL,
	TYPESIG_LIB_NORMAL_WORLD,
	TYPESIG_LIB_POLYGON3,
	// ===================================================

	// ===================================================
	// enums >= TYPESIG_USER are for general use elsewhere
	// ---------------------------------------------------
	TYPESIG_USER = 128,
	// ===================================================
};

#endif /* GFX_TYPESIG_DEFS_H */

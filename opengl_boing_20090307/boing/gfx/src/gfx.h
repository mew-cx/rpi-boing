/*****************************************************************************
 * $Id: gfx.h,v 1.20 2004/03/07 22:59:44 jimb Exp $
 * Author:	Jim E. Brooks  http://www.jimbrooks.org
 * Desc:	Main header file.
 * Notes:
 * LEGAL:	COPYRIGHT (C) JIM E. BROOKS 2004
 *          THIS SOURCE CODE IS RELEASED UNDER THE TERMS
 *          OF THE GNU GENERAL PUBLIC LICENSE (GPL).
 *****************************************************************************/

#ifndef GFX_H
#define GFX_H 1

// Which graphics system.  Default is OpenGL.
#if !defined(GFXSYS_OPENGL)
#define GFXSYS_OPENGL 1
#endif

// Include graphics system header files.
#if GFXSYS_OPENGL
#	ifdef __APPLE_CC__
#		include <GLUT/glut.h>
#   else
#		include <GL/glut.h>
#   endif
#include <GL/gl.h>
#else
#error 
#endif

#include "gfx_os.h"
#include "gfx_define.h"
#include "gfx_assert.h"
#include "gfx_typesig_defs.h"
#include "gfx_typesig.h"
#include "gfx_types.h"
#include "gfx_msg.h"
#if GFXSYS_OPENGL
#include "gfx_opengl.h"
#endif
#include "gfx_math.h"
#include "gfx_math_inline.h"
#include "gfx_list.h"
#include "gfx_random.h"
#include "gfx_rgb.h"
#include "gfx_misc.h"
#include "gfx_iter.h"
#include "gfx_cache.h"

#include <limits.h>
#if defined(INT_MAX) && INT_MAX < 2147483647
#error "Need 32-bit integer machine or better"
#endif

#endif /* GFX_H */

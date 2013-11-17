/******************************************************************************
 * $Id: gfx_opengl.cc,v 1.7 2004/03/03 16:06:54 jimb Exp $
 * Author:	Jim E. Brooks  http://www.jimbrooks.org
 * Desc:	Functions/macros for OpenGL.
 * Notes:
 * LEGAL:	COPYRIGHT (C) JIM E. BROOKS 2004
 *          THIS SOURCE CODE IS RELEASED UNDER THE TERMS
 *          OF THE GNU GENERAL PUBLIC LICENSE (GPL).
 ******************************************************************************/

#define GFX_OPENGL_CC 1
#include "gfx.h"
#if GFXSYS_OPENGL

/*****************************************************************************
 * Calculate the angle to be passed to gluPerspective() so that a scene
 * is visible.  This function originates from the OpenGL Red Book.
 *
 * Parms   : size
 *           The size of the segment when the angle is intersected at "dist"
 *           (ie at the outermost edge of the angle of vision).
 *
 *           dist
 *           Distance from viewpoint to scene.
 *****************************************************************************/
fp
PerspectiveAngle( fp size, fp dist )
{
	GLfloat radTheta, degTheta;
	radTheta = 2.0 * atan2( size / 2.0, dist );
	degTheta = (180.0 * radTheta) / M_PI;
	return degTheta;
}

/*****************************************************************************
 * Draw a triangle strip.
 *
 * NOTE: Caller must swap vertex arrays during successive calls
 *       (if strips are adjacent) or stitching may occur.
 *
 *       Will malfunction on very small arrays (len<4 or so).
 *
 * Strip is drawn using two arrays of vertexes which are offest in one coord
 * (they are parallel).  This is an alternative to drawing as a quad strip.
 * An optional increment value controls LOD (pass 1 for max. LOD).
 *****************************************************************************/
void
DrawTriangleStrip( const vertexEye* va,
                   const vertexEye* vb,
                   int idxEnd,
                   int inc )
{
assert( idxEnd > 4 ); // TODO contigency or correction

	GFX_BEGIN( GP_TRIANGLE_STRIP );

	// Vertexes for an OpenGL triangle strip should be submitted in this order:
	// top   : 0 2 4 6 8
	// bottom: 1 3 5 7 9

	// Start with first vertex of both arrays.
	GFX_VERTEXV( va );
	GFX_VERTEXV( vb );

	// For each alternative vertex in either array,
	// excluding the first and last.
	const vertexEye* pa = va + inc;
	const vertexEye* pb = vb + inc;
	for ( ; pa < &va[idxEnd]-inc; )
	{
		GFX_VERTEXV( pa ); // vertex a
		GFX_VERTEXV( pb+inc ); // vertex b offset
		pa += 2*inc;
		pb += 2*inc;
	}

	// End with last vertex of both arrays.
	pa = &va[idxEnd];
	pb = &vb[idxEnd];
	GFX_VERTEXV( pa );
	GFX_VERTEXV( pb );

	GFX_END();

	return;
}

#endif /* GFXSYS_OPENGL */

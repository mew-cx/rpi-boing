/*****************************************************************************
 * $Id: gfx_opengl.h,v 1.14 2004/04/04 22:52:37 jimb Exp $
 * Author:	Jim E. Brooks  http://www.jimbrooks.org
 * Desc:	Functions/macros for OpenGL.
 * Notes:
 * LEGAL:	COPYRIGHT (C) JIM E. BROOKS 2004
 *          THIS SOURCE CODE IS RELEASED UNDER THE TERMS
 *          OF THE GNU GENERAL PUBLIC LICENSE (GPL).
 *****************************************************************************/

#if GFXSYS_OPENGL
#ifndef GFX_OPENGL_H
#define GFX_OPENGL_H 1

//------------------------------------------------------------------------------
// Macros to abstract programming the OpenGL way.
// GFX_BEGIN(GP_*)
// GFX_END()
// GFX_RGB(fp,fp,fp)    non-vector
// GFX_RGBV(fp[3])      vector
// GFX_RGBAV(fp[4])     vector
// GFX_VERTEXV(fp[3])   vector   there is no non-vector form
// !!NOTE!! Callers must pass type fp args!
// The vector forms will fail if pointer to a wrong float type is passed.

// Graphic primitives (GP) for passing to GFX_BEGIN().
enum gp_e
{
	GP_LINE_LOOP		= GL_LINE_LOOP,
	GP_POLYGON			= GL_POLYGON,
	GP_TRIANGLES		= GL_TRIANGLES,
	GP_TRIANGLE_STRIP	= GL_TRIANGLE_STRIP,
	GP_QUAD_STRIP		= GL_QUAD_STRIP,
};

// GFX_BEGIN()
#define GFX_BEGIN(x)			glBegin( (x) )

// GFX_END()
#define GFX_END()				glEnd()

// GFX_RGB*()
#if FPDOUBLE || FPLONGDOUBLE
#	define GFX_RGB(r,g,b)		glColor3d( (r), (g), (b) )
	// fp type is either double or long double so proper type conversion is used.
	static inline void GFX_RGBV(  const fp* p ) { glColor3d( p[0], p[1], p[2]       ); }
	static inline void GFX_RGBAV( const fp* p ) { glColor4d( p[0], p[1], p[2], p[3] ); }
#else /* typedef GLfloat fp */
#	define GFX_RGB(r,g,b)		glColor3f( (r), (g), (b) )
#	define GFX_RGBV(p)			glColor3fv( (p) )
#	define GFX_RGBAV(p)			glColor4fv( (p) )
#endif

// GFX_VERTEXV()
// The matrix math code is based on vertexWorld/Object/Eye structs with .x/.y/.z members.
// But OpenGL is more efficient when its vector functions (array arg) functions are called
// (as opposed to the non-vector functions that need copying/pushing .x/.y/.z onto the stack).
// SelfCheck() which main() should call ensures that the vertex* structs and GLfloat[3]
// are equivalent and can be cast interchangeably.
// The inline functions use the compiler to convert float types (the faster macros don't!).
#if FPLONGDOUBLE
	static inline void 	GFX_VERTEXV( const vertexEye* v ) { glVertex3d( v->x, v->y, v->z ); }
#elif FPDOUBLE
	#if ALTVERTEX
	static inline void 	GFX_VERTEXV( const vertexEye* v ) { glVertex3d( v->x, v->y, v->z ); }
	#else
	#define 			GFX_VERTEXV( v )					glVertex3dv( (GLdouble*)(v) )
	#endif
#else /* single-precision (default) */
	#if ALTVERTEX
	static inline void	GFX_VERTEXV( const vertexEye* v ) { glVertex3f( v->x, v->y, v->z ); }
	#else
	#define				GFX_VERTEXV( v )					glVertex3fv( (GLfloat*)(v) )
	#endif
#endif

#define GFX_VERTEX		ERROR_USE_GFX_VERTEXV_INSTEAD
//------------------------------------------------------------------------------
// Anti-aliasing.
// This uses GL_LINE_SMOOTH to draw a smooth outline over
// a polyon in order to anti-alias.  The subroutine which
// these two macros must be capable of drawing the same object twice.
#define GFX_AA_BEGIN_LOOP \
for ( int gfx_aa = 0; gfx_aa < 2; ++gfx_aa ) \
{ \
	if ( gfx_aa == 0 ) \
	{ \
		glEnable( GL_POLYGON_OFFSET_FILL ); \
		glPolygonOffset( 1.0, 1.0 ); \
	} \
	else \
	{ \
		glDisable( GL_POLYGON_OFFSET_FILL ); \
		glEnable( GL_BLEND ); \
		glEnable( GL_LINE_SMOOTH ); \
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ); \
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); \
		glLineWidth( 1.0 ); \
	}

#define GFX_AA_END_LOOP \
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ); \
	glDisable( GL_LINE_SMOOTH ); \
	glDisable( GL_BLEND ); \
}

//------------------------------------------------------------------------------

// Keyboard.
enum keyboard_e
{
#ifdef __BEOS__
	KEY_ENTER	= 0xa,
#else
	KEY_ENTER	= 0xd,
#endif
	KEY_ESC		= 27,
	KEY_TAB		= 0x9,
	KEY_BS		= 0x8,
	KEY_DEL		= 0x7f,
	KEY_CTRL_S	= 0x13,
	KEY_1		= 0x31,
	KEY_PLUS	= 0x2b,
	KEY_MINUS	= 0x2d,
	KEY_LEFT	= 0x64,		// for GLUT special key handler
	KEY_RIGHT	= 0x66,		// for GLUT special key handler
	KEY_UP		= 0x65,		// for GLUT special key handler
	KEY_DOWN	= 0x67,		// for GLUT special key handler
	KEY_HOME	= 0x6a,		// for GLUT special key handler
	KEY_END		= 0x6b,		// for GLUT special key handler
	KEY_F1		= 0x1,		// for GLUT special key handler
	KEY_F2		= 0x2,		// for GLUT special key handler
	KEY_F3		= 0x3,		// for GLUT special key handler
	KEY_F4		= 0x4,		// for GLUT special key handler
	KEY_F5		= 0x5,		// for GLUT special key handler
	KEY_F6		= 0x6,		// for GLUT special key handler
	KEY_F7		= 0x7,		// for GLUT special key handler
	KEY_F8		= 0x8,		// for GLUT special key handler
	KEY_F9		= 0x9,		// for GLUT special key handler
	KEY_F10		= 0xa,		// for GLUT special key handler
	KEY_F11		= 0xb,		// for GLUT special key handler
	KEY_F12		= 0xc,		// for GLUT special key handler
};

//------------------------------------------------------------------------------

fp
PerspectiveAngle( fp size, fp dist );

void
DrawTriangleStrip( const vertexEye* va,
                   const vertexEye* vb,
                   int idxEnd,
                   int inc );

#endif /* GFX_OPENGL_H */
#endif /* GFXSYS_OPENGL */

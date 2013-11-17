/*****************************************************************************
 * $Id: gfx_types.h,v 1.16 2004/03/09 17:15:39 jimb Exp $
 * Author:	Jim E. Brooks  http://www.jimbrooks.org
 * Desc:	Types, structs, enums, etc.
 * Notes:
 * LEGAL:	COPYRIGHT (C) JIM E. BROOKS 2004
 *          THIS SOURCE CODE IS RELEASED UNDER THE TERMS
 *          OF THE GNU GENERAL PUBLIC LICENSE (GPL).
 *****************************************************************************/

#ifndef GFX_TYPES_H
#define GFX_TYPES_H 1

///////////////
// Basic types.
#ifndef _WIN32
typedef signed char		byte;
#endif
typedef unsigned char	ubyte;
typedef unsigned char	uchar;
typedef unsigned short	ushort;
typedef unsigned int	uint;
#if defined(__BEOS__)
// BeOS defines *int* in SupportDefs.h.
#else
#	if GFXSYS_OPENGL
typedef GLbyte			int8;
typedef GLubyte			uint8;
typedef GLshort			int16;
typedef GLushort		uint16;
typedef GLint			int32;
typedef GLuint			uint32;
#	else
typedef char			int8;
typedef unsigned char	uint8;
typedef short			int16;
typedef unsigned short	uint16;
typedef int				int32;
typedef unsigned int	uint32;
#	endif
#endif

// Compile single/double/extended precision floating-point instructions.
// (with a few minor exceptions where "float"/"double" is hard-coded).
// Higher precision can be specified by make FPDOUBLE=1 or FPLONGDOUBLE=1
// On x86/SSE1 (PentiumIII,Athlon), [LONG]DOUBLE=1 isn't optimal as SSE1
// is a single-precision FPU.
//
// NOTE: When using printf() to print a "fp" variable (as fp type is changeable),
// explicitly write a type conversion to a standard float type.
// (Better yet, use C++ cout)
// fp x; printf("%f",x);               // oops if fp is long double
// fp x; printf("%f",float(x));        // ok
// fp x; printf("%Lf",(long double)x); // ok
#if FPDOUBLE && FPLONGDOUBLE
#error
#endif

#if GFXSYS_OPENGL
#	if FPLONGDOUBLE
	typedef long double fp;
#	elif FPDOUBLE
	typedef GLdouble fp;
#	else /* single-precision (default) */
	typedef GLfloat fp;
#	endif
	typedef long double fpx; // extended-precision float
#else /* GFXSYS_[other] */
#	if FPLONGDOUBLE
	typedef long double fp;
#	elif FPDOUBLE
	typedef double fp;
#	else /* single-precision (default) */
	typedef float fp;
#	endif
	typedef long double fpx; // extended-precision float
#endif

////////////////////////////////////////////////////////////////////////
// Type decorations.
// To declare const class members (eg const arrays) if compiler permits.
#ifdef __GNUC__
#define CONSTMEMBER const
#else
#define CONSTMEMBER
#endif

/////////
// enums
enum { AXIS_X, AXIS_Y, AXIS_Z };

//////////////////
// Matrix offsets.
// My math functions and OpenGL share the same matrix format.
enum { Xx, Xy, Xz, Xs,
       Yx, Yy, Yz, Ys,
       Zx, Zy, Zz, Zs,
       Ox, Oy, Oz, Os }; /*origin*/

/////////////////////////////
// Index into a vertex array.
typedef unsigned short vidx_t;

////////////////////////////////////////////
// Vertexs in different coordinate systems.
struct vertexLocal
{
	fp	x;
	fp	y;
	fp	z;
};
struct vertexWorld
{
	fp	x;
	fp	y;
	fp	z;
	// Type signature must follow x/y/z as struct is cast as fp[3].
	DECL_TYPESIG_AUTO( vertexWorld, TYPESIG_LIB_VERTEX_WORLD )
};
struct vertexEye
{
	fp	x;
	fp	y;
	fp	z;
	// Type signature must follow x/y/z as struct is cast as fp[3].
	DECL_TYPESIG_AUTO( vertexEye, TYPESIG_LIB_VERTEX_EYE )
#if TYPESIGS
	vertexEye( fp X, fp Y, fp Z ) { SET_TYPESIG(this,TYPESIG_LIB_VERTEX_EYE); x=X; y=Y; z=Z; };
#endif
};
struct vertexNormal
{
	fp	x;
	fp	y;
	fp	z;
	// Type signature must follow x/y/z as struct is cast as fp[3].
	DECL_TYPESIG_AUTO( vertexNormal, TYPESIG_LIB_VERTEX_NORMAL )
};

/////////////////////////////////////////
// Element in an array of normal vectors.
struct normalWorld
{
	vertexNormal	norm;		// normal vector in world coordinates
	vidx_t			viNormOrig;	// index into array of world vertexes of normal's origina
	// Type signature must follow x/y/z as struct is cast as fp[3].
	DECL_TYPESIG_AUTO( normalWorld, TYPESIG_LIB_NORMAL_WORLD )
};
typedef normalWorld normalWorld_t;

/////////////////////////////////////////////////
// Index of the first three vertexes of a polygon
// (for calculating normal vectors).
struct polygon3
{
	vidx_t			vi[3];
	// Type signature must follow x/y/z as struct is cast as fp[3].
	DECL_TYPESIG_AUTO( polygon3, TYPESIG_LIB_POLYGON3 )
};
typedef polygon3 polygon3_t;

#endif /* GFX_TYPES_H */

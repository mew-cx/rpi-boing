/*****************************************************************************
 * $Id: gfx_math_inline.h,v 1.27 2004/04/05 21:15:32 jimb Exp $
 * Author:	Jim E. Brooks  http://www.jimbrooks.org
 * Desc:	(inline) Math functions for computer graphics.
 * Notes:   The matrix math here is based on the OpenGL 4x4 matrix format.
 *        	To speed or improve FP execution:
 *          - use multiplication instead of division
 *          - power of 2 operations involve no loss of accuracy
 * LEGAL:	COPYRIGHT (C) JIM E. BROOKS 2004
 *          THIS SOURCE CODE IS RELEASED UNDER THE TERMS
 *          OF THE GNU GENERAL PUBLIC LICENSE (GPL).
 *****************************************************************************/

#ifndef GFX_MATH_INLINE_H
#define GFX_MATH_INLINE_H 1


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/////////////////////////////  M A T R I X   ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


/*****************************************************************************
 * Load matrix with identity mapping.
 *****************************************************************************/
template<class FP>
void
IdentityMatrix( FP m[16]/*OUT*/ )
{
	m[0]  = 1.0; m[1]  = 0.0; m[2]  = 0.0; m[3]  = 0.0;
	m[4]  = 0.0; m[5]  = 1.0; m[6]  = 0.0; m[7]  = 0.0;
	m[8]  = 0.0; m[9]  = 0.0; m[10] = 1.0; m[11] = 0.0;
	m[12] = 0.0; m[13] = 0.0; m[14] = 0.0; m[15] = 1.0;
	return;
}

/*****************************************************************************
 * Copy matrix.
 * Each matrix can be different precision.
 *****************************************************************************/
template<class FPdest, class FPsrc>
void
CopyMatrix( FPdest dest[16], const FPsrc src[16] )
{
	for ( int i = 0; i < 16; ++i ) dest[i] = src[i];
	return;
}
template<class FPdest, class FPsrc>
void
CopyMatrixAxises( FPdest dest[16], const FPsrc src[16] )
{
	for ( int i = 0; i < 12; ++i ) dest[i] = src[i];
	return;
}

/*****************************************************************************
 * Transpose a matrix.
 * Doing so reverses how a matrix maps two coordinate systems.
 *****************************************************************************/
template<class FPdest, class FPsrc>
void
TransposeMatrix( FPdest dest[16], const FPsrc src[16] )
{
	// Make transposed copy of matrix.
	// X = ~X
	dest[Xx] = src[Xx];
	dest[Xy] = src[Yx];
	dest[Xz] = src[Zx];
	// Y = ~Y
	dest[Yx] = src[Xy];
	dest[Yy] = src[Yy];
	dest[Yz] = src[Zy];
	// Z = ~Z
	dest[Zx] = src[Xz];
	dest[Zy] = src[Yz];
	dest[Zz] = src[Zz];
	// copy origin identically
	dest[Ox] = src[Ox];
	dest[Oy] = src[Oy];
	dest[Oz] = src[Oz];
	return;
}

/*****************************************************************************
 * Subroutines to rotate/translate coordinates.
 *****************************************************************************/
static inline fp
RotCoord( int axis, fp x, fp y, fp z, const fp m[16] )
{
	return x * m[axis*4+0] + y * m[axis*4+1] + z * m[axis*4+2];
}
static inline fp // C++ should select this inline one if fp type matches
RotTransCoord( int axis, fp x, fp y, fp z, const fp m[16] )
{
	return x * m[axis*4+0] + y * m[axis*4+1] + z * m[axis*4+2] + m[12+axis];
}
template<class FP>
FP
RotTransCoord( int axis, FP x, FP y, FP z, const FP m[16] )
{
	return x * m[axis*4+0] + y * m[axis*4+1] + z * m[axis*4+2] + m[12+axis];
}
static inline fp
TransRotCoord( int axis, fp x, fp y, fp z, const fp m[16] )
{
	x += m[Ox];
	y += m[Oy];
	z += m[Oz];
	return x * m[axis*4+0] + y * m[axis*4+1] + z * m[axis*4+2];
}

/*****************************************************************************
 * Subroutines to rotate/translate vertexes.
 *****************************************************************************/
static inline void
RotTrans( vertexWorld* v, fp x, fp y, fp z, const fp m[16] )
{
	v->x = x * m[Xx] + y * m[Xy] + z * m[Xz] + m[Ox];
	v->y = x * m[Yx] + y * m[Yy] + z * m[Yz] + m[Oy];
	v->z = x * m[Zx] + y * m[Zy] + z * m[Zz] + m[Oz];
}
static inline void
RotTrans( vertexWorld* v2, const vertexLocal* v, const fp m[16] )
{
	v2->x = v->x * m[Xx] + v->y * m[Xy] + v->z * m[Xz] + m[Ox];
	v2->y = v->x * m[Yx] + v->y * m[Yy] + v->z * m[Yz] + m[Oy];
	v2->z = v->x * m[Zx] + v->y * m[Zy] + v->z * m[Zz] + m[Oz];
}

/*****************************************************************************
 * Transform a 3D vertex thru one coordinate system.
 *****************************************************************************/
static inline void
TransformVertex1( vertexEye* v2, const vertexWorld* v, const fp m[16] )
{
	// Variants of TransformVertex1() only differ in types (same calculation).
	fp x = v->x + m[Ox];
	fp y = v->y + m[Oy];
	fp z = v->z + m[Oz];
	v2->x = x * m[Xx] + y * m[Xy] + z * m[Xz];
	v2->y = x * m[Yx] + y * m[Yy] + z * m[Yz];
	v2->z = x * m[Zx] + y * m[Zy] + z * m[Zz];
	return;
}
static inline void
TransformVertex1( vertexWorld* v2, const vertexLocal* v, const fp m[16] )
{
	// Variants of TransformVertex1() only differ in types (same calculation).
	fp x = v->x + m[Ox];
	fp y = v->y + m[Oy];
	fp z = v->z + m[Oz];
	v2->x = x * m[Xx] + y * m[Xy] + z * m[Xz];
	v2->y = x * m[Yx] + y * m[Yy] + z * m[Yz];
	v2->z = x * m[Zx] + y * m[Zy] + z * m[Zz];
	return;
}
static inline void
TransformVertex1( vertexWorld* v2, fp x, fp y, fp z, const fp m[16] )
{
	// Variants of TransformVertex1() only differ in types (same calculation).
	x = x + m[Ox];
	y = y + m[Oy];
	z = z + m[Oz];
	v2->x = x * m[Xx] + y * m[Xy] + z * m[Xz];
	v2->y = x * m[Yx] + y * m[Yy] + z * m[Yz];
	v2->z = x * m[Zx] + y * m[Zy] + z * m[Zz];
	return;
}
static inline void
TransformVertex1Submit( fp x, fp y, fp z, const fp m[16] )
{
	x = x + m[Ox];
	y = y + m[Oy];
	z = z + m[Oz];
	vertexEye ve;
	ve.x = x * m[Xx] + y * m[Xy] + z * m[Xz];
	ve.y = x * m[Yx] + y * m[Yy] + z * m[Yz];
	ve.z = x * m[Zx] + y * m[Zy] + z * m[Zz];
	GFX_VERTEXV( &ve );
	return;
}

/*****************************************************************************
 * Transform a 3D vertex thru two coordinate systems.
 *
 * Two transformations are done.  Typically used to transform objects
 * (such as aircraft) which have their own local coordinate systems.
 * local coords --> world coords --> eye coords
 *
 * NOTE: PrepTransformVertex2() must be called prior to TransformVertex2().
 * This requirement is for an optimization that combines two translations
 * (local-->world, world-->eye).
 *****************************************************************************/
static const fp*	tv2_m;
static const fp*	tv2_m2;
static fp			tv2_Ox;
static fp			tv2_Oy;
static fp			tv2_Oz;

static inline void
PrepTransformVertex2( const fp m[16], const fp m2[16] )
{
	tv2_m	= m;
	tv2_m2	= m2;
	tv2_Ox	= m[Ox] + m2[Ox];	// combine translations (optimization)
	tv2_Oy	= m[Oy] + m2[Oy];
	tv2_Oz	= m[Oz] + m2[Oz];
}
static inline void
TransformVertex2( vertexEye* v /*OUT*/, fp x, fp y, fp z ) /* (x,y,z)=vertexLocal */
{
#undef m
#undef m2
#define m  tv2_m
#define m2 tv2_m2
	fp x2, y2, z2;

	// Map local system --> world system
	x2 = x * m[Xx] + y * m[Xy] + z * m[Xz];
	y2 = x * m[Yx] + y * m[Yy] + z * m[Yz];
	z2 = x * m[Zx] + y * m[Zy] + z * m[Zz];

	// Two combined translations: local --> world --> eye
	x2 += tv2_Ox;
	y2 += tv2_Oy;
	z2 += tv2_Oz;

	// Map world system --> eye system
	v->x = x2 * m2[Xx] + y2 * m2[Xy] + z2 * m2[Xz];
	v->y = x2 * m2[Yx] + y2 * m2[Yy] + z2 * m2[Yz];
	v->z = x2 * m2[Zx] + y2 * m2[Zy] + z2 * m2[Zz];
	return;
#undef m
#undef m2
}
static inline void
TransformVertex2( vertexEye* v /*OUT*/, const vertexLocal* w )
{
	TransformVertex2( v, w->x, w->y, w->z );
}

/*****************************************************************************
 * Translate a matrix by row.
 * For translating the eye/viewpoint.
 *****************************************************************************/
template<class FP>
void
TranslateMatrixFixed( int axis, FP inc, FP m[16]/*OUT*/ )
{
	m[Ox] += inc * m[axis*4+0];
	m[Oy] += inc * m[axis*4+1];
	m[Oz] += inc * m[axis*4+2];
	return;
}

/*****************************************************************************
 * Translate a matrix relative to itself (local translation).
 * For translating an object.
 *****************************************************************************/
template<class FP>
void
TranslateMatrixLocal( int axis, FP inc, FP m[16]/*OUT*/ )
{
	FP x, y, z;
	switch ( axis )
	{
		case 0:
			x = inc; y = 0.0; z = 0.0;
			break;
		case 1:
			x = 0.0; y = inc; z = 0.0;
			break;
		case 2:
			x = 0.0; y = 0.0; z = inc;
			break;
		default: return;
	}
	m[12] += x * m[0*4+0] + y * m[0*4+1] + z * m[0*4+2];
	m[13] += x * m[1*4+0] + y * m[1*4+1] + z * m[1*4+2];
	m[14] += x * m[2*4+0] + y * m[2*4+1] + z * m[2*4+2];
	return;
}

/*****************************************************************************
 * Rotate a matrix around a fixed axis.
 *
 * This function is suited to rotating the viewpoint/eye.
 * The word "fixed" should be clear by looking at the math.
 * Eg, the value of the X coord won't be changed by a rotation around the X axis.
 *****************************************************************************/
template<class FP>
void
RotateMatrixFixed( int axis, FP degree, FP m[16]/*OUT*/ )
{
	FP s = sin( deg2rad(degree) );
	FP c = cos( deg2rad(degree) );
	FP n[16];
	CopyMatrix( n, m ); //dest,src
	switch ( axis )
	{
		case 0: // X axis Pitch
		n[4] = m[4]*c - m[8]*s;		// Y = Ycos - Zsin
		n[5] = m[5]*c - m[9]*s;
		n[6] = m[6]*c - m[10]*s;

		n[8] = m[4]*s + m[8]*c;		// Z = Ysin + Zcos
		n[9] = m[5]*s + m[9]*c;
		n[10]= m[6]*s + m[10]*c;
		break;

		case 1: // Y axis Yaw
		n[0] = m[0]*c - m[8]*s;		// X = Xcos - Zsin
		n[1] = m[1]*c - m[9]*s;
		n[2] = m[2]*c - m[10]*s;

		n[8] = m[0]*s + m[8]*c;		// Z = Xsin + Zcos
		n[9] = m[1]*s + m[9]*c;
		n[10]= m[2]*s + m[10]*c;
		break;

		case 2: // Z axis Roll
		n[0] = m[0]*c - m[4]*s;		// X = Xcos - Zsin
		n[1] = m[1]*c - m[5]*s;
		n[2] = m[2]*c - m[6]*s;

		n[4] = m[0]*s + m[4]*c;		// Z = Xsin + Zcos
		n[5] = m[1]*s + m[5]*c;
		n[6] = m[2]*s + m[6]*c;
		break;
	}
	CopyMatrix( m, n ); //dest,src
	return;
}

/*****************************************************************************
 * Rotate a local coordinate system around its own axis.
 *
 * This function is suited to rotating an independent object.
 * The rotation is relative to local coodinate system (not the fixed/eye system).
 * The trick is to load an identity-mapped matrix and rotate it, then transform
 * it thru the given matrix (which defines the local coordinate system)
 * as though it was the coords (1.0, 1.0, 1.0).  These coords of course define
 * the X,Y,Z axises.  The transformation is effectively a local rotation.
 *****************************************************************************/
template<class FP>
void
RotateMatrixLocal( int axis, FP degree, FP m[16]/*OUT*/ )
{
	// Identity (1:1) matrix r.
	FP r[16];
	IdentityMatrix<FP>( r );

	// Rotate matrix r.
	RotateMatrixFixed<FP>( axis, degree, r );

	// Transform r thru m.
	// Ie, thru matrix m, pass r as if it were the rotated coords (1.0, 1.0, 1.0).
	FP t[16];
	t[0] = RotTransCoord<FP>(0,m[0],m[1],m[2],r);
	t[1] = RotTransCoord<FP>(1,m[0],m[1],m[2],r);
	t[2] = RotTransCoord<FP>(2,m[0],m[1],m[2],r);

	t[4] = RotTransCoord<FP>(0,m[4],m[5],m[6],r);
	t[5] = RotTransCoord<FP>(1,m[4],m[5],m[6],r);
	t[6] = RotTransCoord<FP>(2,m[4],m[5],m[6],r);

	t[8] = RotTransCoord<FP>(0,m[8],m[9],m[10],r);
	t[9] = RotTransCoord<FP>(1,m[8],m[9],m[10],r);
	t[10]= RotTransCoord<FP>(2,m[8],m[9],m[10],r);

	// m = r
	// Don't copy over bottom row which contains the origin coords.
	m[0] = t[0];
	m[1] = t[1];
	m[2] = t[2];

	m[4] = t[4];
	m[5] = t[5];
	m[6] = t[6];

	m[8] = t[8];
	m[9] = t[9];
	m[10]= t[10];

	return;
}

/*****************************************************************************
 * Print matrix.
 *****************************************************************************/
template<class FP>
void
PrintMatrix( const FP m[16] )
{
	Msg( "%8f %8f %8f %8f \n", float(m[0] ),  float(m[1] ),  float(m[2] ), float(m[3] ) );
	Msg( "%8f %8f %8f %8f \n", float(m[4] ),  float(m[5] ),  float(m[6] ), float(m[7] ) );
	Msg( "%8f %8f %8f %8f \n", float(m[8] ),  float(m[9] ),  float(m[10]), float(m[11]) );
	Msg( "%8f %8f %8f %8f \n", float(m[12]),  float(m[13]),  float(m[14]), float(m[15]) );
	Msg( "\n" );
}
template<class FP>
void
PrintMatrixOrigin( const FP m[16] )
{
	Msg( "%8f %8f %8f \n", float(m[Ox]), float(m[Oy]), float(m[Oz]) );
	Msg( "\n" );
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//////////////////////////////  V E C T O R   //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


/*****************************************************************************
 * Calculate a cross product to yield a normal vector.
 * c = a x b
 *****************************************************************************/
template<class VERTEX>
void
CrossProduct( vertexNormal* n, /*OUT*/
              const VERTEX* v1,
              const VERTEX* v2,
              const VERTEX* v3 )
{
	fp a, b, c;
	fp d, e, f;

	a = v2->x - v1->x;
	b = v2->y - v1->y;
	c = v2->z - v1->z;

	d = v3->x - v1->x;
	e = v3->y - v1->y;
	f = v3->z - v1->z;

	n->x = b*f - c*e;
	n->y = c*d - a*f;
	n->z = a*e - b*d;

	return;
}

/*****************************************************************************
 * Calculate the dot product of two vectors.
 *****************************************************************************/
template<class VERTEX1, class VERTEX2>
fp
DotProduct( const VERTEX1* v1, const VERTEX2* v2 )
{
	return   v1->x * v2->x
           + v1->y * v2->y
           + v1->z * v2->z;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///////////////////////////////  T R I G   /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


/*****************************************************************************
 * Truncate a degree.
 *****************************************************************************/
static inline fp
TruncateDeg( fp deg )
{
	if ( deg >= 360.0 )
		return (deg - 360.0);
	else
		return deg;
}

/*****************************************************************************
 * Convert a degree (360-based) into a radian.
 * 360' = 2 * PI
 *****************************************************************************/
static inline fp
deg2rad( fp deg )
{
#if 0
	return deg / 360 * (2 * M_PI); // x87 FDIV is slower than FMUL
#else
	const fp f = (1.0/360.0) * (2*M_PI);
	return deg * f;
#endif
}

/*****************************************************************************
 * 360' sin().
 *****************************************************************************/
static inline fp
sin_deg( fp deg )
{
	return sin( deg2rad( deg ) );
}

/*****************************************************************************
 * 360' cos().
 *****************************************************************************/
static inline fp
cos_deg( fp deg )
{
	return cos( deg2rad( deg ) );
}

/*****************************************************************************
 * Distance.
 *****************************************************************************/
// An alternative is DistanceEyeObjectSkipSqrt() in sim/.
static inline fp
Distance( fp X1, fp Y1, fp Z1,
          fp X2, fp Y2, fp Z2 )
{
	fp xd = X2 - X1;
	fp yd = Y2 - Y1;
	fp zd = Z2 - Z1;
	return sqrt( xd*xd + yd*yd + zd*zd );
}
static inline fp
Distance( fp x, fp y, fp z, const fp m[16] )
{
	return Distance( x, y, z, m[Ox], m[Oy], m[Oz] );
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////  M I S C   M A T H   //////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


/*****************************************************************************
 * Force a number to be within a range.
 *****************************************************************************/
template<class T>
T
Range( T val, T lo, T hi )
{
	if ( val < lo ) return lo;
	if ( val > hi ) return hi;
	return val;
}

/*****************************************************************************
 * Map a number from one range into another.
 *****************************************************************************/
static inline fp
Remap( fp value, fp rangeOld, fp rangeNew )
{
	return value/rangeOld*rangeNew;
}

/*****************************************************************************
 * Truncate a float at an interval.
 * Eg, Truncate(24,10) returns 20.
 *****************************************************************************/
static inline fp
Truncate( fp val, fp interval )
{
	double integral;
	modf( val/interval, &integral );
	return integral*interval;
}

#endif /* GFX_MATH_INLINE_H */

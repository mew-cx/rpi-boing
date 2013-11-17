/*****************************************************************************
 * $Id: gfx_define.h,v 1.21 2004/03/21 17:57:26 jimb Exp $
 * Author:	Jim E. Brooks  http://www.jimbrooks.org
 * Desc:	Definitions, macros.
 * Notes:
 * LEGAL:	COPYRIGHT (C) JIM E. BROOKS 2004
 *          THIS SOURCE CODE IS RELEASED UNDER THE TERMS
 *          OF THE GNU GENERAL PUBLIC LICENSE (GPL).
 *****************************************************************************/

#ifndef GFX_DEFINE_H
#define GFX_DEFINE_H 1

/////////////////////////
// definitions, constants
#undef  FALSE
#undef  TRUE
#define FALSE 0
#define TRUE  1

#ifndef M_PI
#define M_PI 3.14159265358979323846  /* pi */
#endif

// ILLEGAL_PTR is a non-zero value for initializing/asserting pointers
// that normally can hold either a non-NULL or NULL address.
#ifndef ILLEGAL_PTR
#define ILLEGAL_PTR ((void*)13) /* an unlucky address in the faulty zero page */
#endif

/////////////////////////
// macro/inline functions
#ifndef MIN
#define MIN(a,b) ((a < b) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a,b) ((a > b) ? (a) : (b))
#endif
#ifndef STREQ
#define STREQ(a,b) (strcmp((a),(b))==0)
#endif
#ifndef STRNE
#define STRNE(a,b) (strcmp((a),(b))!=0)
#endif
#ifndef MEMEQ
#define MEMEQ( s1, s2, len ) ( memcmp( (s1), (s2), (len) ) == 0 )
#endif
#ifndef MENNE
#define MEMNE( s1, s2, len ) ( memcmp( (s1), (s2), (len) ) != 0 )
#endif

static inline void CLEAR( void* p, int n ) { memset( p, 0, n ); }

////////////////////////////////////
// To give branch hints to compiler.
#if __GNUC__ >= 3
#define likely(x)	__builtin_expect((x),1)
#define unlikely(x)	__builtin_expect((x),0)
#else
#define likely(x) (x)
#define unlikely(x) (x)
#endif
#define EX(x) likely((x))	/* abbrev for expected */
#define UX(x) unlikely((x))	/* abbrev for unexpected */

// (GNUish) Tell compiler to pack a struct member.
#ifdef __GNUC__
#define PACKMEMB __attribute__ ((packed))
#else
#define PACKMEMB
#endif

// Tell compiler a var is indeed unused.
#if GCC_VERSION > 0 && GCC_VERSION >= 30100 /* if gcc-3.1 or later */
#define UNUSED __attribute__ ((unused))
#else
#define UNUSED
#endif

#endif /* GFX_DEFINE_H */

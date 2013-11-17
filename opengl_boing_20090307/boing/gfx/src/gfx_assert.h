/*****************************************************************************
 * $Id: gfx_assert.h,v 1.10 2004/03/08 01:14:11 jimb Exp $
 * Author:	Jim E. Brooks  http://www.jimbrooks.org
 * Desc:	Assertion macros.
 * Notes:
 * LEGAL:	COPYRIGHT (C) JIM E. BROOKS 2004
 *          THIS SOURCE CODE IS RELEASED UNDER THE TERMS
 *          OF THE GNU GENERAL PUBLIC LICENSE (GPL).
 *****************************************************************************/

#ifndef GFX_ASSERT_H
#define GFX_ASSERT_H 1

// make ASSERT=1 to enable assert() (normally disabled for speed).
#if ! ASSERT
#define NDEBUG  /* disable assert() */
#endif
#include <assert.h>

#if defined(_WIN32) && ASSERT
#undef assert
// To keep console going after assertion fails.
// __LINE__ is a decimal constant, not a string.
#define assert(X) if (!(X)) { Msg( "\n### assert(%s) failed in %s at line %d ###\n", #X, __FILE__, __LINE__ ); for(;;)0; }
#endif

// AssertOr*() is for error conditions where the program might be able to continue.
// The OR case is in case assert() is disabled by NDEBUG.
#ifndef AssertOrReturn
#define AssertOrReturn( cond ) \
{ \
	assert( (cond) ); \
	if ( !(cond) ) return; \
}
#define AssertOrReturnVal( cond, val ) \
{ \
	assert( (cond) ); \
	if ( !(cond) ) return (val); \
}
#define AssertOrGoto( cond, label ) \
{ \
	assert( (cond) ); \
	if ( !(cond) ) goto label; \
}
#endif /* AssertOrReturn */

#endif /* GFX_ASSERT_H */

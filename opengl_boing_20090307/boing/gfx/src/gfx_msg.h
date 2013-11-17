/*****************************************************************************
 * $Id: gfx_msg.h,v 1.10 2004/03/13 19:40:54 jimb Exp $
 * Author:	Jim E. Brooks  http://www.jimbrooks.org
 * Desc:	Functions to print messages, errors, etc.
 * Notes:
 * LEGAL:	COPYRIGHT (C) JIM E. BROOKS 2004
 *          THIS SOURCE CODE IS RELEASED UNDER THE TERMS
 *          OF THE GNU GENERAL PUBLIC LICENSE (GPL).
 *****************************************************************************/

#ifndef GFX_MSG_H
#define GFX_MSG_H 1

// Print an error message.
#ifdef __GNUC__
#define ErrorMsg( format, args... )			\
	fprintf( stderr, "*** ERROR: " )	,	\
	fprintf( stderr, format , ## args )
#else
#define ErrorMsg printf
#endif

// Print a normal message.
#ifdef __GNUC__
#define Msg( format, args... )				\
	fprintf( stdout, format , ## args ), fflush( stdout )
#else
#define Msg printf
#endif

// Print a status message (enabled by make VERBOSE=1).
#if VERBOSE
#define StatusMsg Msg
#else
  #ifdef __GNUC__
  #define StatusMsg( format, args... )
  #else
  static void StatusMsg( char* format, ... ) { }
  #endif
#endif

// Print a debug message (enabled by make DEBUG=1).
#if DEBUG
  #define DebugMsg Msg
#else
  #ifdef __GNUC__
  #define DebugMsg( format, args... )
  #else
  static void DebugMsg( char* format, ... ) { }
  #endif
#endif

// Print debug message if verbose (enabled by make DEBUG=1 VERBOSE=1).
// For printing messages that are undesired or too frequent in normal debug builds.
#if DEBUG && VERBOSE
  #define DebugVerboseMsg Msg
#else
  #ifdef __GNUC__
  #define DebugVerboseMsg( format, args... )
  #else
  static void DebugVerboseMsg( char* format, ... ) { }
  #endif
#endif

#endif /* GFX_MSG_H */

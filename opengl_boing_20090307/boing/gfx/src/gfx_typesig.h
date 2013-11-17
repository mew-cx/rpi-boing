/*****************************************************************************
 * $Id: gfx_typesig.h,v 1.33 2004/03/13 14:50:09 jimb Exp $
 * Author:	Jim E. Brooks  http://www.jimbrooks.org
 * Desc:	Support for checking type signatures.
 * Notes:
 * LEGAL:	COPYRIGHT (C) JIM E. BROOKS 2004
 *          THIS SOURCE CODE IS RELEASED UNDER THE TERMS
 *          OF THE GNU GENERAL PUBLIC LICENSE (GPL).
 *****************************************************************************/

#ifndef GFX_TYPESIG_H
#define GFX_TYPESIG_H 1

// TYPESIGS controls compiling type signatures.
#ifndef TYPESIGS
#	if DEBUG
#		define TYPESIGS 1
#	else
#		define TYPESIGS 0
#   endif
#endif

////////////////////////////////////////////////////////////////////////////////
// Type signatures are used to catch pointers to objects of the wrong type
// and to ensure the memory holding object isn't corrupted.
// Type signatures are conditionally compiled according to #define TYPESIGS.
// A set of macros are provided which expand to nothing if TYPESIGS=0.
// Secondary (and additional) type signatures can be used to for checking
// the base class type signature of derived object.
//
// Passing the wrong type signature by mistake will be caught by the compiler
// as the name of the type signature member is catenated with the name of
// the enum (this is a macro trick).
//
// Step-by-step example of using type signatures:
//
//     // Prerequisite definitions:
//     define TYPESIGS 1 and include assert.h
//     typesig_t:       typedef unsigned char typesig_t;
//     TYPESIG_INVALID: your enum for an invalid type signature.
//     PACKMEMB:        compiler-specific macro to tell compiler to
//                      pack a struct member (ok if it expands to nothing)
//
//     // Example of declaring type signature enums.
//     enum
//     {
//        TYPESIG_INVALID,
//        TYPESIG_MYSTRUCT,
//     };
//
//     // Example of declaring and setting a type signature in a class.
//     // The destructor invalidates the type signature in order to
//     // catch the error of using a destroyed object by a stale pointer.
//     class MyClass
//     {
//     public:
//         MyClass()        { SET_TYPESIG(this,TYPESIG_MYCLASS); valid = FALSE; };
//         MyClass( int x ) { SET_TYPESIG(this,TYPESIG_MYCLASS); valid = TRUE; value = x; };
//         ~MyClass()       { INVALIDATE_TYPESIG(this,TYPESIG_MYCLASS); };
//         bool valid;
//         int  value;
//         DECL_TYPESIG(TYPESIG_MYCLASS) // declare last for better alignment and if cast
//     }
//
//     // DECL_TYPESIG_AUTO() is a shorthand that expands a constructor
//     // and destructor to automatically set the type signature.
//     // Using it in structs is recommended but is inappropriate in most classes.
//     struct MyStruct
//     {
//         int   data;
//         DECL_TYPESIG_AUTO(MyStruct,TYPESIG_MYSTRUCT) // declare for better alignment and if cast
//     };
//
//     // Example of checking a type signature.
//     // NOTE: For terseness, CHECK_TYPESIG() asserts that the pointer isn't NULL
//     //       to obviate writing assert(p) prior to CHECK_TYPESIG(p,TYPESIG_MYSTRUCT).
//     //       If a pointer is allowed to be NULL, use CHECK_TYPESIG_NULL_OK() instead.
//     void Do( MyStruct* p )
//     {
//         CHECK_TYPESIG(p,TYPESIG_MYSTRUCT);         // asserts pointer != NULL
//         CHECK_TYPESIG_NULL_OK(p,TYPESIG_MYSTRUCT); // alternative that allows NULL ptr
//     }
////////////////////////////////////////////////////////////////////////////////

// Macro to declare a type signature as a member in a class/struct.
#if TYPESIGS
#define DECL_TYPESIG( TYPESIG ) typesig_t typesig##TYPESIG PACKMEMB;
#else
#define DECL_TYPESIG( TYPESIG )
#endif

// Macro to declare a type signature member and expand
// a constructor/destructor to automatically set it.
// Recommended for structs but isn't appropriate in most classes.
#if TYPESIGS
#define DECL_TYPESIG_AUTO( STRUCT, TYPESIG ) \
	STRUCT()  {        SET_TYPESIG( this, TYPESIG ); }; \
	~STRUCT() { INVALIDATE_TYPESIG( this, TYPESIG ); }; \
	DECL_TYPESIG( TYPESIG )
#else
#define DECL_TYPESIG_AUTO( STRUCT, TYPESIG )
#endif

// Macro to assign a type signature member.
#if TYPESIGS
#define SET_TYPESIG( OBJ, TYPESIG ) ( (OBJ)->typesig##TYPESIG = TYPESIG )
#else
#define SET_TYPESIG( OBJ, TYPESIG )
#endif

// Macro to invalidate a type signature member.
#if TYPESIGS
#define INVALIDATE_TYPESIG( OBJ, TYPESIG ) ( (OBJ)->typesig##TYPESIG = TYPESIG_INVALID )
#else
#define INVALIDATE_TYPESIG( OBJ, TYPESIG )
#endif

// Macro to check a type signature -- WILL FAIL IF PASSED A NULL POINTER!!
#if TYPESIGS
#define CHECK_TYPESIG( OBJ, TYPESIG ) assert( (OBJ) != NULL && (OBJ)->typesig##TYPESIG == TYPESIG )
#else
#define CHECK_TYPESIG( OBJ, TYPESIG )
#endif

// Macro to check a type signature -- will accept NULL pointer (ok)
#if TYPESIGS
#define CHECK_TYPESIG_NULL_OK( OBJ, TYPESIG ) assert( (OBJ) == NULL || (OBJ)->typesig##TYPESIG == TYPESIG )
#else
#define CHECK_TYPESIG_NULL_OK( OBJ, TYPESIG )
#endif

#endif /* GFX_TYPESIG_H */

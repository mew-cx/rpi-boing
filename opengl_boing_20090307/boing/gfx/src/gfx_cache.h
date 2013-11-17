/*****************************************************************************
 * $Id: gfx_cache.h,v 1.31 2004/03/27 13:32:26 jimb Exp $
 * Author:	Jim E. Brooks  http://www.jimbrooks.org
 * Desc:	C++ template for a C++ object cache.
 * Notes:
 * LEGAL:	COPYRIGHT (C) JIM E. BROOKS 2004
 *          THIS SOURCE CODE IS RELEASED UNDER THE TERMS
 *          OF THE GNU GENERAL PUBLIC LICENSE (GPL).
 *****************************************************************************/

#ifndef GFX_CACHE_H
#define GFX_CACHE_H 1

////////////////////////////////////////////////////////////////////////////////
// Template for a cache of C++ objects.
//
// The purpose of this cache is to reduce memory consumption
// by deleting objects which haven't been used recently.
// Each cache only caches one type of C++ objects.
// Different caches must be instantiated to cache different types.
// All C++ objects put into a cache must be allocated by the C++ new operator
// as the cache will free the LRU objects using the delete or delete[] operator.
//
// Put() puts an object into a cache and returns an opaque struct (handle) to
// the user of the cache.  The user can subsequently pass the same opaque struct
// to Get() to retrieve the same cached object.  Get() returns NULL if an object
// was evicted in which case the user must recompute the object.
//
// The cache code uses the opaque struct itself to cache the object.
// A list is used solely for maintaining recently-used order.
// The first link in the list (head) is the most-recently-used one.
// The list is never searched.
////////////////////////////////////////////////////////////////////////////////

enum { CACHE_SCALAR = DLIST_SCALAR, CACHE_ARRAY = DLIST_ARRAY };
typedef unsigned int cacheSize_e;
template <class T> struct CacheLink;
template <class T> class Cache;

// Opaque struct returned to user of cache.
template<class T>
class CacheOpq
{
public:
			CacheOpq( void ) { lnkCache = NULL;        SET_TYPESIG( this, TYPESIG_LIB_CACHE_OPQ ); };
			~CacheOpq()      { lnkCache = NULL; INVALIDATE_TYPESIG( this, TYPESIG_LIB_CACHE_OPQ ); };
private:
	CacheLink<T>*	lnkCache;
public:
	DECL_TYPESIG( TYPESIG_LIB_CACHE_OPQ )
	friend class Cache<T>;
};

// Internal cache struct (not for use outside of cache code).
template<class T>
struct CacheLink
{
	T*						obj;		// cached object
	CacheOpq<T>*			opq;		// pointer to opaque struct (returned to user)
	Dlink< CacheLink<T> >*	lnkList;	// pointer to link in list (to maintain recently-used order)
	DECL_TYPESIG_AUTO( CacheLink, TYPESIG_LIB_CACHE_LINK )
};

// Cache class template.
template<class T>
class Cache
{
public:
				Cache( cacheSize_e size, bool cacheOfArrays );
				~Cache();
	void		Put( CacheOpq<T>* opq, T* obj );
	T*			Get( CacheOpq<T>* opq );

private:
	void		Shrink( void );

	cacheSize_e				cnt;		// count of cached elements
	cacheSize_e				limit;		// cache is shrunk to this many elems when cnt becomes greater
	Dlist< CacheLink<T> >*	list;		// maintains recently-used order
	unsigned char			arrays;		// true if cached elements are arrays -- delete[] used

public:
	DECL_TYPESIG( TYPESIG_LIB_CACHE )
	friend bool CacheTest( void );
};

/*****************************************************************************
 * Constructor.
 *****************************************************************************/
template<class T>
Cache<T>::Cache( cacheSize_e size, bool cacheOfArrays )
{
	SET_TYPESIG( this, TYPESIG_LIB_CACHE );
	cnt		= 0;
	limit	= MAX(size,10);
	list	= new Dlist< CacheLink<T> >( DLIST_SCALAR );
	arrays	= cacheOfArrays;
CHECK_TYPESIG( list, TYPESIG_LIB_DLIST );
	return;
}

/*****************************************************************************
 * Destructor.
 *****************************************************************************/
template<class T>
Cache<T>::~Cache()
{
assert( cnt >= 0 );
CHECK_TYPESIG( list, TYPESIG_LIB_DLIST );
	while ( cnt > 0 ) Shrink();
	delete list;
INVALIDATE_TYPESIG( this, TYPESIG_LIB_CACHE );
	return;
}

/*****************************************************************************
 * Shrink the cache by evicting the LRU object.
 * The last link in the list (tail) references it.
 *****************************************************************************/
template<class T>
void
Cache<T>::Shrink( void )
{
	DebugVerboseMsg( "shrinking cache [cnt=%d] \n", cnt );
assert( cnt > 0 );
CHECK_TYPESIG( list, TYPESIG_LIB_DLIST );

	// Delete the cached object (CacheLink->obj).
	// Nullify the pointer to this CacheLink that
	// is stored in the user's CacheOpq struct.
	// Then unlink and delete the CacheLink struct.
	--cnt;
	Dlink< CacheLink<T> >* lnkList = list->Last();	// get list's tail
CHECK_TYPESIG( lnkList, TYPESIG_LIB_DLINK );
	CacheLink<T>* lnkCache = lnkList->obj;
CHECK_TYPESIG( lnkCache, TYPESIG_LIB_CACHE_LINK );
	if ( arrays )				// delete cached object appropriately
		delete[] lnkCache->obj;
	else
		delete lnkCache->obj;
	CacheOpq<T>* opq = lnkCache->opq;
CHECK_TYPESIG( opq, TYPESIG_LIB_CACHE_OPQ );
assert( opq->lnkCache == lnkCache );
	opq->lnkCache = NULL;		// update user's opaque struct (CacheOpq)
	list->Unlink( lnkList );	// remove from list -- Unlink() deletes lnkList
	delete lnkCache;			// delete CacheLink struct -- keep CacheOpq
	return;
}

/*****************************************************************************
 * Put a C++ object into the cache.
 *****************************************************************************/
template<class T>
void
Cache<T>::Put( CacheOpq<T>* opq, T* obj )
{
CHECK_TYPESIG( this, TYPESIG_LIB_CACHE );
CHECK_TYPESIG( opq,  TYPESIG_LIB_CACHE_OPQ );
CHECK_TYPESIG( list, TYPESIG_LIB_DLIST );
assert( obj );
	if ( UX(opq->lnkCache != NULL) ) return;		// already cached?
	++cnt;
	CacheLink<T>* lnkCache = new CacheLink<T>;
	lnkCache->obj     = obj;						// fill CacheLink struct
	lnkCache->opq     = opq;
	lnkCache->lnkList = list->Prepend( lnkCache );	// prepend CacheLink to head of list
	opq->lnkCache = lnkCache;						// point user's CacheOpq struct to CacheLink
	if ( cnt > limit ) Shrink();					// after adding, shrink cache if necessary
assert( cnt <= limit );
	return;
}

/*****************************************************************************
 * Get a C++ object from the cache.
 *****************************************************************************/
template<class T>
T*
Cache<T>::Get( CacheOpq<T>* opq )
{
CHECK_TYPESIG( this, TYPESIG_LIB_CACHE );
CHECK_TYPESIG( opq,  TYPESIG_LIB_CACHE_OPQ );
CHECK_TYPESIG( list, TYPESIG_LIB_DLIST );

	// The opaque struct itself records if the object is cached.
	// This simple pointer test is what makes this cache quick.
	if ( EX(opq->lnkCache != NULL) )
	{
		// Move the cached object to the head of the list
		// to make it the most-recently-used and to age the others.
		// cnt isn't decremented as object is still in the cache.
CHECK_TYPESIG( opq->lnkCache, TYPESIG_LIB_CACHE_LINK );
		list->Unlink( opq->lnkCache->lnkList );
		opq->lnkCache->lnkList = list->Prepend( opq->lnkCache );
CHECK_TYPESIG( opq->lnkCache->lnkList, TYPESIG_LIB_DLINK );
assert( opq->lnkCache->obj );
		return opq->lnkCache->obj;
	}
	else
	{
		return NULL; // not in cache
	}
}

#endif /* GFX_CACHE_H */

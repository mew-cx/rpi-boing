/*****************************************************************************
 * $Id: gfx_iter.h,v 1.12 2004/03/10 05:33:47 jimb Exp $
 * Author:	Jim E. Brooks  http://www.jimbrooks.org
 * Desc:	Iterator class template.
 * Notes:
 * LEGAL:	COPYRIGHT (C) JIM E. BROOKS 2004
 *          THIS SOURCE CODE IS RELEASED UNDER THE TERMS
 *          OF THE GNU GENERAL PUBLIC LICENSE (GPL).
 *****************************************************************************/

#ifndef GFX_ITER_H
#define GFX_ITER_H 1

const void* IterEmptyGet( void* obj, int n );

////////////////////////////////////////////////////////////////////////////////
// Iterator class template.
//
// To use an iterator, define your Get() function and instantiate the template.
// The core of the iterator is the user-defined Get() function that retrieves or
// computes the next datum.  Pass to Iter()/Init() a pointer to your Get()
// and your user-defined pointer.  Get() can use the passed count arg (cnt)
// to initialize itself when cnt=0 and to identify which datum to retrieve.
// Get() can use the user-defined pointer to point to an object that is the source
// of the data.  Get() should return NULL after it has exhausted its data.
//
// The form of an iteration loop:
//     for ( iter->Begin(); iter->Go(); iter->Next() )
//     {
//         MyData* data = iter->Data();
//     }
//
// The iteration loop won't execute if the iterator is empty
// (indicated by Get() returning NULL).
// An interator doesn't need to be reinitialized to replay data.
// Calling Begin() will reset the iterator back to the first datum.
// Go() returns false to stop the "for" loop (false means no-go).
// Data() returns the datum on every iteration.
// Data should always return a non-NULL value because Go() should
// break the loop before Data() would ever have nothing to return.
// Next() prepares the iterator for the next datum and can be thought
// of as the interator's increment operator.
//
template <class T>
class Iter
{
public:
typedef const T* (*iterGetFunc_t)( void* obj, int n );
					Iter( void ); // constructs an empty iterator which won't ever iterate
					Iter( void* obj_, iterGetFunc_t get_ );
					~Iter() { INVALIDATE_TYPESIG(this,TYPESIG_LIB_ITER); };
	void			Init( void* obj_, iterGetFunc_t get_ );
	inline void		Begin( void );
	inline bool		Go( void );
	inline void		Next( void );
	inline T*		Data( void );

private:
	void*			obj;	// pointer to user-defined data
	iterGetFunc_t	Get;	// pointer to function that gets every datum
	int				cnt;	// iteration count
	T*				data;	// data to be returned on every iteration

public:
	DECL_TYPESIG( TYPESIG_LIB_ITER )
};

/*****************************************************************************
 * Constructor.
 *****************************************************************************/
// Constructs an empty iterator which won't ever iterate.
template<class T>
Iter<T>::Iter( void )
{
	SET_TYPESIG( this, TYPESIG_LIB_ITER );
	Init( NULL, (iterGetFunc_t)IterEmptyGet );
	return;
}
template<class T>
Iter<T>::Iter( void* obj_, iterGetFunc_t get_ )
{
	SET_TYPESIG( this, TYPESIG_LIB_ITER );
	Init( obj_, get_ );
	return;
}

/*****************************************************************************
 * Initialize iterator.
 * Pass a pointer to the object of whose data the iteration is applied to and
 * a pointer to the Get() function (that the iterator calls) that retrieves
 * each datum from the object.
 *****************************************************************************/
template<class T>
void
Iter<T>::Init( void* obj_, iterGetFunc_t get_ )
{
CHECK_TYPESIG( this, TYPESIG_LIB_ITER );
	obj = obj_;
	Get = get_;
	cnt = 0;
	data = NULL;
	return;
}

/*****************************************************************************
 * Begin iteration.
 *****************************************************************************/
template<class T>
void
Iter<T>::Begin( void )
{
CHECK_TYPESIG( this, TYPESIG_LIB_ITER );
	cnt = 0; // in case Begin() is called again
	Next();
	return;
}

/*****************************************************************************
 * Return false (false to stop a "for" loop) at the end of iteration.
 *****************************************************************************/
template<class T>
bool
Iter<T>::Go( void )
{
CHECK_TYPESIG( this, TYPESIG_LIB_ITER );
	return data != NULL; // return true while data is ready
}

/*****************************************************************************
 * Prepare the next datum to be returned by Data().
 *****************************************************************************/
template<class T>
void
Iter<T>::Next( void )
{
CHECK_TYPESIG( this, TYPESIG_LIB_ITER );
	data = (T*) Get( obj, cnt );
	++cnt;
	return;
}

/*****************************************************************************
 * Return the current datum.
 *****************************************************************************/
template<class T>
T*
Iter<T>::Data( void )
{
CHECK_TYPESIG( this, TYPESIG_LIB_ITER );
	return data;
}

#endif /* GFX_ITER_H */

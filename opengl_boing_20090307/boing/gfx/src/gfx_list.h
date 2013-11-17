/*****************************************************************************
 * $Id: gfx_list.h,v 1.31 2004/03/23 15:25:59 jimb Exp $
 * Author:	Jim E. Brooks  http://www.jimbrooks.org
 * Desc:	Double-link list (C++ template).
 * Notes:	"link" is UNIX syscall, so "lnk" is written.
 * LEGAL:	COPYRIGHT (C) JIM E. BROOKS 2004
 *          THIS SOURCE CODE IS RELEASED UNDER THE TERMS
 *          OF THE GNU GENERAL PUBLIC LICENSE (GPL).
 *****************************************************************************/

#ifndef DLIST_TEMPLATE
#define DLIST_TEMPLATE 1

////////////////////////////////////////////////////////////////////////////////
// List template class based on circular double-links with a sentinel.
//
// Linked objects must be allocated by the C++ new operator since
// the list destructor uses the C++ delete operator to free linked objects
// (in addition to the list itself).
//
// Traversing the list directly using the Dlink structure is discouraged.
// Rather, for reliability and in case of the list implementation is changed,
// use the Dlist methods such as For*() and Next().
//
// A sentinel is omnipresent, never unlinked, and excluded by
// the iterator helpers For*().
// A list is empty when the sole link is the sentinel.
// The head pointer always points to the sentinel (as head is never reassigned)
// but the tail pointer will change.
// The first actual link follows the sentinel and is pointed to by head->next.
////////////////////////////////////////////////////////////////////////////////
enum { DLIST_SCALAR, DLIST_ARRAY };
template <class T> class Dlist;

template<class T>
class Dlink
{
private:
	Dlink<T>*		prev PACKMEMB;
	Dlink<T>*		next PACKMEMB;
public:
	T*				obj  PACKMEMB;
	DECL_TYPESIG_AUTO( Dlink, TYPESIG_LIB_DLINK )
	friend class Dlist<T>;
};

template<class T>
class Dlist
{
public:
////////////////////////////////////////////////////////////////////////////////
// Dlist()      Pass DLIST_SCALAR or DLIST_ARRAY if linked objects are scalars or arrays.
//              Controls whether ~Dlist() calls delete or delete[] to free linked objects.
// ~Dlist()		Deletes a list and ALL OBJECTS STILL LINKED IN THE LIST.
// Prepend()	Prepend new link before the head of the list (new link becomes the first one).
// Append()		Append  new link after  the tail of the list (new link becomes the last  one).
// Unlink()		Remove link out of list (but does not delete the linked object).
// Empty()		True if list is empty.                           (iterator helpers instead is recommended).
// First()		Return pointer to the first link or NULL if none (iterator helpers instead is recommended).
// Last()		Return pointer to the last  link or NULL if none (iterator helpers instead is recommended).
// Next()		Return pointer to following link or NULL if none (iterator helpers instead is recommended).
// Iterator helpers to write a "for" loop to iterate thru the list.
// Can cope with loops that unlink the current link.
//	for ( Dlink<MyClass>* lnk = list->ForStart();
//        list->ForTest(lnk);
//        list->ForNext(lnk) )
////////////////////////////////////////////////////////////////////////////////
						Dlist( bool listOfArrays ); // pass DLIST_SCALAR or DLIST_ARRAY
						~Dlist();
	inline Dlink<T>*	Prepend( T* obj );
	inline Dlink<T>*	Append( T* obj );
	inline void			Unlink( Dlink<T>* lnk );
	inline bool			Empty( void ) { return head == tail; }; // empty if sentinel is the only link
	inline Dlink<T>*	First( void ) { return head == tail ? NULL : head->next; };
	inline Dlink<T>*	Last( void )  { return head == tail ? NULL : tail;       };
	inline Dlink<T>*	Next( Dlink<T>* lnk ) { return lnk->next != head ? lnk->next : NULL; };
	// lnkNext remembers the next link in case a loop unlinks the current link.
	inline Dlink<T>*	ForStart( void )          { return head->next; };
	inline bool			ForTest( Dlink<T>*& lnk ) { lnkNext = lnk->next; return lnk != head; };
	inline void			ForNext( Dlink<T>*& lnk ) { lnk = lnkNext; };

private:
	Dlink<T>*		head;		// always points to the sentinel
	Dlink<T>*		tail;		// can be changed Append()/Prepend()
	Dlink<T>*		lnkNext;	// used by For*()
	unsigned char	arrays;		// true if linked objects are arrays (false if scalar)

public:
	DECL_TYPESIG( TYPESIG_LIB_DLIST )
	friend bool TestList( void );
};

/*****************************************************************************
 * Constructor.
 *****************************************************************************/
template<class T>
Dlist<T>::Dlist( bool listOfArrays )
{
	SET_TYPESIG( this, TYPESIG_LIB_DLIST );

	// Make the sentinel which points to itself.
	head = tail = new Dlink<T>;
	head->obj = NULL;
	head->prev = head->next = tail->prev = tail->next = head;
	arrays = listOfArrays;
	return;
}

/*****************************************************************************
 * Destructor.
 * Delete all objects in the list and the list itself.
 *****************************************************************************/
template<class T>
Dlist<T>::~Dlist()
{
	Dlink<T>* lnk = head->next;
	while ( lnk != lnk->next )
	{
assert( lnk && lnk->next && lnk->obj );
CHECK_TYPESIG( lnk, TYPESIG_LIB_DLINK );
		Dlink<T>* next = lnk->next;
		if ( arrays )			// delete the object pointed to by this link
			delete[] lnk->obj;
		else
			delete lnk->obj;
		Unlink( lnk );			// delete the link struct itself
		lnk = next;
	}
assert( head == tail );
INVALIDATE_TYPESIG( head, TYPESIG_LIB_DLINK ); // head==tail==sentinel
INVALIDATE_TYPESIG( this, TYPESIG_LIB_DLIST );
	return;
}

/*****************************************************************************
 * Make new link for object and prepend link to list.
 * The new link will be inserted immediately after the head/sentinel.
 * But effectively the new link is the first link as the loop helper ForStart()
 * will start an iteration at the link following the head.
 *****************************************************************************/
template<class T>
Dlink<T>*
Dlist<T>::Prepend( T* obj )
{
CHECK_TYPESIG( head, TYPESIG_LIB_DLINK );
CHECK_TYPESIG( tail, TYPESIG_LIB_DLINK );

	// As a sentinel is used, head/tail always point to something,
	// and there is always a previous and a next link.

	// New link struct for this object.
	Dlink<T>* lnk = new Dlink<T>;

	// Insert new link between head/sentinel link and second link.
	if ( UX(head == tail) ) tail = lnk; // if list is empty, new link is the tail
	lnk->next  = head->next;
	lnk->prev  = head;
	lnk->obj   = obj;
	head->next = lnk->next->prev = lnk;
	return lnk;
}

/*****************************************************************************
 * Make new link for object and append link to list.
 *****************************************************************************/
template<class T>
Dlink<T>*
Dlist<T>::Append( T* obj )
{
CHECK_TYPESIG( head, TYPESIG_LIB_DLINK );
CHECK_TYPESIG( tail, TYPESIG_LIB_DLINK );

	// As a sentinel is used, head/tail always point to something,
	// and there is always a previous and a next link.

	// New link struct for this object.
	Dlink<T>* lnk = new Dlink<T>;

	// Link to tail and supplant it.
	lnk->prev  = tail;
	lnk->next  = tail->next; // tail->next = head = sentinel
	lnk->obj   = obj;
	tail->next = lnk;
	tail       = lnk;
	return lnk;
}

/*****************************************************************************
 * Unlink object's link out of list.
 * Does NOT delete the object.
 *****************************************************************************/
template<class T>
void
Dlist<T>::Unlink( Dlink<T>* lnk )
{
CHECK_TYPESIG( head, TYPESIG_LIB_DLINK );
CHECK_TYPESIG( tail, TYPESIG_LIB_DLINK );
CHECK_TYPESIG( lnk,  TYPESIG_LIB_DLINK );

	// As a sentinel is used, head/tail always point to something,
	// and there is always a previous and a next link.
	if ( UX(tail == lnk) ) tail = lnk->prev; // update tail if unlinking it
	lnk->prev->next = lnk->next;		 // preceding --> following
	lnk->next->prev = lnk->prev;		 // following --> preceding
	delete lnk;
}

#endif /* DLIST_TEMPLATE */

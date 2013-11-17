/******************************************************************************
 * $Id: gfx_iter.cc,v 1.2 2004/03/10 06:05:25 jimb Exp $
 * Author:	Jim E. Brooks  http://www.jimbrooks.org
 * Desc:	Iterator class template.
 * Notes:
 * LEGAL:	COPYRIGHT (C) JIM E. BROOKS 2004
 *          THIS SOURCE CODE IS RELEASED UNDER THE TERMS
 *          OF THE GNU GENERAL PUBLIC LICENSE (GPL).
 ******************************************************************************/

#define GFX_ITER_CC 1
#include "gfx.h"

/*****************************************************************************
 * Default Get() callback for an empty iterator.
 *****************************************************************************/
const void*
IterEmptyGet( UNUSED void* obj, UNUSED int n )
{
	return NULL;
}

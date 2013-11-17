#!/bin/sh
# $Id: cc-config2.sh,v 1.2 2004/03/11 16:04:10 jimb Exp $
# Author: Jim Brooks  http://www.jimbrooks.org
# (See cc-config.sh)
#-------------------------------------------------------------------------------

# [2007/05 removed aggressive compiler optimizations to maximize portability]

#===============================================================================
# Determine header file include directories.
#===============================================================================

# --- UNIX ---
DIR=/usr/local/include
if [ -f $DIR/GL/glut.h ] || [ -f $DIR/GL/gl.h ]
then
   echo -n "-I$DIR "
   exit 0
fi

DIR=/usr/X11R6/include
if [ -f $DIR/GL/glut.h ] || [ -f $DIR/GL/gl.h ]
then
   echo -n "-I$DIR "
   exit 0
fi

DIR=/usr/X11/include
if [ -f $DIR/GL/glut.h ] || [ -f $DIR/GL/gl.h ]
then
   echo -n "-I$DIR "
   exit 0
fi

DIR=/usr/include
if [ -f $DIR/GL/glut.h ] || [ -f $DIR/GL/gl.h ]
then
   # Just exit, gcc will nag about /usr/include
   #echo -n "-I$DIR "
   exit 0
fi

# --- BeOS ---
DIR=/boot/develop/headers/be/opengl
if [ -f $DIR/GL/glut.h ] || [ -f $DIR/GL/gl.h ]
then
   echo -n "-I$DIR "
   exit 0
fi

# Oops.
echo "cc-config.sh: could not determine header file include dirs"
exit 1

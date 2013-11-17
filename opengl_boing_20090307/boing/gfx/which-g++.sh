#!/bin/sh
# $Id: which-g++.sh,v 1.6 2004/02/03 03:00:05 jimb Exp $
# Author:	Jim E. Brooks  http://www.jimbrooks.org
#
# Pick the best g++ version.
# Eg, Debian 3 provides both "g++" (2.9.x) and "g++-3.0".
#-------------------------------------------------------------------------------

if [ "`which g++3 2>/dev/null`" ]; then
    echo "g++3"
elif [ "`which g++-3.0 2>/dev/null`" ]; then
	echo "g++-3.0"
else
	echo "g++"
fi

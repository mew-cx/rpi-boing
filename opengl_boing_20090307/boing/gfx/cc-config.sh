#!/bin/sh
# $Id: cc-config.sh,v 1.13 2004/02/03 03:00:05 jimb Exp $
# Author: Jim Brooks  http://www.jimbrooks.org
#
# Print to stdout appropriate and optimal GNU g++ options
# for compiling OpenGL programs on a system with GNU tools.
#
# Args (these are optional and override auto/default settings):
# * -mfpmath <arg>
# * -march <arg>
#   Currently this is meant only for selecting siblings within the same arch
#   (eg k6 vs. k6-2) or otherwise optimization flags that are auto-selected
#   will be inappropriate.
# * name of gcc/g++ is assumed if arg is none of the above
#-------------------------------------------------------------------------------

# To speed compiling, minimize running the full script by caching its results.

SCRIPT=$0                     # $0 holds the full pathname of a script
PATH_SCRIPT=${SCRIPT%/*}      # remove the filename portion
PATH_SCRIPT=${PATH_SCRIPT}/   # append trailing "/"

CACHE=${PATH_SCRIPT}/out/cc-config.cache
if [ -f ${CACHE} ]; then
	cat ${CACHE}
	exit 0
else
	${PATH_SCRIPT}/cc-config2.sh | tee ${CACHE} 2>/dev/null
	exit 0
fi

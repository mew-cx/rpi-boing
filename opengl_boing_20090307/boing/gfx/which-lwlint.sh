#!/bin/sh
# $Id: which-lwlint.sh,v 1.3 2004/02/11 17:02:47 jimb Exp $
# Author: Jim Brooks  http://www.jimbrooks.org
# If my light-weight lint (lwlint) tool is available.
# http://www.jimbrooks.org/tools/
#-------------------------------------------------------------------------------

#if ( which lwlint 2>/dev/null >/dev/null ); then # which always succeeds on cygwin
WHICH=`which lwlint 2>/dev/null`
if [ "${WHICH}" != "" ]; then
	echo "lwlint -all"
fi
exit 0

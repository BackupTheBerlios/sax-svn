#!/bin/bash
# Copyright (c) 2000 SuSE GmbH Nuernberg, Germany.  All rights reserved.
#
# Authors: Marcus Schaefer <ms@suse.de>
#
# this script use the following variable(s):
# 
# - $BUILD_DIST
#
# functions...
# --------------
function Setup80NeedForBuild {
	cp sax2.spec sax2.spec.orig
	cat sax2.spec.orig | \
		sed -e s@XFree86-driver-options@xf86driver-options@\g | \
		sed -e s@XFree86-server@xloader@\g \
	> sax2.spec
}

function SetupS390NeedForBuild {
	cp sax2.spec sax2.spec.orig
	cat sax2.spec.orig | \
		sed -e s@XFree86-server@@\g | \
		sed -e s@XFree86-driver-options@@\g \
	> sax2.spec
}

case $1 in
	8.0*)
		Setup80NeedForBuild
	;;

	8.1*)
		Setup80NeedForBuild
	;;

	s390)
		SetupS390NeedForBuild
	;;

	*)
	;;
esac

# clean up
# ---------
rm -f sax2.spec.org
rm -f sax2.dif.orig

#!/bin/sh
# Copyright (c) 2001 SuSE GmbH Nuernberg, Germany.  All rights reserved.
#
# Author: Marcus Schaefer <sax@suse.de>, 2001
# SaX (sax) configuration level [xapi]
#
# CVS ID:
# --------
# Status: Up-to-date
#
export QMAKESPEC=/usr/lib/qt3/mkspecs/linux-g++/
/usr/lib/qt3/bin/qmake -makefile -unix -o Makefile xapi.pro

arch=`arch`
if [ "$arch" = "x86_64" ];then
	cat Makefile | \
		sed -e s"@-fPIC@-fPIC -fno-strict-aliasing@"g \
			-e s"@X11R6/lib@X11R6/lib64@"g \
	> Makefile.new
	mv Makefile.new Makefile
fi

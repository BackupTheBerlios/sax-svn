#!/bin/sh
# Copyright (c) 2000 SuSE GmbH Nuernberg, Germany.  All rights reserved.
#
# Author: Marcus Schaefer <sax@suse.de>, 2001
# check for Extensions section
#
# CVS ID:
# --------
# Status: Up-to-date
#
file=/usr/X11R6/lib/libxf86config.a
if [ ! -f $file ];then
	file=/usr/X11R6/lib64/libxf86config.a
fi
nm $file | grep -q xf86parseExtensions
if [ $? = 0 ];then
	echo "HAVE_EXTENSIONS_SECTION"
else
	echo "NO_EXTENSIONS_SECTION"
fi

#!/bin/sh
# Copyright (c) 2000 SuSE GmbH Nuernberg, Germany.  All rights reserved.
#
# Author: Marcus Schaefer <sax@suse.de>, 2004
# check for XFree86 or Xorg loader
#
# CVS ID:
# --------
# Status: Up-to-date

if [ -f "/usr/X11R6/bin/Xorg" ];then
	echo -DXORG_LOADER
else
	echo -DXFree86_LOADER
fi

#!/bin/sh
# Copyright (c) 2000 SuSE GmbH Nuernberg, Germany.  All rights reserved.
#
# Author: Marcus Schaefer <sax@suse.de>, 2001
# check for libhd version
#
# CVS ID:
# --------
# Status: Up-to-date
#
if [ -f "/etc/SuSE-release" ];then
	VERSION=`cat /etc/SuSE-release | head -n 1 | cut -f3 -d" " | tr -d .`
else
	VERSION=81
fi
test "$VERSION" -ge "73" && echo "DDC_SUPPORT" || echo "NO_DDC_SUPPORT"

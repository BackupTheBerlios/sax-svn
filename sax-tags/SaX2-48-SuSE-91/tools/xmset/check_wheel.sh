#!/bin/sh
# Copyright (c) 2000 SuSE GmbH Nuernberg, Germany.  All rights reserved.
#
# Author: Marcus Schaefer <sax@suse.de>, 2001
# check for libhd version
#
# CVS ID:
# --------
# Status: Up-to-date

VERSION=`rpm -q hwinfo | cut -f2 -d- | tr -d .`
test "$VERSION" -ge "202" && echo "-DWHEEL_SUPPORT" || echo "-DNO_WHEEL_SUPPORT"

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
VERSION=`rpm -q hwinfo --qf %{VERSION} | tr -d .`
test "$VERSION" -ge "5" && echo "WHEEL_SUPPORT" || echo "NO_WHEEL_SUPPORT"

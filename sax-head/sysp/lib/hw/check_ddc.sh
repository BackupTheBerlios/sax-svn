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
VERSION=`rpm -q hwinfo | cut -f2 -d- | tr -d . | cut -c1`
test "$VERSION" -ge "5" && echo "DDC_SUPPORT" || echo "NO_DDC_SUPPORT"

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
export QMAKESPEC=$QTDIR/mkspecs/linux-g++/
$QTDIR/bin/qmake -makefile -unix -o Makefile xfine.pro

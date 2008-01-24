#!/bin/sh
# Copyright (c) 2001 SuSE GmbH Nuernberg, Germany.  All rights reserved.
#
# Author: Marcus Schaefer <sax@suse.de>, 2001
# SaX (sax) configuration level [xcmd]
#
# CVS ID:
# --------
# Status: Up-to-date
#
xport QMAKESPEC=/usr/share/qt4/mkspecs/linux-g++/
`which qmake` -makefile -unix -o Makefile xcmd.pro


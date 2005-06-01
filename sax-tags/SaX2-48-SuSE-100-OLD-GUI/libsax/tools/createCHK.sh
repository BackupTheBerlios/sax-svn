#!/bin/sh
# Copyright (c) 2005 SuSE GmbH Nuernberg, Germany.  All rights reserved.
#
# Author: Marcus Schaefer <sax@suse.de>, 2005
# libsax script: create VNC password using the vncpasswd program
# --
#
# CVS ID:
# --------
# Status: Up-to-date
#
LC_ALL="POSIX"

if [ -x /usr/share/sax/profile/$1.sh ];then
	status=`/usr/share/sax/profile/$1.sh check`
	if [ $status = "dual" ];then
		exit 0
	fi
	exit 1
else
	exit 0
fi

#!/usr/bin/perl
# Copyright (c) 1996 SuSE GmbH Nuernberg, Germany.  All rights reserved.
#
# Author: Marcus Schaefer <sax@suse.de>, 2003
# sysp script: get vesa BIOS
# --
# This script is called via the 3D server detection
# to lookup the VESA BIOS
#
# CVS ID:
# --------
# Status: Up-to-date
#
use strict;
use Env;

my $bios = qx (/usr/sbin/hwinfo --bios | grep "VESA BIOS");
if ($bios =~ /VESA BIOS/) {
	print "1\n"; exit 1;
} else {
	print "0\n"; exit 0;
}

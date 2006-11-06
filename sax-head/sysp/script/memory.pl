#!/usr/bin/perl
# Copyright (c) 2006 SUSE LINUX Products GmbH. All rights reserved.
#
# Author: Marcus Schaefer <sax@suse.de>, 2006
# sysp script: check videoram
# --
# This script is called via the server stuff detection
# to check for the amount of videoram
#
# CVS ID:
# --------
# Status: Up-to-date
#
use strict;
use Env;

my $video = qx (X -probeonly -verbose :98 2>&1 | grep -i videoram);
if ($video =~ /(\d+) kByte/) {
	print "$1\n";
} else {
	print "0\n";
}

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

my $param = "-probeonly -logverbose 255 -verbose -xf86config $ARGV[0]";
my $video = qx (X $param :99 2>&1 | grep -i videoram);
my $code = $? >> 8;
if ($code != 0 ) {
	$video = qx (X $param :99 2>&1 | grep -i memory:);
}
if ($video =~ /(\d+) kByte/) {
	print "$1\n";
} else {
	print "0\n";
}

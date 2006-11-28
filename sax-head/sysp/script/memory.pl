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

# /.../
# Workaround for Bug: #220197
# Unfortunately X11 doesn't provide a clean interface to detect
# the amount of videoram installed on the graphics card. According
# to this it is needed to initialize the X-Server and check the log
# to find out that value. The following workaround will switch off
# the memory detection because there are bad side effects while
# calling X11 with the option -probeonly. The reason for this
# workaround is the broken X11 implementation and I'm really hoping
# this will be fixed as soon as possible
# ----
print "0\n";
exit 0;
# ----

my $param = "-probeonly -logverbose 255 -verbose -xf86config $ARGV[0]";
my $video = qx (X $param :99 2>&1 | grep -i videoram);
if ($video =~ /(\d+) kByte/) {
	print "$1\n";
} else {
	print "0\n";
}

#!/usr/bin/perl
# Copyright (c) 1996 SuSE GmbH Nuernberg, Germany.  All rights reserved.
#
# Author: Marcus Schaefer <sax@suse.de>, 2000
# sysp script: check currently 3D status
# --
# 0 -> 3D activated
# 1 -> 3D disabled
#
# CVS ID:
# --------
# Status: Up-to-date
#
use strict;
use Env;

#====================================
# Globals...
#------------------------------------
my %spec;
$spec{InitFlag}  = "/var/cache/sax/files/reinit";

my $answer = $ARGV[0];
if (-f $spec{InitFlag}) {
#================================================
# There is an init flag file which indicate that
# a new detection is needed. 
#------------------------------------------------
if ($answer eq "yes") {
	#============================================
	# The 3D answer is set to "yes" therefore
	# 3D status is checked without looking at
	# the config file because it's not
	# written at this time
	#--------------------------------------------
	qx(/usr/bin/3Ddiag-result --omit-config);
} else {
	#============================================
	# The 3D answer is set to "no" therefore
	# 3D must be disabled
	#--------------------------------------------
	print "1\n"; exit(1);
}
} else {
	#============================================
	# No init flag file therefore check with
	# 3Ddiag-result
	#--------------------------------------------
	qx(/usr/bin/3Ddiag-result);
}
my $return = $? >> 8;
print "$return\n";
exit (
    $return
);

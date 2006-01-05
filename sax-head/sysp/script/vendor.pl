#!/usr/bin/perl
# Copyright (c) 2003 SuSE GmbH Nuernberg, Germany.  All rights reserved.
#
# Author: Marcus Schaefer <sax@suse.de>, 2003
# sysp script: read vendor string from X11 driver modules
# --
# This script is called via the 3D scan
#
# CVS ID:
# --------
# Status: Up-to-date
#
use strict;
use Env;

#---[ vendorName ]----#
sub vendorName {
#-----------------------------------------------
# call strings and filter the given object data
# to check for one of the vendor names listed
# in @vendor
#
	my $driver = $_[0];
	my @vendor = (
		"X.Org Foundation",
		"The XFree86 Project",
		"NVIDIA Corporation",
		"FireGL - ATI Technologies Inc."
	);
	my $drvfile = "/usr/X11R6/lib/modules/drivers/".$driver."_drv.*";
	my $objdump = qx (/usr/bin/strings $drvfile 2>/dev/null);
	foreach my $vname (@vendor) {
	if ($objdump =~ /$vname/) {
		return $vname;
	}
	}
	return ("<none>");
}

my $vendor = vendorName ( $ARGV[0] );
print "$vendor\n";
exit (0);

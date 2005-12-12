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
# call xsload and filter the given driver name
# if it exist return the vendor string otherwhise
# return <none>
#
	my $driver = $_[0];
	# FIXME
	# workaround for Bug: #137374, to be removed ASAP
	if ($driver eq "nvidia") {
		return "NVIDIA Corporation";
	}
	my $xsload = "/usr/X11R6/bin/xsload";
	if (! -f $xsload) {
		return ("<none>");
	}
	my $data = qx ($xsload -vendor);
	my @list = split(/\n/,$data);
	foreach my $line (@list) {
	if ($line =~ /(.*):(.*)/) {
		if ($driver eq $1) {
			return $2;
		}
	}	
	}
	if ($driver =~ /^nv/) {
		if (installed ("XFree86")) {
			return ("The XFree86 Project");
		} else {
			return ("X.Org Foundation");
		}
	} else {
		return ("<none>");
	}
}

sub installed {
	my $pac = $_[0];
	qx (rpm -q $pac);
	my $code = $? >> 8;
	if ($code >= 1) {
		return undef;
	}
	return 1;
}

my $vendor = vendorName ( $ARGV[0] );
print "$vendor\n";
exit (0);

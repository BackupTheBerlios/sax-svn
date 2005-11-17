#!/usr/bin/perl

use lib '/usr/share/sax/profile';

use strict;
use Profile;

#====================================
# DualHead check
#------------------------------------
if ($ARGV[0] eq "check") {
	ProfileNVDualCheck();
	exit 0;
}

#====================================
# Init profile script
#------------------------------------
my $profile = ProfileInitScript();

#====================================
# Do the profile adaptions...
#------------------------------------
if (ProfileIsXOrgVendor ("nvidia")) {
	open (FD,">",$profile) ||
		die "NVidia_DualHead: Can't open $profile: $!";
	print FD "Desktop -> [X] ->  CalcModelines = no\n";
	print FD "Monitor -> [X] ->  CalcAlgorithm = XServerPool\n";
	close FD;
}

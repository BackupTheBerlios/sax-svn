#!/usr/bin/perl

use lib '/usr/share/sax/profile';

use strict;
use Profile;

#====================================
# DualHead check
#------------------------------------
if ($ARGV[0] eq "check") {
	print "dual\n";
	exit 0;
}

#====================================
# Init profile script
#------------------------------------
my $profile = ProfileInitScript();

#====================================
# Do the profile adaptions...
#------------------------------------
open (FD,">>",$profile) ||
	die "Matrox_DualHead: Can't open $profile: $!";
#====================================
# check secondary DDC data
#------------------------------------
my %data = ProfileGetDDC2Data();
foreach my $key (keys %data) {
	my $val = $data{$key};
	SWITCH: for ($key) {
		/^Model/      && do {
			print FD "Monitor -> [X+1] -> ModelName = $val\n";
			last SWITCH;
		};
		/^Vendor/     && do {
			print FD "Monitor -> [X+1] -> VendorName = $val\n";
			last SWITCH;
		};
		/^Resolution/ && do {
			foreach my $depth (8,15,16,24) {
				print FD "Screen -> [X+1] -> Depth->$depth->Modes = $val\n";
			}
			last SWITCH;
		};
		/^Hsync/      && do {
			print FD "Monitor -> [X+1] -> HorizSync = $val\n";
			last SWITCH;
		};
		/^Vsync/      && do {
			print FD "Monitor -> [X+1] -> VertRefresh = $val\n";
			last SWITCH;
		};
		/^Size/       && do {
			print FD "Monitor -> [X+1] -> DisplaySize = $val\n";
			last SWITCH;
		};
	}
}
close FD;

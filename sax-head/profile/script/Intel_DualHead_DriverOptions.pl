#!/usr/bin/perl

use lib '/usr/share/sax/profile';

use strict;
use Profile;

#====================================
# Profile name...
#------------------------------------
my $name = "Intel_DualHead_DriverOptions";

#====================================
# Init profile script
#------------------------------------
my $profile = ProfileInitScript();

#====================================
# Do the profile adaptions...
#------------------------------------
my $mlayout = ProfileIntelSetupMonitorLayout ($profile);

#====================================
# Do the profile adaptions...
#------------------------------------
if (ProfileIsNoteBookHardware()) {
	open (FD,">",$profile) ||
		die "$name: Can't open $profile: $!";
	my ($x,$y) = ProfileGetDualDisplaySize();
	my $id = 20;
	my $dt = '"Device->[X]->Raw->".$id++."->Option"';
	my $dx = eval $dt;
	print FD "SaXMeta->[X]->SAX_NO_CDB_CHECK=1\n";
	print FD "Device->[X]->Option=SaXDualHead\n";
	print FD "Monitor->[X]->DisplaySize=$x $y\n";
	print FD "$dx=\"MonitorLayout\" \"$mlayout\"\n";
	$dx = eval $dt;
	print FD "$dx=\"SaXDualMode\" \"off\"\n";
	$dx = eval $dt;
	print FD "$dx=\"Clone\" \"yes\"\n";
	$dx = eval $dt;
	print FD "$dx=\"SaXDualOrientation\" \"off\"\n";
	$dx = eval $dt;
	print FD "$dx=\"SaXDualHSync\" \"off\"\n";
	$dx = eval $dt;
	print FD "$dx=\"SaXDualResolution\" \"off\"\n";
	$dx = eval $dt;
	print FD "$dx=\"SaXDualMonitorVendor\" \"__VESA__\"\n";
	$dx = eval $dt;
	print FD "$dx=\"SaXDualMonitorModel\" \"1024X768@60HZ\"\n";
	$dx = eval $dt;
	print FD "$dx=\"CloneRefresh\" \"50-60\"\n";
	$dx = eval $dt;
	print FD "$dx=\"SaXDualVSync\" \"50-60\"\n";
	close FD;
} else {
	#====================================
	# check secondary DDC data
	#------------------------------------
	open (FD,">>",$profile) ||
		die "$name: Can't open $profile: $!";
	my ($x,$y) = ProfileGetDualDisplaySize();
	my %data   = ProfileGetDDC2Data();
	my $id = 20;
	my $dt = '"Device->[X]->Raw->".$id++."->Option"';
	my $dx = eval $dt;
	print FD "Monitor->[X]->DisplaySize=$x $y\n";
	print FD "$dx=\"MonitorLayout\" \"$mlayout\"\n";
	$dx = eval $dt;
	print FD "$dx=\"SaXDualMode\" \"off\"\n";
	$dx = eval $dt;
	print FD "$dx=\"Clone\" \"yes\"\n";
	$dx = eval $dt;
	print FD "$dx=\"SaXDualOrientation\" \"off\"\n";
	$dx = eval $dt;
	print FD "$dx=\"SaXDualHSync\" \"off\"\n";
	$dx = eval $dt;
	print FD "$dx=\"SaXDualResolution\" \"off\"\n";
	$dx = eval $dt;
	foreach my $key (keys %data) {
		my $val = $data{$key};
		SWITCH: for ($key) {
			/^Model/      && do {
				$id = 27;
				$dx = eval $dt;
				print FD "$dx=\"SaXDualMonitorModel\" \"$val\"\n";
				last SWITCH;
			};
			/^Vendor/     && do {
				$id = 26;
				$dx = eval $dt;
				print FD "$dx=\"SaXDualMonitorVendor\" \"$val\"\n";
				last SWITCH;
			};
			/^Vsync/      && do {
				$id = 28;
				$dx = eval $dt;
				print FD "$dx=\"CloneRefresh\" \"$val\"\n";
				$id = 29;
				$dx = eval $dt;
				print FD "$dx=\"SaXDualVSync\" \"$val\"\n";
				last SWITCH;
			};
		}
	}
	close FD;
}

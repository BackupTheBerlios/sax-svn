#!/usr/bin/perl

use lib '/usr/share/sax/profile';
use lib '.';

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
		die "NVidia_DualHead_DriverOptions: Can't open $profile: $!";
	print FD "Desktop -> [X] -> CalcModelines = yes\n";
	print FD "Monitor -> [X] -> CalcAlgorithm = CheckDesktopGeometry\n";
	close FD;
} else {
	#====================================
	# Do the profile adaptions...
	#------------------------------------
	my $mlayout = ProfileNVidiaSetupMonitorLayout ($profile);
	my ($CA,$CB) = split (/,/,$mlayout);
	my ($x,$y) = ProfileGetDualDisplaySize();
	if (ProfileIsNoteBookHardware()) {
		open (FD,">",$profile) ||
			die "NVidia_DualHead_DriverOptions: Can't open $profile: $!";
		my $dx = "Device->[X]->Raw";
		print FD "\$Modes=Screen->[X]->Depth->16->Modes\n";
		print FD "Monitor->[X]->DisplaySize=$x $y\n";
		print FD "SaXMeta->[X]->SAX_NO_CDB_CHECK=1\n";
		print FD "Desktop->[X]->CalcModelines=no\n";
		print FD "Monitor->[X]->CalcAlgorithm=XServerPool\n";
		print FD "Device->[X]->Driver=nvidia\n";
		print FD "Device->[X]->Screen=0\n";
		print FD "Device->[X]->Option = TwinView,SaXDualHead\n";
		print FD "$dx->11->Option=\"SecondMonitorHorizSync\" \"31-48\"\n";
		print FD "$dx->12->Option=\"SecondMonitorVertRefresh\" \"50-60\"\n";
		print FD "$dx->13->Option=\"MetaModes\" \"$CA:\${Modes[0]},$CB:1024x768;$CA:1024x768,$CB:1024x768\"\n";
		print FD "$dx->14->Option=\"TwinViewOrientation\" \"Clone\"\n";
		print FD "$dx->15->Option=\"ConnectedMonitor\" \"$mlayout\"\n";
		print FD "$dx->16->Option=\"SaXDualOrientation\" \"RightOf\"\n";
		print FD "$dx->17->Option=\"SaXDualHSync\" \"31-48\"\n";
		print FD "$dx->18->Option=\"SaXDualVSync\" \"50-60\"\n";
		print FD "$dx->19->Option=\"SaXDualResolution\" \"1024x768\"\n";
		print FD "$dx->20->Option=\"SaXDualMode\" \"Clone\"\n";
		print FD "$dx->21->Option=\"SaXDualMonitorVendor\" \"__VESA__\"\n";
		print FD "$dx->22->Option=\"SaXDualMonitorModel\" \"1024X768@60HZ\"\n";
		close FD;
	} else {
		open (FD,">>",$profile) ||
			die "NVidia_DualHead_DriverOptions: Can't open $profile: $!";
		print FD "Monitor->[X]->DisplaySize=$x $y\n";
		print FD "Device->[X]->Raw->22->Option=\"MetaModes\" \"$CA:\${Modes[0]},$CB:\${Modes[0]}\"\n";
		close FD;
	}
}

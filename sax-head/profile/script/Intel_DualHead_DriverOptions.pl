#!/usr/bin/perl

use lib '/usr/share/sax/profile';

use strict;
use Profile;

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
		die "Intel_DualHead_DriverOptions: Can't open $profile: $!";
	my $dx = "Device->[X]";
	print FD "SaXMeta->[X]->SAX_NO_CDB_CHECK=1\n";
	print FD "$dx->Raw->20->Option=\"MonitorLayout\" \"$mlayout\"\n";
	print FD "$dx->Raw->21->Option=\"Clone\" \"yes\"\n";
	print FD "$dx->Raw->22->Option=\"CloneRefresh\" \"50-60\"\n";
	print FD "$dx->Option=SaXDualHead\n";
	print FD "$dx->Raw->24->Option=\"SaXDualOrientation\" \"off\"\n";
	print FD "$dx->Raw->25->Option=\"SaXDualHSync\" \"off\"\n";
	print FD "$dx->Raw->26->Option=\"SaXDualVSync\" \"50-60\"\n";
	print FD "$dx->Raw->27->Option=\"SaXDualResolution\" \"off\"\n";
	print FD "$dx->Raw->28->Option=\"SaXDualMode\" \"off\"\n";
	print FD "$dx->Raw->29->Option=\"SaXDualMonitorVendor\" \"__VESA__\"\n";
	print FD "$dx->Raw->30->Option=\"SaXDualMonitorModel\" \"1024X768@60HZ\"\n";
	close FD;
}

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
if (ProfileIsNoteBookHardware()) {
	open (FD,">",$profile) ||
		die "Radeon_DualHead_DriverOptions: Can't open $profile: $!";
	my $dx = "Device->[X]";
	print FD "SaXMeta->[X]->SAX_NO_CDB_CHECK=1\n";
	print FD "\$Modes=Screen->[X]->Depth->16->Modes\n";
	print FD "$dx->Raw->20->Option=\"MergedFB\" \"yes\"\n";
	print FD "$dx->Raw->21->Option=\"CRT2HSync\" \"31-48\"\n";
	print FD "$dx->Raw->22->Option=\"CRT2VRefresh\" \"50-60\"\n";
	print FD "$dx->Raw->23->Option=\"IgnoreEDID\" \"yes\"\n";
	print FD "$dx->Raw->24->Option=\"MetaModes\" \"\${Modes[0]}-1024x768\"\n";
	print FD "$dx->Raw->25->Option=\"CRT2Position\" \"Clone\"\n";
	print FD "$dx->Option=SaXDualHead\n";
	print FD "$dx->Raw->26->Option=\"SaXDualOrientation\" \"LeftOf\"\n";
	print FD "$dx->Raw->27->Option=\"SaXDualHSync\" \"31-48\"\n";
	print FD "$dx->Raw->28->Option=\"SaXDualVSync\" \"50-60\"\n";
	print FD "$dx->Raw->29->Option=\"SaXDualResolution\" \"1024x768\"\n";
	print FD "$dx->Raw->30->Option=\"SaXDualMode\" \"Clone\"\n";
	print FD "$dx->Raw->31->Option=\"SaXDualMonitorVendor\" \"__VESA__\"\n";
	print FD "$dx->Raw->32->Option=\"SaXDualMonitorModel\" \"1024X768@60HZ\"\n";
	close FD;
}

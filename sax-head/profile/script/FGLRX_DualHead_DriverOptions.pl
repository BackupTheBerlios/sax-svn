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
		die "FGLRX_DualHead_DriverOptions: Can't open $profile: $!";
	my $dx = "Device->[X]";
	print FD "SaXMeta->[X]->SAX_NO_CDB_CHECK=1\n";
	print FD "$dx->Raw->55->Option=\"Mode2\" \"1024x768\"\n";
	print FD "$dx->Raw->56->Option=\"HSync2\" \"31-48\"\n";
	print FD "$dx->Raw->57->Option=\"VRefresh2\" \"50-60\"\n";
	print FD "$dx->Raw->58->Option=\"DesktopSetup\" \"Clone\"\n";
	print FD "$dx->Raw->59->Option=\"ForceMonitors\" \"auto,crt1\"\n";
	print FD "$dx->Option=SaXDualHead\n";
	print FD "$dx->Raw->60->Option=\"SaXDualOrientation\" \"LeftOf\"\n";
	print FD "$dx->Raw->61->Option=\"SaXDualHSync\" \"31-48\"\n";
	print FD "$dx->Raw->62->Option=\"SaXDualVSync\" \"50-60\"\n";
	print FD "$dx->Raw->63->Option=\"SaXDualResolution\" \"1024x768\"\n";
	print FD "$dx->Raw->64->Option=\"SaXDualMode\" \"Clone\"\n";
	print FD "$dx->Raw->65->Option=\"SaXDualMonitorVendor\" \"__VESA__\"\n";
	print FD "$dx->Raw->66->Option=\"SaXDualMonitorModel\" \"1024X768@60HZ\"\n";
	close FD;
}

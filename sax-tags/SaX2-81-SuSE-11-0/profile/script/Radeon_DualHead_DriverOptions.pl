#!/usr/bin/perl

use lib '/usr/share/sax/profile';

use strict;
use Profile;

#====================================
# Profile name...
#------------------------------------
my $name = "Radeon_DualHead_DriverOptions";

#====================================
# Init profile script
#------------------------------------
my $profile = ProfileInitScript();

#====================================
# Do the profile adaptions...
#------------------------------------
my $mlayout = ProfileRadeonSetupMonitorLayout ($profile);

#====================================
# Do the profile adaptions...
#------------------------------------
if (! ProfileIsXOrgVendor ("fglrx")) {
	#====================================
	# Call fglrx dual profile
	#------------------------------------
	my $used = "/usr/share/sax/profile/FGLRX_DualHead_DriverOptions";
	print STDERR "$name: selected profile: $used\n";
	if ( -f "$used.pl" ) {
		print STDERR "$name: calling profile script: $used.pl\n";
		qx ($used.pl);
		my $base = qx (basename $used);
		chomp $base;
		$used="/var/lib/sax/$base.tmp";
	}
	qx (cp $used $profile);
} else {
	#====================================
	# Do the radeon adaptions...
	#------------------------------------
	if (ProfileIsNoteBookHardware()) {
		open (FD,">",$profile) ||
			die "$name: Can't open $profile: $!";
		my $id = 20;
		my $dt = '"Device->[X]->Raw->".$id++."->Option"';
		my $dx = eval $dt;
		print FD "SaXMeta->[X]->SAX_NO_CDB_CHECK=1\n";
		print FD "\$MS=Screen->[X]->Depth->16->Modes\n";
		print FD "Device->[X]->Option=SaXDualHead\n";
		print FD "$dx=\"MonitorLayout\" \"$mlayout\"\n";
		$dx = eval $dt;
		print FD "$dx=\"MergedFB\" \"yes\"\n";
		$dx = eval $dt;
		print FD "$dx=\"IgnoreEDID\" \"yes\"\n";
		$dx = eval $dt;
		print FD "$dx=\"CRT2Position\" \"Clone\"\n";
		$dx = eval $dt;
		print FD "$dx=\"SaXDualOrientation\" \"LeftOf\"\n";
		$dx = eval $dt;
		print FD "$dx=\"SaXDualMode\" \"Clone\"\n";
		$dx = eval $dt;
		print FD "$dx=\"SaXDualMonitorVendor\" \"__VESA__\"\n";
		$dx = eval $dt;
		print FD "$dx=\"SaXDualMonitorModel\" \"1024X768\@60HZ\"\n";
		$dx = eval $dt;
		print FD "$dx=\"CRT2HSync\" \"31-48\"\n";
		$dx = eval $dt;
		print FD "$dx=\"CRT2VRefresh\" \"50-60\"\n";
		$dx = eval $dt;
		print FD "$dx=\"MetaModes\" \"\${MS[0]},1024x768;1024x768,1024x768\"\n";
		$dx = eval $dt;
		print FD "$dx=\"SaXDualHSync\" \"31-48\"\n";
		$dx = eval $dt;
		print FD "$dx=\"SaXDualVSync\" \"50-60\"\n";
		$dx = eval $dt;
		print FD "$dx=\"SaXDualResolution\" \"1024x768\"\n";
		close FD;
	} else {
		#====================================
		# check secondary DDC data
		#------------------------------------
		open (FD,">>",$profile) ||
			die "$name: Can't open $profile: $!";
		my %data   = ProfileGetDDC2Data();
		my $id = 20;
		my $dt = '"Device->[X]->Raw->".$id++."->Option"';
		my $dx = eval $dt;
		print FD "$dx=\"MonitorLayout\" \"$mlayout\"\n";
		$dx = eval $dt;
		print FD "$dx=\"MergedFB\" \"yes\"\n";
		$dx = eval $dt;
		print FD "$dx=\"IgnoreEDID\" \"yes\"\n";
		$dx = eval $dt;
		print FD "$dx=\"CRT2Position\" \"Clone\"\n";
		$dx = eval $dt;
		print FD "$dx=\"SaXDualOrientation\" \"LeftOf\"\n";
		$dx = eval $dt;
		print FD "$dx=\"SaXDualMode\" \"Clone\"\n";
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
				/^Resolution/ && do {
					$id = 31;
					$dx = eval $dt;
					print FD "$dx=\"MetaModes\" \"\${Modes[0]},$val\"\n";
					$id = 34;
					$dx = eval $dt;
					print FD "$dx=\"SaXDualResolution\" \"$val\"\n";
					last SWITCH;
				};
				/^Hsync/      && do {
					$id = 29;
					$dx = eval $dt;
					print FD "$dx=\"CRT2HSync\" \"$val\"\n";
					$id = 32;
					$dx = eval $dt;
					print FD "$dx=\"SaXDualHSync\" \"$val\"\n";
					last SWITCH;
				};
				/^Vsync/      && do {
					$id = 30;
					$dx = eval $dt;
					print FD "$dx=\"CRT2VRefresh\" \"$val\"\n";
					$id = 33;
					$dx = eval $dt;
					print FD "$dx=\"SaXDualVSync\" \"$val\"\n";
					last SWITCH;
				};
			}
		}
		close FD;
	}
}

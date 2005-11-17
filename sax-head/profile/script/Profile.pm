#!/usr/bin/perl

use strict;
use SaX;

#=====================================
# ProfileGetDualDisplaySize
#-------------------------------------
sub ProfileGetDualDisplaySize {
	my $x = 0;
	my $y = 0;
	my $size = qx (/usr/sbin/hwinfo --monitor | grep Size: | cut -f2 -d:);
	if ($size =~ /(\d+)x(\d+) cm/) {
		$x = $1;
		$y = $2;
		$x = $x * 10 * 2;
		$y = $y * 10;
	}
	return ($x,$y);
}

#=====================================
# ProfileIsNoteBookHardware
#-------------------------------------
sub ProfileIsNoteBookHardware {
	my $import = new SaX::SaXImport ( $SaX::SAX_CARD );
	my $name = $import->getSectionName();
	my $saxCard = new SaX::SaXManipulateCard (
		$import
	);
	if ($saxCard->isNoteBook()) {
		return 1;
	}
	return 0;
}

#=====================================
# ProfileName
#-------------------------------------
sub ProfileName {
	my $profile = qx(basename $0 | cut -f1 -d.);
	chomp $profile && return $profile;
}

#=====================================
# ProfileInitScript
#-------------------------------------
sub ProfileInitScript {
	my $profile = ProfileName();
	my $stdname = $profile;
	$profile = "/usr/share/sax/profile/$profile";
	if (( ! -f $profile ) || ( $< != 0 )) {
		die "*** $stdname: no such file or permission denied";
	}
	qx (cp $profile $profile.tmp);
	return $profile.".tmp";
}

#=====================================
# ProfileIsXOrgVendor
#-------------------------------------
sub ProfileIsXOrgVendor {
	my $driver = $_[0];
	my $vendor = qx(/usr/share/sax/sysp/script/vendor.pl $driver);
	if (($vendor eq "The XFree86 Project") || ($vendor eq "X.Org Foundation")) {
		return 1;
	}
	return 0;
}

#=====================================
# ProfileNVDualCheck
#-------------------------------------
sub ProfileNVDualCheck {
	my $vendor = qx(/usr/share/sax/sysp/script/vendor.pl nvidia);
	if (($vendor eq "The XFree86 Project") || ($vendor eq "X.Org Foundation")) {
		print "single\n";
	} else {
		print "dual\n";
	}
}

#=====================================
# ProfileReadXLogFile
#-------------------------------------
sub ProfileReadXLogFile {
	my $stdname = ProfileName();
	my $xorglogname = "/var/log/Xorg.99.log";
	if (! -f $xorglogname) {
		$xorglogname = "/var/log/Xorg.0.log";
	}
	open (FD, "<$xorglogname") ||
		die "*** $stdname: Cannot read X.org log $xorglogname";
	my $xorglog = <FD>;
	close FD;
	return $xorglog;
}

#=====================================
# ProfileIntelGetMonitorLayout
#-------------------------------------
sub ProfileIntelGetMonitorLayout {
	my $xorglog  = ProfileReadXLogFile();
	my %bootdevs = ();
	my %devs = ();
	my $primary;
	my $secondary;
	my $connected;
	$_ = $xorglog;
	my $D = "Display Info:";
	my $A = "attached:";
	my $P = "present:";
	while (/^\(..\) I8.0\(\d+\): $D ([^ :]+)[^:]*: $A (\w+), $P (\w+)/mg) {
		my $d=$1;
		my $a=$2;
		my $p=$3;
		if ($p =~ /true/i) {
			$devs{$d}=1;
			$bootdevs{$d}=1 if $a =~ /true/i;
		}
	}
	if ($bootdevs{"LFP"}) {
		$primary="LFP";
	} elsif ($bootdevs{"DFP"}) {
		$primary="DFP";
	} elsif ($bootdevs{"CRT"}) {
		$primary="CRT";
		print STDERR "*** Device not booted into DFP panel\n";
	} else {
		$primary="DFP";
		print STDERR "*** Cannot determine boot display\n";
	}
	undef $devs{$primary};
	if ($devs{"LFP"}) {
		$primary="LFP";
		$secondary="CRT";
		print STDERR "*** Not booted into LFP, but connected\n";
		print STDERR "*** Weird. Config may not work\n";
	} elsif ($devs{"CRT"}) {
		$secondary="CRT";
	} elsif ($devs{"DFP"}) {
		$secondary="DFP";
		print STDERR "*** Secondary output seems to be a flat panel as well\n";
	} elsif ($devs{"DFP2"}) {
		$secondary="DFP";
		print STDERR "*** Secondary output seems to be a flat panel as well\n";
	} elsif ($devs{"CRT2"}) {
		$secondary="CRT";
	} else {
		$secondary="CRT";
		print STDERR "*** No secondary output found\n";
		print STDERR "*** Config may not work\n";
	}
	# ...
	# Intel chips *always* seem to have the internal display attached
	# to pipe B. The following code *might* work well enough to find strange
	# hardware with LDP attached to Pipe A. Might.
	# ---
	$connected="$secondary,$primary";
	$_ = $xorglog;
	if (s/^.*\(..\) I8.0\([0-9]+\): Currently active displays on Pipe A:\s*//s
		&& s/\(..\) I8.0\([0-9]+\): \S.*$//s
	) {
		if (/\sLFP\s/) {
			$connected="$primary,$secondary";
		}
	}
	$_ = $xorglog;
	if (/^\(..\) I[89].0\([0-9]+\): Primary Pipe is A,/m) {
		$connected="$primary,$secondary";
	}
	print STDERR "*** Checked XOrg log file:";
	print STDERR "*** Selecting $connected as monitor configuration.\n";
	return $connected;
}

#=====================================
# ProfileIntelSetupMonitorLayout
#-------------------------------------
sub ProfileIntelSetupMonitorLayout {
	my $profile = $_[0];
	my $stdname = ProfileName();
	open (FD,"<",$profile) ||
		die "*** $stdname: Can't open $profile: $!";
	my $profileData = <FD>;
	my $monitorLayout = ProfileIntelGetMonitorLayout();
	$profileData =~ s/\[MONITORLAYOUT\]/$monitorLayout/;
	close FD;
	open (FD,">",$profile) ||
		die "*** $stdname: Can't open $profile: $!";
	print FD $profileData;
	close FD;
	return $monitorLayout;
}

1;

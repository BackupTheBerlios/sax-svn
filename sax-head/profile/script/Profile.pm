#!/usr/bin/perl

use lib   '/usr/share/sax/modules';

use strict;
use Env;
use CreateSections;
use Storable;
use SaX;

#=====================================
# ProfileGetDualDisplaySize
#-------------------------------------
sub ProfileGetDualDisplaySize {
	my $x = 0;
	my $y = 0;
	my $sysp = "/usr/sbin/sysp";
	my $size = qx ($sysp -q xstuff | grep Size | head -n 1 | cut -f2 -d:);
	if ($size =~ /(\d+)x(\d+)/) {
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
	chomp $vendor;
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
	chomp $vendor;
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
	if (($ENV{HW_UPDATE} == 1) || (! -f $xorglogname)) {
		my $xc = ProfileCreatePreliminaryConfig();
		qx (X -probeonly -logverbose 255 -xf86config $xc :99 >/dev/null 2>&1);
	}
	local $/;
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
	print STDERR "*** Selecting $connected as monitor configuration.\n";
	return $connected;
}

#=====================================
# ProfileIntelSetupMonitorLayout
#-------------------------------------
sub ProfileIntelSetupMonitorLayout {
	my $profile = $_[0];
	my $stdname = ProfileName();
	local $/;
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

#=====================================
# ProfileNVidiaGetMonitorLayout
#-------------------------------------
sub ProfileNVidiaGetMonitorLayout {
	my $xorglog = ProfileReadXLogFile();
	my $bootdev;
	my $otherdevs;
	my $connected;
	$_ = $xorglog;
	my $I = '^\(..\) NVIDIA\(\d+\): ';
	my $B = 'Boot display device\(?s?\)?: ';
	my $S = 'Supported display device\(?s?\)?: ';
	if (/$I$B(.*?)\s*$/m) {
		$bootdev = $1;
		if ($bootdev =~ /,\s*(DFP[^,]*)/) {
			$bootdev = $1;
		} elsif ($bootdev =~ /,\s*(CRT[^,]*)/) {
			$bootdev = $1;
		} else {
			$bootdev =~ s/,.*$//;
		}
	} else {
		$bootdev = "AUTO";
	}
	if (/$I$S(.*?)\s*$/m) {
		($otherdevs = $1) =~ s/$bootdev//;
	} else {
		$otherdevs="AUTO";
	}
	if ($bootdev =~ /CRT/) {
		print STDERR "*** Device booted into CRT. OOPS! This might not be what you intended!\n";
	}
	if ($bootdev =~ /DFP/ || $bootdev =~ /CRT/) {
		if ($otherdevs =~ /DFP/) {
		if ($otherdevs =~ /CRT/) {
			print STDERR "*** Secondary output might be a flat panel or a CRT.\n";
			print STDERR "    Change AUTO to DFP or CRT to activate the according output\n";
			print STDERR "    without hardware plugged in.\n";
			$connected = "$bootdev,AUTO";
		} else {
			$connected = "$bootdev,DFP";
		}
		} elsif ($otherdevs =~ /CRT/) {
			$connected = "$bootdev,CRT";
		} else {
			print STDERR "*** No known secondary output found.\n";
			$connected = "$bootdev,AUTO";
		}
	} else {
		print STDERR "*** Unknown boot display device.\n";
		$connected = "$bootdev,AUTO";
	}

	print STDERR "*** Selecting $connected as monitor configuration.\n";
	return $connected;
}

#=====================================
# ProfileNvidiaSetupMonitorLayout
#-------------------------------------
sub ProfileNVidiaSetupMonitorLayout {
	my $profile = $_[0];
	my $stdname = ProfileName();
	local $/;
	open (FD,"<",$profile) ||
		die "*** $stdname: Can't open $profile: $!";
	my $profileData = <FD>;
	my $monitorLayout = ProfileNVidiaGetMonitorLayout();
	$profileData =~ s/\[MONITORLAYOUT\]/$monitorLayout/;
	close FD;
	open (FD,">",$profile) ||
		die "*** $stdname: Can't open $profile: $!";
	print FD $profileData;
	close FD;
	return $monitorLayout;
}

#=====================================
# ProfileCreatePreliminaryConfig
#-------------------------------------
sub ProfileCreatePreliminaryConfig {
	my $cfgfile = "/tmp/xorg.conf.$$";
	my $config  = "/var/cache/sax/files/config";
	my $stdname = ProfileName();

	#==========================================
	# Retrieve registry data
	#------------------------------------------
	my $hashref = retrieve($config);
	if (! defined $hashref) {
		die "*** $stdname: can not load sax registry";
	}
	my %var = %{$hashref};

	#==========================================
	# Create config suggestion
	#------------------------------------------
	my @part0  = CreateHeaderSection ();
	my @part1  = CreateFilesSection  (\%var);
	my @part2  = CreateModuleSection (\%var);
	my @part3  = CreateServerFlagsSection (\%var);
	my @part4  = CreateInputDeviceSection (\%var);
	my @part5  = CreateMonitorSection (\%var,"yes");
	my @part7  = CreateDeviceSection (\%var);
	my @part8  = CreateScreenSection (\%var);
	my @part9  = CreateServerLayoutSection (\%var);
	my @part10 = CreateDRISection ();
	my @part11 = CreateExtensionsSection (\%var);

	#==========================================
	# Write preliminary config file
	#------------------------------------------
	open (HANDLE,">$cfgfile") ||
		die "*** $stdname: Can't open file: $cfgfile : $!";
	print HANDLE @part0;  print HANDLE "\n";
	print HANDLE @part1;  print HANDLE "\n";
	print HANDLE @part2;  print HANDLE "\n";
	print HANDLE @part3;  print HANDLE "\n";
	print HANDLE @part4;  print HANDLE "\n";
	print HANDLE @part5;  print HANDLE "\n";
	print HANDLE @part7;  print HANDLE "\n";
	print HANDLE @part8;  print HANDLE "\n";
	print HANDLE @part9;  print HANDLE "\n";
	print HANDLE @part10; print HANDLE "\n";
	print HANDLE @part11; print HANDLE "\n";
	close HANDLE;

	return $cfgfile;
}

1;

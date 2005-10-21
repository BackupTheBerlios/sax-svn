#!/usr/bin/perl

use lib '/usr/share/sax/profile';

use strict;
use Getopt::Long;
use Storable;
use NVidia_DualHead_DriverOptions;

#====================================
# DualHead check
#------------------------------------
if ($ARGV[0] eq "check") {
	my $vendor = qx (/usr/share/sax/sysp/script/vendor.pl nvidia);
	if (($vendor eq "The XFree86 Project") || ($vendor eq "X.Org Foundation")) {
		print "single\n";
	} else {
		print "dual\n";
	}
	exit 0;
}

#====================================
# Init profile script
#------------------------------------
my $profile = qx(basename $0 | cut -f1 -d.);
chomp $profile;
$profile = "/usr/share/sax/profile/$profile";
if (( ! -f $profile ) || ( $< != 0 )) {
	die "NVidia_DualHead_DriverOptions: no such file or permission denied";
}

#====================================
# Copy profile
#------------------------------------
qx (cp $profile $profile.tmp);
$profile="$profile.tmp";

#====================================
# Check if profile is valid
#------------------------------------
my $vendor = qx (/usr/share/sax/sysp/script/vendor.pl nvidia);
if (($vendor eq "The XFree86 Project") || ($vendor eq "X.Org Foundation")) {
	open (FD,">",$profile) ||
		die "NVidia_DualHead_DriverOptions: Can't open $profile: $!";
	print FD "Desktop -> [X] ->  CalcModelines = no\n";
	print FD "Monitor -> [X] ->  CalcAlgorithm = XServerPool";
} else {
	open (FD,">>",$profile) ||
		die "NVidia_DualHead_DriverOptions: Can't open $profile: $!";
	my ($x,$y) = getDualDisplaySize();
	print FD "Monitor -> [X] ->  DisplaySize = $x $y\n";
}
close FD;

#!/usr/bin/perl

use strict;
use Getopt::Long;
use Storable;

#====================================
# Functions...
#------------------------------------
sub getProfile {
	my $config = "/var/cache/sax/files/config";
	my $card   = 0;
	my $hashref = retrieve($config);
	if (! defined $hashref) {
		die "MergedFB: can not load sax registry";
	}
	my %dialog = %{$hashref};
	my $driver = $dialog{Device}{$card}{Driver};
	my $dir = "/usr/share/sax/profile";
	SWITCH: for ($driver) {
		/^i810|i915/ && do {
			return "$dir/Intel_DualHead_DriverOptions";
		};
		/^nvidia/    && do {
			return "$dir/NVidia_DualHead_DriverOptions";
		};
		/^radeon/    && do {
			return "$dir/Radeon_DualHead_DriverOptions";
		};
		/^fglrx/     && do {
			return "$dir/FGLRX_DualHead_DriverOptions";
		};
		die "MergedFB: no DualHead profile available";
	}
}

#====================================
# Init profile script
#------------------------------------
my $profile = qx(basename $0 | cut -f1 -d.);
chomp $profile;
$profile = "/usr/share/sax/profile/$profile";
if (( ! -f $profile ) || ( $< != 0 )) {
	die "MergedFB: no such file or permission denied";
}

#====================================
# Copy profile
#------------------------------------
qx (cp $profile $profile.tmp);
my $used = getProfile();
print STDERR "MergedFB: selected profile: $used\n";
if ( -f "$used.sh" ) {
	print STDERR "MergedFB: calling profile script: $used.sh\n";
	$used="$used.tmp";
}
qx (cp $used $profile.tmp);

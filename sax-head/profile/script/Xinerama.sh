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
		die "Xinerama: can not load sax registry";
	}
	my %dialog = %{$hashref};
	my $driver = $dialog{Device}{$card}{Driver};
	my $dir = "/usr/share/sax/profile";
	SWITCH: for ($driver) {
		/^nvidia/    && do {
			return "$dir/NVidia_DualHead";
		};
		/^radeon/    && do {
			return "$dir/Radeon_DualHead";
		};
		/^fglrx/     && do {
			return "$dir/FGLRX_DualHead";
		};
		die "Xinerama: no DualHead profile available";
	}
}

#====================================
# Init profile script
#------------------------------------
my $profile = qx(basename $0 | cut -f1 -d.);
chomp $profile;
$profile = "/usr/share/sax/profile/$profile";
if (( ! -f $profile ) || ( $< != 0 )) {
	die "Xinerama: no such file or permission denied";
}

#====================================
# Copy profile
#------------------------------------
qx (cp $profile $profile.tmp);
my $used = getProfile();
print STDERR "Xinerama: selected profile: $used\n";
if ( -f "$used.sh" ) {
	print STDERR "Xinerama: calling profile script: $used.sh\n";
	$used="$used.tmp";
}
qx (cp $used $profile.tmp);

#!/usr/bin/perl

use lib '/usr/share/sax/profile';

use strict;
use Profile;

sub KMSAndNotIntelOrAMD {
  my $ret = qx(/usr/share/sax/profile/KMSAndNotIntelOrAMD.sh);
  chomp($ret);
  return $ret;
}

#====================================
# Init profile script
#------------------------------------
my $profile = ProfileInitScript();
open (FD,">",$profile) ||
	die "Modesetting: Can't open $profile: $!";

#==========================================
# Check if modesetting driver is neccessary
#------------------------------------------
if ( KMSAndNotIntelOrAMD() eq "yes") {
        print STDERR "modesetting driver required\n";
	print FD "Device -> [X] -> Driver = modesetting\n";
} else {
	print STDERR "modesetting driver not neccessary\n";
        # just to make SaX happy and overwrite the possibly
        # existing modesetting driver entry in the profile
	print FD "Device -> [X] -> Option -> Dummy = yes";
}
close FD;

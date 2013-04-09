#!/usr/bin/perl

use lib '/usr/share/sax/profile';

use strict;
use Profile;

sub DrvSelectInSecBootMode {
  my $ret = qx(/usr/share/sax/profile/DrvSelectInSecBootMode.sh);
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
if ( DrvSelectInSecBootMode () eq "modesetting16") {
	print STDERR "modesetting driver (16bit) required\n";
	print FD "Device -> [X] -> Driver = modesetting\n";
	print FD "Screen->[X]->DefaultDepth = 16"
} elsif ( DrvSelectInSecBootMode () eq "modesetting24") {
	print STDERR "modesetting driver (24 bit) required\n";
	print FD "Device -> [X] -> Driver = modesetting\n";
	print FD "Screen->[X]->DefaultDepth = 24"
} elsif ( DrvSelectInSecBootMode () eq "fbdev") {
	print STDERR "fbdev driver required\n";
	print FD "Device -> [X] -> Driver = fbdev\n";
	print FD "!remove Screen->[X]->DefaultDepth\n";
} else {
	print STDERR "native driver can be used\n";
        # just to make SaX happy and overwrite the possibly
        # existing modesetting driver entry in the profile
	print FD "Device -> [X] -> Option -> Dummy = yes";
}
close FD;

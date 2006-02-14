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
my $fglrx="/usr/share/sax/profile/FireGL";
if (! ProfileIsXOrgVendor ("fglrx")) {
	qx (cp $fglrx $profile);
}

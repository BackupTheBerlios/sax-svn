#!/usr/bin/perl

use lib '/usr/share/sax/profile';

use strict;
use Profile;

#====================================
# Init profile script
#------------------------------------
my $profile = ProfileInitScript();

#=====================================
# get SYSP xstuff data
#-------------------------------------
my $xstuff = new SaX::SaXImportSysp ($SaX::SYSP_CARD);
$xstuff->doImport();
my $vid = $xstuff->getItem("VID");
my $did = $xstuff->getItem("DID");

#====================================
# exit if card doesn't match IDs ...
#------------------------------------
if (!(($vid eq "0x8086") &&
      (($did eq "0x7121") ||
       ($did eq "0x7123") ||
       ($did eq "0x7125") ||
       ($did eq "0x1132") ||
       ($did eq "0x3577") ||
       ($did eq "0x2562") ||
       ($did eq "0x2572") ||
       ($did eq "0x258A")))) {
	# not in i810/i815/i830/i845/i865/E7221
	exit;
}

#====================================
# get current bool options
#------------------------------------
my $import = new SaX::SaXImport ( $SaX::SAX_CARD );
$import->setSource ( $SaX::SAX_AUTO_PROBE );
$import->doImport();
my $saxCard = new SaX::SaXManipulateCard ( $import );
my %options = %{$saxCard -> getOptions()};

my @optlist = ();
foreach my $opt (keys %options) {
	if ($options{$opt} eq "") {
		push @optlist,$opt;
	}
}
push @optlist,"NoDDC";
my $options = join (",",@optlist);

#====================================
# Do the profile adaptions...
#------------------------------------
open (FD,">",$profile) ||
	die "NoDDC: Can't open $profile: $!";
print FD "Device -> [X] -> Option = $options\n";
close FD;

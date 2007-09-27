#!/usr/bin/perl

use lib '/usr/share/sax/profile';

use strict;
use Profile;

#====================================
# Init profile script
#------------------------------------
my $profile = ProfileInitScript();

#====================================
# Add default mouse if needed
#------------------------------------
qx(/usr/sbin/sysp -q mouse | grep Profile | grep -qi "<undefined>");
my $code = $? >> 8;
if ($code != 0) {
	open (FD,">>",$profile) ||
		die "synaptics: Can't open $profile: $!";
	my $i="InputDevice";
	my $l="ServerLayout";
	print FD "$i->\[X+2\]->Identifier = Mouse[[X+2]]\n";
	print FD "$i->\[X+2\]->Driver     = mouse\n";
	print FD "$i->\[X+2\]->Option->InputFashion = Mouse\n";
	print FD "$i->\[X+2\]->Option->ZAxisMapping = 4 5\n";
	print FD "$i->\[X+2\]->Option->ButtonNumber = 5\n";
	print FD "$i->\[X+2\]->Option->Device = /dev/input/mice\n";
	print FD "$i->\[X+2\]->Option->Name = USB-Mouse;ExplorerPS/2 on USB\n";
	print FD "$l->all->InputDevice->[X+2]->id = Mouse[[X+2]]\n";
	print FD "$l->all->InputDevice->[X+2]->usage = SendCoreEvents\n";
	close FD;
}

#!/usr/bin/perl
# Copyright (c) 1996 SuSE GmbH Nuernberg, Germany.  All rights reserved.
#
# Author: Marcus Schaefer <ms@suse.de>, 2003
# xupdate will exchange the InputDevice section for the mouse
# with a new one created involving the libhd data
#   
# Status: Up-to-date
#
use strict;

#---[ readConfig ]-----#
sub readConfig {
#--------------------------------------------------
# read config file and save lines as perl list
# return this list and close the descriptor
#
	my @config = ();
	while (my $line = <>) {
		push (@config,$line);
	}
	close (FD);
	return (@config);
}

#---[ getMouseSection ]-----#
sub getMouseSection {
#--------------------------------------------------
# get mouse section coordinates
#
	my @point = ();
	my $count = 0;
	my $start = 0;
	foreach my $line (@_) {
	$count++;
	if ($line =~ /Section.*InputDevice.*/) {
		$start = $count;
		next;
	}
	if ($line =~ /Driver.*mouse.*/) {
		$point[0] = $start;
	}
	if ((defined $point[0]) && ($line =~ "EndSection")) {
		$point[1] = $count;
		return @point;
	}
	}
}

#---[ prepareUSBMouse ]----#
sub prepareUSBMouse {
#----------------------------------------------------
# if there is a USB mouse connected we need to check
# for the USB mousedev module
#
	my $device = readlink ("/dev/mouse");
	if ($device =~ /\/dev\/input\/mice/) {
		qx (modprobe mousedev);
	} 
}

# /.../
# now exchange the mouse section with the real detected
# input device section using xmset -c feature
# ---
my @list   = readConfig();
my @point  = getMouseSection (@list);
my @detect = split (/\n/,qx (/usr/X11R6/bin/xmset -c));

prepareUSBMouse ();

for (my $i=0;$i < $point[0] - 1;$i++) {
	print $list[$i];
}
foreach my $line (@detect) {
	print "$line\n";
}
for (my $i=$point[1];$i <= @list;$i++) {
	print $list[$i];
}

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

#---[ getServerLayoutSection ]-----#
sub getServerLayoutSection {
#--------------------------------------------------
# get server layout section coordinates
#
	my @point = ();
	my $count = 0;
	my $start = 0;
	foreach my $line (@_) {
	$count++;
	if ($line =~ /Section.*ServerLayout.*/) {
		$start = $count;
		next;
	}
	if ($line =~ /InputDevice.*CorePointer.*/) {
		$point[0] = $count;
	}
	if ((defined $point[0]) && ($line =~ "EndSection")) {
		$point[1] = $count;
		return @point;
	}
	}
}

#---[ getPointerLayout ]-----#
sub getPointerLayout {
#--------------------------------------------------
# get InputDevice section entries
#
	my @result = ();
	my @list;
	foreach my $line (@_) {
		if ($line =~ /.*Device.*\"(.*)\"/) {
			push (@list,$1);
		}
		if ($line =~ /.*Driver.*\"(.*)\"/) {
			push (@list,$1);
		}
	}
	my %entity;
	foreach my $line (@_) {
		if ($line =~ /.*Mouse\[(.*)\].*/) {
			my $count  = $1;
			my $driver = $list[$count - 1];
			my $device = $list[$count];
			if ($count eq 1) {
				my $pointer = "CorePointer";
				push (@result,"InputDevice  \"Mouse[$count]\" \"$pointer\"");
			} else {
			if (! defined $entity{$driver}{$device}) {
				my $pointer = "SendCoreEvents";
				push (@result,"InputDevice  \"Mouse[$count]\" \"$pointer\"");
			}
			}
			$entity{$driver}{$device} = $count;
		}
	}
	return @result;
}

# /.../
# now exchange the mouse section with the real detected
# input device section using xmset -c feature
# ---
my @list   = readConfig();
my @point1 = getMouseSection (@list);
my @point2 = getServerLayoutSection (@list);
my @detect = split (/\n/,qx (/usr/X11R6/bin/xmset -c));
my @layout = getPointerLayout (@detect);

for (my $i=0;$i < $point1[0] - 1;$i++) {
	print $list[$i];
}
foreach my $line (@detect) {
	print "$line\n";
}
for (my $i=$point1[1];$i <= $point2[0] - 2;$i++) {
	print $list[$i];
}
foreach (@layout) {
	print "  $_\n";
}
for (my $i=$point2[0];$i <= @list;$i++) {
	print $list[$i];
}

#!/usr/bin/perl
# Copyright (c) 2002 SuSE GmbH Nuernberg, Germany.  All rights reserved.
#
# Author: Marcus Schaefer <sax@suse.de>, 2002
# SaX2 script: get console -> x11 xkb mapping information
# --
# CVS ID:
# --------
# Status: Up-to-date
#
use strict;
use Env;

#=================================
# Globals...
#---------------------------------
my $CDBMap = "/usr/X11R6/lib/sax/api/data/cdb/Keyboards";
my $CFGMap = "/usr/X11R6/lib/sax/sysp/maps/Keyboard.map";


#=================================
# The magic main :-)
#---------------------------------
sub main {
#------------------------------------------------
# check for all the keyboard information needed	
# to setup X11 XKB keyboard:
# --
	my $XkbVariant;
	my $XkbLayout;
	my $XkbModel;
	my $Protocol;
	my $XkbRules;
	my $XkbOptions;
	my $MapName;
	my $XkbKeyCodes;
	my $LeftAlt;
	my $RightAlt;
	my $ScrollLock;
	my $RightCtl;
	my $Apply;

	my %cdb;
	my %map;

	if (! defined $ARGV[0]) {
		die "xkbctrl: no console name given";
	}
	%cdb = ReadDataCDB ($CDBMap);
	%map = ReadDataConfigMap ($CFGMap);
	foreach (keys %map) {
	if ($_ eq $ARGV[0]) {
		my @list = split (/:/,$map{$_});
		$Protocol    = Tr (shift(@list));
		$XkbRules    = Tr (shift(@list));
		$XkbModel    = Tr (shift(@list));
		$XkbLayout   = Tr (shift(@list));
		$XkbVariant  = Tr (shift(@list));
		$MapName     = Tr (shift(@list));
		$XkbKeyCodes = Tr (shift(@list));
		$LeftAlt     = Tr (shift(@list));
		$RightAlt    = Tr (shift(@list));
		$ScrollLock  = Tr (shift(@list));
		$RightCtl    = Tr (shift(@list));
		$XkbOptions  = Tr (join(":",@list));

		foreach (keys %cdb) {
		if ($MapName eq $_) {
			my $symbols = $cdb{$_}{'<...>'}{ApplySymbols};
			my $layout  = $XkbLayout;
			my @symlist  = split(/,/,$layout);
			$layout = $symlist[0];
			if ($layout ne "us") {
			if ($XkbVariant eq "on") {
				$symbols = $symbols."+".$layout."(nodeadkeys)";
			} else {
				$symbols = $symbols."+".$layout;
			} 
			}
			my $keycodes = $cdb{$_}{'<...>'}{ApplyKeycodes};
			my $compat   = $cdb{$_}{'<...>'}{ApplyCompat};
			my $types    = $cdb{$_}{'<...>'}{ApplyTypes};
			my $geometry = $cdb{$_}{'<...>'}{ApplyGeometry};

			$Apply = "$keycodes:$compat:$types:$symbols:$geometry";
			$Apply =~ s/ +//g;
			$Apply =~ s/\t+//g;
		}
		}
	}
	}
	print "\$[\n";
	if ($XkbVariant eq "on") {
	print "   \"XkbVariant\"   : \"nodeadkeys\",\n";
	}
	print "   \"XkbLayout\"    : \"$XkbLayout\",\n";
	print "   \"XkbModel\"     : \"$XkbModel\",\n";
	print "   \"Protocol\"     : \"$Protocol\",\n";
	if ($XkbRules !~ /xfree86/) {
	print "   \"XkbRules\"     : \"$XkbRules\",\n";
	}
	if ($XkbOptions !~ /none/) {
	print "   \"XkbOptions\"   : \"$XkbOptions\",\n";
	}
	print "   \"MapName\"      : \"$MapName\",\n";
	if ($XkbKeyCodes !~ /xfree86/) {
	print "   \"XkbKeyCodes\"  : \"$XkbKeyCodes\",\n";
	}
	if ($LeftAlt !~ /none/) {
	print "   \"LeftAlt\"      : \"$LeftAlt\",\n";
	}
	if ($RightAlt !~ /none/) {
	print "   \"RightAlt\"     : \"$RightAlt\",\n";
	}
	if ($ScrollLock !~ /none/) {
	print "   \"ScrollLock\"   : \"$ScrollLock\",\n";
	}
	if ($RightCtl !~ /none/) {
	print "   \"RightCtl\"     : \"$RightCtl\",\n";
	}
	print "   \"Apply\"        : \"$Apply\"\n";
	print "]\n"
}

#----[ ReadDataConfigMap ]----------#
sub ReadDataConfigMap {
#--------------------------------------------
# read Keyboard.map information file...
# return a hash
#
	my $filename = $_[0];

	my $consoleName;
	my $stuff;
	my %result;

	if (! open (DATA,$filename)) {
		die "could not open file: $filename";
	}
	while (my $line=<DATA>) {
		chomp ($line);
		my @list = split (/:/,$line);
		$consoleName = shift (@list);
		$consoleName =~ s/ +//g;
		$consoleName =~ s/\t+//g;
		$stuff = join(":",@list);
		$result{$consoleName} = $stuff;
	}
	return (%result);
}

#----[ ReadDataCDB ]----------#
sub ReadDataCDB {
#--------------------------------------------
# read CDB formatted data files...
# return a hash
#
	my $filename = $_[0];
	my %result;
	my $vendor;
	my $name;
	my $option;
	my $value;

	my @file = ();
	my $filesize = 0;
	if (! open (DATA,$filename)) {
		die "could not open file: $filename";
	}
	while (my $line=<DATA>) {
		chomp ($line);
		push (@file,$line);
		$filesize++;
	}
	close(DATA);

	foreach my $line (@file) {
	if (($line =~ /^#/) || ($line eq "")) {
		next;
	}

	if ($line =~ /(.*):(.*)\{/) {
		# get the topic line...
		# ----------------------
		$vendor = $1;
		$name   = $2;
		$vendor =~ s/^ +//g; $vendor =~ s/ +$//g;
		$name   =~ s/^ +//g; $name   =~ s/ +$//g;
	} elsif ($line =~ /(.*)=(.*)/) {
		# get the values behind this topic...
		# ------------------------------------
		$option = $1;
		$value  = $2;
		$option =~ s/^ +//g; $option =~ s/ +$//g;
		$value  =~ s/^ +//g; $value  =~ s/ +$//g;

		if ((defined $name) && (defined $vendor)) {
			$result{$vendor}{$name}{$option} = $value;
		}
	}
	}
	return(%result);
}

#---[ tr ]----#
sub Tr {
#----------------------------------------------
# translate item into a non whitespace format
# 
	my $item = $_[0];
	$item =~ s/^\t+//g; $item =~ s/\t+$//g;
	$item =~ s/^ +//g; $item =~ s/ +$//g;
	return ($item);
}

main();

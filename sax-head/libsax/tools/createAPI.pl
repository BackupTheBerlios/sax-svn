#!/usr/bin/perl
# Copyright (c) 2004 SuSE AG Nuernberg, Germany.  All rights reserved.
#
# Author: Marcus Schaefer <sax@suse.de>, 2004
# libsax script: create xorg.conf configuration file using ISaX
# --
# Status: Development
#
use strict;
use Env;
use FileHandle;

#=====================================
# Globals 
#-------------------------------------
my %spec;

#---[ init ]----#
sub init {
#----------------------------------------------
# init global specifications in %spec hash
#
	$spec{CDir}  = "/var/lib/sax/";
	$spec{ISaX}  = "/sbin/isax";
}

#---[ createXOrgConfig ]-----#
sub createXOrgConfig {
#----------------------------------------------
# create configuration file using ISaX
#
	if (-f "$spec{CDir}/keyboard") {
		qx (cat $spec{CDir}/keyboard | grep '^0' > $spec{CDir}/keyboard.new);
		qx (cat $spec{CDir}/keyboard | grep -v '^0' >> $spec{CDir}/input);
		qx (mv  $spec{CDir}/keyboard.new $spec{CDir}/keyboard);
	}
	my $patchBIOS = 0;
	my $BIOSData  = "ok";
	my @configFile = (
		"card",
		"desktop",
		"input",
		"keyboard",
		"layout",
		"path",
		"extensions"
	);
	my $api = new FileHandle "> $spec{CDir}/apidata";
	if (! defined $api) {
		exit (3);
	}
	foreach my $file (@configFile) {
		my $handle = new FileHandle;
		if ($handle -> open("< $spec{CDir}/$file")) {
			SWITCH: for ($file) {
			/^card/     && do {
				print $api "Card \{\n";
				last SWITCH;
			};
			/^desktop/  && do {
				print $api "Desktop \{\n";
				last SWITCH;
			};
			/^input/      && do {
				print $api "Mouse \{\n";
				last SWITCH;
			};
			/^keyboard/   && do {
				print $api "Keyboard \{\n";
				last SWITCH;
			};
			/^layout/     && do {
				print $api "Layout \{\n";
				last SWITCH;
			};
			/^path/       && do {
				print $api "Path \{\n";
				last SWITCH;
			};
			/^extensions/ && do {
				print $api "Extensions \{\n";
				last SWITCH;
			};
			}
			while (<$handle>) {
				chomp ($_); my @list = split (/ : /,$_);
				my $line = sprintf (" %s %-20s =     %s\n",
					$list[0],$list[1],$list[2]
				);
				if ($list[2] =~ /^i810 *$/) {
					$patchBIOS=1;
				}
				if ($list[1] =~ /^Modes:16 *$/) {
					my @reslist = split (/,/,$list[2]);
					$BIOSData = shift (@reslist);
				}
				print $api $line;
			}
			print $api "\}\n\n";
		}
		$handle->close;
	}
	$api->close();
	if ($patchBIOS == 1) {
		return $BIOSData;
	} else {
		return "ok";
	}
}


#=====================================
# Run... 
#-------------------------------------
init();
my $patchBIOS = createXOrgConfig();
if ($patchBIOS ne "ok") {
	#=====================================
	# patching bios using 855resolution
	#-------------------------------------
	my $tool = "/usr/sbin/855resolution";
	if ( -f $tool ) {
		my @xy = split (/x/,$patchBIOS);
		my $result = qx ($tool 3c $xy[0] $xy[1] 2>&1);
		my $args = "3c $xy[0] $xy[1]";
		my $key  = "VIDEOBIOS_PARAMETERS";
		my $file = "/etc/sysconfig/videobios";
		if (-f $file) {
			qx (cat $file | sed -e s!$key=.*!$key=\\""$args"\\"! > $file.tmp);
			qx (mv $file.tmp $file);
		}
	}
}

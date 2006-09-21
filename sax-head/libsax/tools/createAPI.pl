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
# getIntelPatchMap
#-------------------------------------
sub getIntelPatchMap {
	my $resx = shift;
	my $resy = shift;
	my %result;
	if (! open (FD,"/usr/share/sax/sysp/maps/IntelPatch.map")) {
		return undef;
	}
	foreach my $line (<FD>) {
		if ($line =~ /^#/) {
			next;
		}
		if ($line =~ /(.*):(.*):(.*)/) {
			my $code = $1;
			my $args = $2;
			my $prog = $3;
			$code =~ s/^ +//; $code =~ s/ +$//;
			$args =~ s/^ +//; $args =~ s/ +$//;
			$prog =~ s/^ +//; $prog =~ s/ +$//;
			if ((defined $resx) && (defined $resy)) {
				$args =~ s/\%x/$resx/;
				$args =~ s/\%y/$resy/;
			}
			$result{$code}{$prog} = $args;
		}
	}
	close  FD;
	return %result;
}

#=====================================
# getIntelPatchCode
#-------------------------------------
sub getIntelPatchCode {
	my @data = qx (/usr/sbin/sysp -q server);
	my $subvendor = 0;
	my $subdevice = 0;
	my $vendor    = 0;
	my $device    = 0;
	foreach my $item (@data) {
		if ($item =~ /VID.*: 0x(.*)/) {
			$vendor = $1;
		}
		if ($item =~ /DID.*: 0x(.*)/) {
			$device = $1;
		}
		if ($item =~ /SUB-VID.*: 0x(.*)/) {
			$subvendor = $1;
		}
		if ($item =~ /SUB-DID.*: 0x(.*)/) {
			$subdevice = $1;
		}
	}
	return "0x$vendor$device$subvendor$subdevice";
}

#=====================================
# Run... 
#-------------------------------------
init();
my $patchBIOS = createXOrgConfig();
if ($patchBIOS ne "ok") {
	#=====================================
	# i810 device: check for patching bios
	#-------------------------------------
	my @xy = split (/x/,$patchBIOS);
	my %patch = getIntelPatchMap  ($xy[0],$xy[1]);
	my $icode = getIntelPatchCode ();
	foreach my $code (keys %patch) {
		if ($code ne $icode) {
			next
		}
		foreach my $prog (keys %{$patch{$code}}) {
		if (-f $prog) {
			my $args = $patch{$code}{$prog};
			my $result = qx ($prog $args 2>&1);
			my $key  = "VIDEOBIOS_PARAMETERS";
			my $file = "/etc/sysconfig/videobios";
			my $tmpf = "$file.tmp";
			if (-f $file) {
				qx (cat $file | sed -e s!$key=.*!$key=\\""$args"\\"! > $tmpf);
				qx (mv $tmpf $file);
				$key = "VIDEOBIOS_PATCH";
				qx (cat $file | sed -e s!$key=.*!$key=\\""yes"\\"! > $tmpf);
				qx (mv $tmpf $file);
			}
		}
		}
	}
}

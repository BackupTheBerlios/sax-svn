#!/usr/bin/perl
# Copyright (c) 1996 SuSE GmbH Nuernberg, Germany.  All rights reserved.
# Author: Marcus Schaefer <sax@suse.de>, 2000
#
# xrun.pl configuration level 3
# perl script to start XFree86 and the tuning tool
# This is part of the xapi.pl configuration step
#
# CVS ID:
# --------
# Status: Up-to-date
#
use lib "/usr/X11R6/lib/sax/api/dialog";

use strict;
use Env;

use Getopt::Long;
use Locale::gettext;
use FileHandle;
use POSIX;

#=========================
# Globals...
#-------------------------
my $choose = 1;
my $server;
my %var;

main();
#======================================================
# this script will start the X-Server and create
# the environment XFine2 can start on. This include the
# start of the XIdle daemon and the helper boundary
# around the desktop
#-------------------------------------------------------
#
#--[ main ]-----#
sub main {
#-----------------------------------
# main routine to start server and
# tuning tool
#
	my $Display;
	my $result = GetOptions(
		"display|d=s" => \$Display,
		"help|h"      => \&usage,
		"<>"          => \&usage
	);
	if ( $result != 1 ) {
		usage();
	}
	if ($Display eq "") {
		$Display = ":2.0";
	}
	my $user = qx(whoami);
	if ($user !~ /root/i) {
		die "xrun: only root can do this";
	}
	# /.../
	# set specifications...
	# ---
	$var{X}             = "/usr/X11R6/lib/sax/xw.pl";
	$var{Xpid}          = "/var/log/SaX.pid";
	$var{SaXLog}        = "/var/log/SaX.log";
	$var{LastLog}       = "/var/cache/sax/files/lastlog";
	$var{ConfigFile}    = "/var/cache/sax/files/XF86Config";
	$var{FirstConfig}   = "/var/cache/sax/files/XF86Config.first";
	$var{StartOptions}  = "/var/cache/sax/files/xapi.opt";
	$var{Xidle}         = "/usr/X11R6/lib/sax/xapi";
	$var{XFine}         = "/usr/X11R6/lib/xfine/xfine";

	# /.../
	# set signal handler...
	# ---
	$SIG{TERM} = "Quit";
	$SIG{INT}  = "Quit";

	# /.../
	# the initiial X-Terminal on a new server startup is
	# saved at /var/log/xvt. This terminal plus 1 is the
	# test terminal for the new X-Server.
	# -----------------------------------
	my $tty = GetXTerminal(); $tty++;

	# run server at vt $tty...
	# -------------------------
	unlink($var{LastLog});
	my $XLoader = "$var{X} $var{LastLog}:xfine -xf86config $var{ConfigFile}";
	my $XFlags  = "-allowMouseOpenFail -verbose 2 +accessx vt$tty";
	system("$XLoader $XFlags $Display >$var{Xpid} 2>/dev/null");
	my $exitStatus = $? >> 8;
	if ($exitStatus ne "0") {
		$tty--; qx (chvt $tty);
		exit (1);
	}
	if (! open  (FD,"$var{Xpid}")) { 
		die "xapi: could not open file: $var{Xpid}";
	}
	while (<FD>) { $server = $_; } 
	close (FD);

	unlink($var{Xpid});
	logger($var{ConfigFile},$var{LastLog},$var{SaXLog});
	$server =~ s/\n//g;

	# /.../
	# switch directly to the test server terminal
	# ---------------------------------------------
	my $proc = qx (LANG=POSIX ps --no-headers $server);
	if ($proc eq "") {
		$tty--; qx (chvt $tty);
		exit (1);
	}
	qx (chvt $tty);

	# start XFine2 on the server...
	# -------------------------------
	my $xfineopt = GetStartOptions();
	my $xfinePid = fork();
	if (! defined $xfinePid) {
		print "xrun: could not fork...abort\n";
		exit (1);
	} elsif ($xfinePid) {
		$var{xfinePid} = $xfinePid;
		waitpid ($xfinePid,0);
		$choose = $? >> 8;
	} else {
		exec ("$var{XFine} -display $Display $xfineopt >/dev/null 2>&1");
	}

	# /.../
	# we finished the tuning and want to clean up the
	# X-Server clients...
	# ----------------------
	Quit();
};

#----[ Quit ]------#
sub Quit {
#-----------------------------------------------------
# exit all running X11 clients and quit the
# X-Server than...
#
	Kill ($server);
	print "$choose\n";
	exit ($choose);
}

#-----[ logger ]------#
sub logger {
#---------------------------------------
# append logging data to file...
#
	my $datafile   = $_[0];
	my $configfile = $_[1];
	my $file       = $_[2];
	my $date   = qx (
		LANG=POSIX /bin/date "+%d-%h %H:%M:%S"
	);
	chomp ($date);

	my @data;
	my $diff;

	# /.../
	# if FirstConfig does not exist:
	# - create it and save file contents as first log
	# - otherwhise create a diff and save the diff
	# --------------------------------------------
	if (! -f $var{FirstConfig}) {
		qx (cp $datafile $var{FirstConfig});
		push (@data,"Configuration: [FIRST SETUP]...\n");
		open (FD, "$datafile");
		while(<FD>) {
			push (@data,$_);
		}
		close(FD);
	} else {
		push (@data,"Configuration: [NEXT SETUP DIFF]...\n");
		$diff = qx (diff -u $var{FirstConfig} $datafile);
		if ($diff ne "") {
		foreach my $line (split("\n+",$diff)) {
			push (@data,$line);
		}
		} else {
			push (@data,"no differences\n");
		}
	}
	open (FD,">>$file");
	my $topic = shift (@data);
	chomp ($topic);
	print FD "$date <T> $topic [\n";
	foreach my $line (@data) {
		chomp ($line);
		print FD "\t$line\n";
	}
	print FD "]\n";

	# /.../
	# open log and save it to list...
	# ---------------------------------
	open (IO, "$configfile");
	print FD "$date <T> Configuration: [LOG]... [";
	while(<IO>) {
		chomp ($_);
		print FD "\t$_\n";
	}
	print FD "]\n";
	close (IO);
	close (FD);
}

#---[ Kill ]-------#
sub Kill {
#--------------------------------------
# my own kill function to make sure
# the process is really killed
#
	my $pid = $_[0];
	my $cnt;

	if ($pid eq "") {
		return;
	}
	kill(15,$pid);
}

#----[ GetXTerminal ]---------#
sub GetXTerminal {
#----------------------------------------------------
# this function read the /var/log/xvt file and 
# return the number of the X terminal SaX2 was 
# running on
#
	my $file = "/var/log/xvt";
	my $nr;

	open (FD,$file);
	while(<FD>) {
	if ($_ =~ /TTyX:(.*)/) {
		$nr = $1;
	}
	}
	close(FD);
	if ($nr eq "") {
		$nr = 7;
	}
	return($nr);
}

#---[ GetStartOptions ]---#
sub GetStartOptions {
#---------------------------------------------
# read the options file written via xc.pl and
# lookup the --unifont/-U and the -style
# options
#
	my $result  = "";
	my $options = "";
	my $optfile = "/var/cache/sax/files/xapi.opt";
	if (open (FD,"<$optfile")) {
		$options = <FD>; chomp $options;
	}
	close FD;
	SWITCH: for ($options) {
	/--unifont|-U/   && do {
		$result=$result."--unifont ";
	};
	/-style/         && do {
		$result=$result."-style platinum ";
	};
	}
	$result =~ s/ +$//;
	return $result;
}

#---[ usage ]------#
sub usage {
#-----------------------
# if you need help :-)
#
	print "Linux SaX2 Version 4.8 finish level (xrun) (2002-02-20)\n";
	print "(C) Copyright 2002 SuSE GmbH\n";
	print "\n";

	print "usage: xrun [ options ]\n";
	print "options:\n";
	print "[ -d | --display < string > ]\n";
	print "   set display string for startup\n";
	print "--\n";
	exit(0);
}

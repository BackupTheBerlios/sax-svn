# /.../
# Copyright (c) 2001 SuSE GmbH Nuernberg, Germany.  All rights reserved.
#
# Author: Marcus Schaefer <sax@suse.de>, 2001
#
# CreateSections.pm configuration level 2
# Create the header section of the XF86Config
#
# CVS ID:
# --------
# Status: Up-to-date
#

#---[ CreateHeaderSection ]-----#
sub CreateHeaderSection {
#---------------------------------------------------
# create the XF86Config header according to the
# builder program (SaX2 or ISaX)
#
	my $builder = $_[0];
	my @result;
	my $date    = qx(/bin/date -I seconds);
	$date =~ s/\n//g;

	if ($builder eq "") {
		$builder = "SaX2";
	}
	push(@result,"# /.../\n");
	push(@result,"# SaX generated XFree86 config file\n");
	push(@result,"# Created on: $date.\n");
	push(@result,"#\n");
	push(@result,"# Version: $Version\n");
	push(@result,"# Contact: Marcus Schaefer <sax\@suse.de>, 2002\n");
	push(@result,"#\n");
	push(@result,"# Automatically generated by [$builder] ($Version)\n");
	push(@result,"# PLEASE DO NOT EDIT THIS FILE!\n"); 
	push(@result,"#\n");
	return(@result);
}

1;

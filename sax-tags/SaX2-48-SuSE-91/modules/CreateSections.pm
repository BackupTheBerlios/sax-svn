# /.../
# Copyright (c) 2001 SuSE GmbH Nuernberg, Germany.  All rights reserved.
#
# Author: Marcus Schaefer <sax@suse.de>, 2001
#
# CreateSections.pm configuration level 2
# provide create functions which are using the var hash 
# to generate the single config sections
#
# CVS ID:
# --------
# Status: Up-to-date
#
use lib '/usr/X11R6/lib/sax/modules/create';

use Env;
use device;
use dri;
use files;
use flags;
use header;
use input;
use layout;
use modes;
use module;
use monitor;
use screen;

$Version = "4.8";

#---[ PrintLine ]----#
sub PrintLine {
#---------------------------------------------
# print a formatted config line. If the
# value begins with a # made this entry an
# optional comment entry
#
	my $key   = $_[0];
	my $value = $_[1];
	my $line;

	if ($value =~ /^.*#.*/) {
		$value =~ s/#//;
		$value =~ s/^\" +/\"/;
		$line = sprintf("  #%-12s %s\n",$key,$value);
	} else {
		$line = sprintf("  %-12s %s\n",$key,$value);
	}
	return($line);
}

1;

#/.../
# Copyright (c) 2000 SuSE GmbH Nuernberg, Germany.  All rights reserved.
#
# Author: Marcus Schaefer <sax@suse.de>, 2003
# WinINF.pm  read windows inf file ( especially monitor inf files )
#
# CVS ID:
# --------
# Status: Up-to-date
#
use strict;

#---[ mountDisk ]----#
sub mountDisk {
#----------------------------------------------
# mount a disk and return the mountpoint of the
# filesystem where the data lives
#
	my $mountpoint = "/tmp/mydisk.$$";
	if (! -d $mountpoint) {
		mkdir ($mountpoint,0500);
	}
	qx (mount /dev/fd0 $mountpoint >/dev/null 2>&1);
	my $error = $? >> 8;
	if ($error) {
		rmdir ($mountpoint);
		return ("/");
	}
	opendir (FD,$mountpoint);
	my @files = readdir(FD);
	closedir(FD);
	foreach my $file (@files) {
	if ($file =~ /.*\.inf/) {
		return ("$mountpoint/$file");
	}
	}
	return ("/");
}

#---[ umountDisk ]---#
sub umountDisk {
#----------------------------------------------
# unmount disk and remove the mountpoint
# directory
#
	my $mountpoint = $_[0];
	qx (umount /dev/fd0 >/dev/null 2>&1);
	rmdir ($mountpoint);
}

#---[ readDisk ]---#
sub readDisk {
#----------------------------------------------
# read first .inf file from the disk and
# return a list of data lines
#
	my $file = mountDisk();
	my @data = ();
	if ($file eq "/") {
		return (@data);
	}
	if ( ! open(FD,"$file") ) {
		return (@data);
	}
	while (<FD>) {
	if (($_ !~ /^;/) && ($_ !~ /^.$/)) {
		chomp ($_);
		push (@data,$_);
	}
	}
	push (@data,"[]");
	close (FD); umountDisk();
	return (@data);
}

#---[ readData ]-----#
sub readData {
#--------------------------------------------------
# read lines from .inf file. Remove empty lines
# and comments
#
	my @data = ();
	while (<>) {
	if (($_ !~ /^;/) && ($_ !~ /^.$/)) {
		chomp ($_);
		push (@data,$_);
	}
	}
	push (@data,"[]");
	return (@data);
}

#---[ getSections ]-----#
sub getSections {
#--------------------------------------------------
# create hash containing list references to all
# [ hashkey ] sections in the .inf file
#
	my $sname;
	my %sections;
	my @data;
	my $count = 0; 
	foreach (@_) {
	if ($_ =~ /^\[(.*)\]/) {
		if ( $count == 1 ) {
			my @save = @data;
			$sections{$sname} = \@save;
			$sname = $1;
			$count = 0;
			@data  = ();
		}
		if ( $count == 0 ) {
			$sname = $1;
			$count++;
		}
	} else {
		push (@data,$_);
	}
	}
	return (%sections);
}

#---[ getManufacturerList ]----#
sub getManufacturerList {
#--------------------------------------------------
# get names of Manufactures in the Manufacturer
# section. Return a list with names
#
	my %sections = %{$_[0]};
	my @manulist = @{$sections{Manufacturer}};
	my @result   = ();
	foreach my $item (@manulist) {
	if ($item =~ /\%(.*)\%.*/) {
		push (@result,$1);
	}
	}
	return (@result);
}

#---[ getStringSection ]----#
sub getStringSection {
#--------------------------------------------------
# resolve string sections contents. Return a hash
# with key/value according to key=value in the Strings
# section
#
	my %sections = %{$_[0]};
	my @stringlist = @{$sections{Strings}};
	my %result = {};
	foreach my $item (@stringlist) {
	if ($item =~ /(.*)=\"(.*)\".*/) {
		$result{uc $1} = $2;
	}
	}
	return (%result);
}

#---[ getRegister ]----#
sub getRegister {
#--------------------------------------------------
# resolve AddReg section and jump to the key in
# this section, returning the data section for 
# AddReg
#
	my %sections = %{$_[0]};
	my $instkey  = $_[1];
	my @result = ();
	foreach my $reg (@{$sections{$instkey}}) {
	if ($reg =~ /^AddReg=(.*)/) {
		my @reglist = split(/,/,$1);
		my $regkey = $reglist[0];
		@result = @{$sections{$regkey}};
	}
	}
	return (@result);
}

#---[ getMonitorINFData ]----#
sub getMonitorINFData {
#--------------------------------------------------
# walk through the single sections needed to provide
# monitor data within a perl hash for CDB usage
#
	my %sections = %{$_[0]};
	my @manulist = getManufacturerList (\%sections);
	my %strings  = getStringSection (\%sections);
	my %result;
	#==============================================
	# walk through all the monitor vendors
	#----------------------------------------------
	foreach my $item (@manulist) {
		my $vendor = uc ($item);
		# /.../
		# got the vendor name here...
		# print "+++++++++ $item -> $strings{$vendor}\n";
		# ---
		#my $vendorName = $strings{$vendor};
		my $vendorName = $vendor;
		#==============================================
		# walk through all the entries of a vendor
		#----------------------------------------------
		foreach my $manu (@{$sections{$item}}) {
		if ($manu =~ /^\%(.*)\%=(.*),.*Monitor\\(.......)/) {
			my $namekey = $1;
			my $instkey = $2;
			my $monikey = $3;
			if (defined ($monikey)) {
			if (defined ($strings{uc $namekey})) {
				my $bigname = uc ($namekey);
				# /.../
				# got the name and the DDC-ID here...
				# print "+++++++ $namekey -> $strings{$bigname} : $monikey\n";
				# ---
				my $productName = $strings{$bigname};
				$productName =~ s/\t//g;
				$productName =~ s/,/ /g;
				$productName =~ s/  +/ /g;
				my @data = getRegister (\%sections,$instkey);
				foreach my $sync (@data) {
				if ($sync =~ /.*Mode1,,\"(.*)\"/) {
					my $syncstr = $1;
					$syncstr =~ s/\.[0-9]+//g;
					my @synclist = split(/,/,$syncstr);
					for(my $i=0;$i<2;$i++) {
					if ($synclist[$i] !~ /.*\-.*/) {
						my $value = $synclist[$i];
						my $sendv = $value;
						if ($i == 0) {
							$sendv = $value - 5;
						}
						if ($i == 1) {
							$sendv = $value - 10;
						}
						$synclist[$i] = "$sendv-$value";
					}
					}
					# /.../
					# got hsync/vsync here...
					# print "$synclist[0] : $synclist[1]\n";
					# ---
					$result{$vendorName}{$productName}{Option} = "DPMS";
					$result{$vendorName}{$productName}{Hsync}  = $synclist[0];
					$result{$vendorName}{$productName}{Vsync}  = $synclist[1];
					$result{$vendorName}{$productName}{DDC}    = $monikey;
				}
				}
			}
			}
		}
		}
	}
	return (%result);
}

1;

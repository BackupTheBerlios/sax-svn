#----[ AutoDetectFontPath ]----#
sub AutoDetectFontPath {
#---------------------------------------------------
# this function look at the first font definitions 
# for the different languages and set the requested 
# FontPath(s) as registered
#
	my (%spec) = %{$_[0]}; # specs
	my (%var)  = %{$_[1]}; # config hash

	my $setting;   # current FontPath setting
	my @list;      # single paths in a list
	my $language;  # language spec from rc.config
	my %lang;      # language ISO code table
	my %path;      # first path definitions per language
	my %stat;      # static font path list
	my $firstpath; # the first path in the Files section
	my $flist;     # list of first paths
	my @newfont;   # all fonts sorted for first path
	my $n;         # loop variable
	my $found;     # found font in predefined list
	my $arch;      # machine type

	# read static FontPath list according to the
	# machines architecture
	# ----------------------
	$arch = qx(/bin/arch);
	%stat = ReadTable($spec{StaticFontPathList});
	SWITCH: for ($arch) {
		# Sparc FontPath definitions...
		# -------------------------------
		/sparc|sparc64/    && do {
		$var{Files}{0}{FontPath} = $stat{sparc};
		last SWITCH;
		};
		# default definition 
		# for Intel and PPC
		# -------------------
		$var{Files}{0}{FontPath} = $stat{intel};
	}
 
	# read language specific tables and Fonts
	# ----------------------------------------
	%lang = ReadTable($spec{LangCodes});
	%path = ReadTable($spec{LangFirstPath});

	$setting   = $var{Files}{0}{FontPath};
	if ($setting ne "") {
		@list     = split(/,/,$setting);
		$language = GetRcLang($spec{RcConfig},$spec{RcSysConfigLang});
		$language =~ s/ +//g;
		$language = lc($language);

		if (defined $lang{$language}) {
			$firstpath = $path{$lang{$language}};
		} else {
			$firstpath = $path{en};
		}
		@flist = split(/,/,$firstpath);
		foreach (@flist) {
			$_ =~ s/\n//g; $_ =~ s/ +//g;
			push(@newfont,$_);
		}
		foreach $n (@list) {
			$n     =~ s/\n//g;
			$n     =~ s/ +//g;
			$found = 0;
			foreach (@flist) {
			if ($_ =~ /$n.*/) {
				$found = 1; last;
			}
			}
			if ($found == 0) {
				push(@newfont,$n);
			}
		}
		$setting = join(",",@newfont);
		$var{Files}{0}{FontPath} = $setting;
	}
	return(%var);
}

#---[ ReadTable ]-----#
sub ReadTable {
#-------------------------------------------
# similar to the ReadData function this
# function will incorp the data files
# returning a hash instead of a list
#
	my $file = $_[0];  # file to read
	my %result;        # result hash
	my $line;          # one line of the file
	my $key;           # key
	my $value;         # value

	open(FD,"$file") || die "init: could not open $file: $!";
	while($line = <FD>) {
		chomp($line);
		# if the line starts with a tab stop we will add this
		# line to the hash key defined in a earlier loop pass
		# ----------------------------------------------------
		if ($line =~ /^\t(.*)/) {
		if (defined $key) {
			$result{$key} = "$result{$key},$1";
		}
		}
		if ($line !~ /^\[.*/) {
		if ($line =~ /(.*)=(.*)/) {
		$key   =  $1;
		$value =  $2;
		$key   =~ s/ +//g;
		$value =~ s/ +//g;
		$result{$key} = $value;
		}
		}
	}
	close(FD);
	return(%result);
}

#----[ GetRcLang ]----#
sub GetRcLang {
#--------------------------------------
# this function look at the rc.config 
# and get the RC_LANG statement...
#
	my $file1 = $_[0];
	my $file2 = $_[1];
	my $file  = $file1;
	my $l;

	# /.../
	# if the sysconfig file exist, use the
	# sysconfig file not the rc.config
	# --------------------------------
	if (-f $file2) {
		$file = $file2;
	}

	$l = qx(cat $file | grep RC_LANG);
	$l =~ /.*=\"(.*)\".*/;
	$l = $1;

	if ($l eq "") {
		$l = qx(cat $file | grep DEFAULT_LANGUAGE);
		$l =~ /.*=\"(.*)\".*/;
		$l = $1;
	}
	return($l);
}

1;

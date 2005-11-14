#==========================
# define flag list...
#--------------------------
@flag_list = ();

#----[ AutoDetectResolution ]----------#
sub AutoDetectResolution {
#----------------------------------------------------
# detecting DDC resolutions and calculate modelines
# for it. check for special clocks and the framebuffer 
# if needed
#
	my (%spec) = %{$_[0]}; # specs
	my (%var)  = %{$_[1]}; # config hash
	my %query;             # the query hash
	my @dac_list;          # dac values
	my @mem_list;          # memory list
	my @vesa_list;         # vesa mode list
	my @clock_list;        # clocks list
	my @dpi_list;          # display size list
	my @fbt_list;          # framebuffer timing list
	my @vm_list;           # vmware color depth list
	my @vid_list;          # list of vendor ID`s
	my $size;              # number of cards
	my @idquery;           # query list for vendor ID`s
	my $card;              # current card number in loop
	my $i;                 # loop counter;
	my $detectvesa;        # detect VESA flag
	my $detectfb;          # detect FbTiming flag
 
	# get the vendor id list as basic list...
	# -----------------------------------------
	@idquery = GetQuery("server",\%spec);
	for ($i=6;$i<@idquery;$i+=15) {
		push (@flag_list,$idquery[$i+5]); # get 3D flags
		push (@vid_list,$idquery[$i]);    # get Vendor ID`s
	}

	# hot query on xstuff...
	# ------------------------
	%query = GetHotQuery("xstuff",\%spec);
	$size  = 0;
	foreach $card (sort numbers keys %query) {
	if ($card ne "") {
		$detectvesa = 0;
		$detectfb   = 0;
		foreach $i (sort keys %{$query{$card}}) {
		SWITCH: for ($i) {
			# Ram DAC speeds...
			# ------------------
			/^Dacspeed/      && do {
			push(@dac_list,$query{$card}{$i});
			last SWITCH;
			};
			# Fixed clock values...
			# ----------------------
			/^Clock/         && do {
			push (@clock_list,$query{$card}{$i});
			last SWITCH;
			};
			# Memory detection...
			# ---------------------
			/^Memory/        && do {
			push (@mem_list,$query{$card}{$i});
			last SWITCH;
			};
			# The framebuffer Timing is used...
			# ----------------------------------
			/^FbTiming/      && do {
			$detectfb = 1;
			push(@fbt_list,$query{$card}{$i});
			last SWITCH;
			};
			# The VMware color depth...
			# --------------------------
			/^ColorDepth/    && do {
			push(@vm_list,$query{$card}{$i});
			last SWITCH;
			};
			# Display size in cm...
			# ------------------------
			/^Size/          && do {
			push (@dpi_list,$query{$card}{$i});
			last SWITCH;
			};
			# The VESA resolution list...
			# ----------------------------
			/^Vesa/          && do {
			$detectvesa = 1;
			push (@vesa_list,"true");
			last SWITCH;
			}; 
		}
		}
		if ($detectvesa == 0) {
			push (@vesa_list,"false");
		}
		if ($detectfb == 0) {
			push (@fbt_list ,"false");
		}
		$size++;
	}
	}

	# create default screen sections
	# -------------------------------
	my $IsStandardCase = 1;
	for($i=0;$i<@vid_list;$i++) {
		$var{Screen}{$i}{Identifier} = "Screen\[$i\]";
		$var{Screen}{$i}{Device}     = "Device\[$i\]";
		$var{Screen}{$i}{Monitor}    = "Monitor\[$i\]";
		SWITCH: for ($var{Device}{$i}{Driver}) {
		# standard VGA...
		# ----------------
		/vga/i     && do {
		%var = SetVGADefaultScreen(\%var,$i);
		$IsStandardCase = 0;
		last SWITCH;
		};
		# vmware driver...
		# ---------------- 
		/vmware/i  && do {
		%var = SetVMwareDefaultScreen(\%var,$i,$vm_list[$i]);
		last SWITCH;
		};
		# framebuffer driver...
		# ----------------------
		/fbdev/i   && do {
		%var = SetFbdevDefaultScreen(\%var,\%spec,$i);
		$IsStandardCase = 0;
		last SWITCH;
		}; 
		# default case...
		# -----------------
		%var = SetDefaultScreen(\%var,$i);
		}
	}

	# lookup for the resolution list
	# -------------------------------
	$mres = {};
	for($i=0;$i<@vid_list;$i++) {
	if ($vesa_list[$i] ne "true") {
		# we did not get any VESA mode information
		# assuming default is ok...
		# ...
	} else {
		# walk through the modes list and create a
		# unique and sorted resolution list
		# ----------------------------------
		@res  = ();
		$old  = "";
		$mem  = $mem_list[$i];
		foreach $vesanr (sort keys %{$query{$i}{Vesa}}) {
			@vesa  = split(/ /,$query{$i}{Vesa}{$vesanr});
			$mode  = join("x",$vesa[0],$vesa[1]);
			if ($mode ne $old) {
				push(@res,$mode);
			}
			$old = $mode;
		}
		# ...
		# sort the resolution list and remove the highest
		# one if the list contain more than one entry
		# ---
		@res = sortres(@res);
		# ...
		# To remove the highest resolution enable the following
		# line again...
		# ---
		# if (@res > 1) { shift (@res); }
		# ---
		foreach $bpp (8,15,16,24,32) { 
		if ($var{Device}{$i}{Driver} eq "fbdev") {
			$var{Screen}{$i}{Depth}{$bpp}{Modes} = "default";
		} else {
			$mode = CheckResolution(@res,$mem,$bpp,$i);
			if ($mode ne "") {
				$var{Screen}{$i}{Depth}{$bpp}{Modes} = $mode;
			}
		}
		}
	}
	}

	# calculate the Modelines needed for the resolutions
	# ---------------------------------------------------
	if ($IsStandardCase) {
	for($i=0;$i<@vid_list;$i++) {
		@sync  = split(/-/,$var{Monitor}{$i}{VertRefresh});
		$vsmax = $sync[1];
		@sync  = split(/-/,$var{Monitor}{$i}{HorizSync});
		$hsmax = $sync[1];
		$dac   = $dac_list[$i];
		$dac   = $dac * 1e6;
		$clock = $clock_list[$i];

		# ...
		# sort the resolution list and remove the highest
		# one if the list contain more than one entry
		# ---
		my @res = sortres(keys %{$mres{$i}}); 
		foreach (@res) {
			@xy  = split(/x/,$_);
			%var = GenerateModeline (
			\%spec,\%var,$vsmax,$hsmax,$xy[0],$xy[1],$i,"PClock"
			);
		}
	}
	}

	# set the FbTiming if it exist. We will delete the
	# calculated modelines and set the given timing 
	# ----------------------------------------------
	for($i=0;$i<@fbt_list;$i++) {
	if ($var{Device}{$i}{Driver} eq "vga") {
		next;
	}
	if ($fbt_list[$i] ne "false") {
		foreach (keys %{$var{Monitor}{$i}{Modeline}}) {
			delete $var{Monitor}{$i}{Modeline}{$_};
		} 
		$fbt_list[$i] =~ /\"(.*)\" (.*)/;
		$res = $1; $tim = $2;
		$var{Monitor}{$i}{Modeline}{0}{$res} = $tim;
 
		# enable the UseFbTiming algorithm...
		# -------------------------------------
		$architecture = qx(/bin/arch);
		if ($architecture =~ /ppc/i) {
			$var{Desktop}{$i}{CalcAlgorithm} = "UseFrameBufferTiming";
		}
	}

	# set display size information if defined...
	# -------------------------------------------
	if (defined $dpi_list[$i]) {
		@dpi  = split(/x/,$dpi_list[$i]);
		$dpix = $dpi[0] * 10;
		$dpiy = $dpi[1] * 10;
		$var{Monitor}{$i}{DisplaySize} = "$dpix $dpiy";
	} else {
		# /.../
		# Try to set the DisplaySize according to the
		# selected resolution
		# ---
		my $default = $var{Screen}{$i}{DefaultDepth};
		my $rstring = $var{Screen}{$i}{Depth}{$default}{Modes};
		my @reslist = split (/,/,$rstring);
		my $resused = shift (@reslist);

		@reslist = split (/x/,$resused);
		my $x = $reslist[0];
		my $y = $reslist[1];
		my $p = $x * $y;
		if ($p >= 1920000) {
		#=============================================================
		# equal or more than 1600 x 1200 pixels, assume 19'z monitor
		#-------------------------------------------------------------
		$var{Monitor}{$i}{DisplaySize} = "380 290";

		} elsif ($p > 786432) {
		#=============================================================
		# more than 1024 x 768 pixels assume 17'z monitor
		#-------------------------------------------------------------
		$var{Monitor}{$i}{DisplaySize} = "340 270";
		}
	}
	}
}

#----[ SetVMwareDefaultScreen ]-----#
sub SetVMwareDefaultScreen {
#------------------------------------------------
# if the vmware driver is used we had to set the
# color depth of the host system server
#
	my (%var)   = %{$_[0]};
	my $i       = $_[1];
	my $depth   = $_[2];
	$var{Screen}{$i}{DefaultDepth} = $depth;
	return (%var);
}

#----[ SetVGADefaultScreen ]----#
sub SetVGADefaultScreen {
#------------------------------------------------
# if the vga driver is used we need only a color
# depth of 4 bit and a generic chipset entry
#
	my (%var)  = %{$_[0]};
	my $i      = $_[1];
	$var{Screen}{$i}{DefaultDepth}          = "4";
	$var{Screen}{$i}{Depth}{4}{Modes}       = "640x480";
	$var{Screen}{$i}{Depth}{8}{Modes}       = "";
	$var{Screen}{$i}{Depth}{15}{Modes}      = "";
	$var{Screen}{$i}{Depth}{16}{Modes}      = "";
	$var{Screen}{$i}{Depth}{24}{Modes}      = "";
	$var{Screen}{$i}{Depth}{32}{Modes}      = "";
	$var{Device}{$i}{Videoram}              = "";
	$var{Device}{$i}{Chipset}               = "generic";

	# set standard vga modelines... 
	# ------------------------------
	$var{Monitor}{$i}{Modeline}{5}{"640x480"} 
		= "25.175 640 664 760 800 480 491 493 525";
	$var{Monitor}{$i}{Modeline}{4}{"640x480"} 
		= "25.18 640 664 760 800 480 491 493 525";
	$var{Monitor}{$i}{Modeline}{3}{"640x480"} 
		= "28.32 640 664 760 800 480 491 493 525";
	$var{Monitor}{$i}{Modeline}{2}{"640x480"} 
		= "28 640 664 760 800 480 491 493 525";
	$var{Monitor}{$i}{Modeline}{1}{"640x480"} 
		= "31.5 640 680 720 864 480 488 491 521";
	$var{Monitor}{$i}{Modeline}{0}{"640x480"} 
		= "31.5 640 656 720 840 480 481 484 500";

	# adapt sync range...
	# --------------------
	$var{Monitor}{$i}{VertRefresh} = "50-72";
	$var{Monitor}{$i}{HorizSync}   = "30-42";
	return(%var);
}

#----[ SetFbdevDefaultScreen ]----#
sub SetFbdevDefaultScreen {
#------------------------------------------------
# if the fbdev driver is used we need to ask
# for the framebuffer color depth to be the
# default
#
	my (%var)  = %{$_[0]};
	my (%spec) = %{$_[1]};
	my $i = $_[2];
	my $data  = FBSet::FbGetData();
	my $depth = $data->swig_depth_get();
	if ($depth == 32) {
		$depth = 24;
	}
	$var{Screen}{$i}{DefaultDepth}          = $depth;
	$var{Screen}{$i}{Depth}{8}{Modes}       = "";
	$var{Screen}{$i}{Depth}{16}{Modes}      = "";
	$var{Screen}{$i}{Depth}{15}{Modes}      = "";
	$var{Screen}{$i}{Depth}{24}{Modes}      = "";
	$var{Screen}{$i}{Depth}{32}{Modes}      = "";
	$var{Screen}{$i}{Depth}{$depth}{Modes}  = "default";
	return(%var);
}

#----[ SetDefaultScreen ]-----#
sub SetDefaultScreen {
#------------------------------------------------
# if not vga and fbdev we set the default
# to 640x480 at 16 bit color depth
#
	my (%var)  = %{$_[0]};
	my $i      = $_[1];
	$architecture = qx(/bin/arch);
	$var{Screen}{$i}{DefaultDepth}          = "16";
	$var{Screen}{$i}{Depth}{8}{Modes}       = "800x600";
	$var{Screen}{$i}{Depth}{16}{Modes}      = "800x600";
	$var{Screen}{$i}{Depth}{15}{Modes}      = "800x600";
	$var{Screen}{$i}{Depth}{24}{Modes}      = "800x600";
	$var{Screen}{$i}{Depth}{32}{Modes}      = "800x600";

	# special case for PPC... need 15bit colordepth
	# -----------------------------------------------
	if (($architecture =~ /ppc/i) && ($flag_list[$i] eq "DEFAULT")) {
		SWITCH: for ($var{Device}{$i}{Driver}) {
			/^r128|^radeon|^nv/  && do {
				$var{Screen}{$i}{DefaultDepth} = "16";
				last SWITCH;
			};
			# default case...
			# -----------------
			$var{Screen}{$i}{DefaultDepth} = "15";
		}
	}
	return(%var);
}

#----[ CheckResolution ]----#
sub CheckResolution {
#----------------------------------------------
# check the resolution list if it fits the
# requirements to the memory and color depth
# additionally create a global hash map
# containing all resolutions which we need 
# modelines for
#
	my @list = @_;
	my $card = pop(@list);
	my $bpp  = pop(@list);
	my $mem  = pop(@list);
	my @res;

	if ($mem eq "") {
		$modes = join(",",@list);
		return($modes);
	} 
	$mem = $mem * 1024; 
	foreach (@list) {
		@xy   = split(/x/,$_);
		$need = $xy[0] * $xy[1] * ($bpp / 8);
		# .../
		# removed the memory check to prevent wrong
		# memory detections to have any effect. The
		# X-Server is able to react appropriately if
		# such a case comes true
		# ----
		#if ($need <= $mem) {
		#	push(@res,$_); $mres{$card}{$_} = $_;
		#} 
		push(@res,$_); $mres{$card}{$_} = $_;
	}
	$modes = join(",",@res);
	return($modes);
}

#----[ GenerateModeline ]-----#
sub GenerateModeline {
#-----------------------------------------------------
# calculate modelines in a range between 70 and 
# vsmax Hz and save the result to the var hash
#
	my (%spec)    = %{$_[0]};
	my (%var)     = %{$_[1]};
	my $vsmax     = $_[2];
	my $hsmax     = $_[3];
	my $x         = $_[4];
	my $y         = $_[5];
	my $card      = $_[6];
	my $clock     = $_[7];
	my $clockline = $_[8];

	my $v;        # loop counter
	my $ml;       # raw modeline result
	my $oldtim;   # old timing
	my $tim;      # current timing
	my $res;      # modeline resolution string
	my @mode;     # modeline result as list
	my @clocks;   # clockline splitted
	my $clk;      # single clock in Hz

	$oldtim       = "";

	# if the vertical sync maximum is over 140 Hz we
	# will reduce the refresh rate to a maximum of 140 Hz
	# ----------------------------------------------------
	if ($vsmax > 140) {
		$vsmax = 140;
	}
	# programmable clocks requested
	# -------------------------------
	if ($clock eq "PClock") {
	for ($v=$vsmax;$v>=70;$v-=10)  {
		$ml = qx($spec{Xmode} -x $x -y $y -r $v -s $hsmax);
		@mode  = split(/\n/,$ml);
		$hsync = $mode[0];
		$vsync = $mode[1];
		$ml    = $mode[2];
		$ml    =~ /Modeline \"(.*)\" (.*)/;
		$res   = $1;
		$tim   = $2;
		if (($tim ne "") && ($tim ne $oldtim)) {
			$var{Monitor}{$card}{Modeline}{$ldnr}{$res} = $tim;
			$ldnr++;
		}
		$oldtim = $tim;
	}
	} else {
	# fixed clocks requested
	# ------------------------
	if ((defined $clockline) && ($clockline ne "")) {
		@clocks    = split(/ +/,$clockline);
		foreach (@clocks) {
		if ($_ eq "") { 
			next; 
		}
		$clk = $_ * 1e6;
		for ($v=$vsmax;$v>=70;$v-=10)  {
			$ml = qx($spec{Xmode} -x $x -y $y -r $v -n -d $clk -s $hsmax);
			@mode  = split(/\n/,$ml);
			$hsync = $mode[0];
			$vsync = $mode[1];
			$ml    = $mode[2];
			$ml    =~ /Modeline \"(.*)\" (.*)/;
			$res   = $1;
			$tim   = $2;
			if (($tim ne "") && ($tim ne $oldtim)) {
				$var{Monitor}{$card}{Modeline}{$ldnr}{$res} = $tim;
				$ldnr++;
			}
			$oldtim = $tim;
		}
		}
	}
	}
	return(%var);  
}

1;

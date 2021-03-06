#----[ AutoDetectMouse ]---------#
sub AutoDetectMouse {
#---------------------------------------------------
# this function calls the sysp -q mouse  routine 
# to get mouse information       
#
	my (%spec) = %{$_[0]}; # specs
	my (%var)  = %{$_[1]}; # config hash
	my %query;             # the query string
	my @prot_list;         # list of protocols 
	my @dev_list;          # list of devices
	my @btn_list;          # list of button numbers
	my @whl_list;          # list of wheel numbers
	my @emu_list;          # list of emulate btn flags
	my $mouse;             # mouse key in query hash
	my $value;             # hash value 
	my $i;                 # hash count
 
	%query = GetHotQuery("mouse",\%spec);
	if ($QueryStatus == -1) {
		return(-1);
	}
	foreach $mouse (sort numbers keys %query) {
	if ($mouse ne "") {
		push (@dev_list ,$query{$mouse}{Device});
		push (@prot_list,$query{$mouse}{Protocol});
		push (@btn_list, $query{$mouse}{Buttons});
		push (@whl_list, $query{$mouse}{Wheel});
		push (@emu_list, $query{$mouse}{Emulate});
	}
	}
	# /.../
	# set detected mouse protocols...
	# ------------------------------- 
	$index = 1;
	@prot_list = reverse(@prot_list);
	for($i=0;$i<@prot_list;$i++) {
		$var{InputDevice}{$index}{Identifier}       = "Mouse\[$index\]"; 
		$var{InputDevice}{$index}{Option}{Vendor}   = "Sysp";
		$var{InputDevice}{$index}{Option}{Name}     = "Autodetection";
		$var{InputDevice}{$index}{Option}{Protocol} = $prot_list[$i];
		if ($prot_list[$i] =~ /AUTO/i) {
			$var{InputDevice}{$index}{Option}{Option}  = "AutoSoft";
		}
		# this enables Emulate3Buttons per default...
		# --------------------------------------------
		# $var{InputDevice}{$index}{Option}{Emulate3Buttons} = "on";
		$index = $index + 2;
	}
	# /.../
	# set detected mouse devices...
	# -----------------------------
	$index = 1;
	@dev_list = reverse(@dev_list);
	for($i=0;$i<@dev_list;$i++) {
		$var{InputDevice}{$index}{Option}{Device} = $dev_list[$i];
		$index = $index + 2;
	}
	# /.../
	# set detected wheel options...
	# -------------------------------
	$index = 1;
	@whl_list = reverse(@whl_list);
	for($i=0;$i<@whl_list;$i++) {
	if ($whl_list[$i] > 0) { 
		$var{InputDevice}{$index}{Option}{ZAxisMapping} = "4 5";
	}
	$index = $index + 2;
	}
	# /.../
	# set detected button number...
	# ------------------------------
	$index = 1;
	@btn_list = reverse(@btn_list);
	for($i=0;$i<@btn_list;$i++) {
	if ($btn_list[$i] > 3) {
		$var{InputDevice}{$index}{Option}{Buttons} = $btn_list[$i];
	}
	$index = $index + 2;
	}
	# /.../
	# set detected emulate 3 button option...
	# -----------------------------------------
	$index = 1;
	@emu_list = reverse(@emu_list);
	for($i=0;$i<@emu_list;$i++) {
	if ($emu_list[$i] == 1) {
		$var{InputDevice}{$index}{Option}{Emulate3Buttons} = "on";
	}
	$index = $index + 2;
	}
	# /.../
	# set detected core layout...
	# -----------------------------
	$index = 1;
	for($i=0;$i<@dev_list;$i++) {
	if ($index >= 3) {
		$var{ServerLayout}{all}{InputDevice}{$index}{usage} = "SendCoreEvents";
		$var{ServerLayout}{all}{InputDevice}{$index}{id}    = 
        $var{InputDevice}{$index}{Identifier};
	}
	$var{InputDevice}{$index}{Driver} = "mouse";
	$index = $index + 2;
	}
	return(%var);
}

1;

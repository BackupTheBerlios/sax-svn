#====================================
# Init profile script
#------------------------------------
profile=$(basename $0 | cut -f1 -d.)
profile="/usr/share/sax/profile/$profile"
if [ ! -f "$profile" ] || [ "$UID" -ne 0 ];then
	exit 0
fi

#====================================
# Copy profile
#------------------------------------
cp $profile $profile.tmp
profile="$profile.tmp"

#====================================
# Check if profile is valid
#------------------------------------
fglrx=/usr/share/sax/profile/FireGL
if [ -f /usr/X11R6/lib/modules/drivers/fglrx_drv.o ];then
	cp $fglrx $profile
fi

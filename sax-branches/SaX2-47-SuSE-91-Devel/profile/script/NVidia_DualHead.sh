#/bin/sh

#====================================
# Init profile script
#------------------------------------
profile=$(basename $0 | cut -f1 -d.)
profile="/usr/X11R6/lib/sax/profile/$profile"
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
vendor=$(/usr/X11R6/lib/sax/sysp/script/vendor.pl nvidia)
if [ "$vendor" = "The XFree86 Project" ];then
	echo "Desktop -> [X] ->  CalcModelines = no"            > $profile
	echo "Monitor -> [X] ->  CalcAlgorithm = XServerPool"  >> $profile
fi

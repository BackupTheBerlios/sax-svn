#!/bin/sh

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
size=`hwinfo --monitor | grep Size: | cut -f2 -d:`
if [ ! -z "$size" ];then
	size=`echo $size | cut -f1 -d" "`
	xcm=`echo $size | cut -f1 -dx`
	ycm=`echo $size | cut -f2 -dx`
	xcm=`expr $xcm \* 2 \* 10`
	ycm=`expr $ycm \* 10`
	echo "Monitor -> [X] ->  DisplaySize = $xcm $ycm"  >> $profile
fi

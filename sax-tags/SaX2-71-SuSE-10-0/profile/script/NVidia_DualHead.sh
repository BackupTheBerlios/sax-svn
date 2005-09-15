#!/bin/sh

#====================================
# DualHead check
#------------------------------------
if [ "$1" = "check" ];then
	vendor=$(/usr/share/sax/sysp/script/vendor.pl nvidia)
	if [ "$vendor" = "The XFree86 Project" ] || \
	   [ "$vendor" = "X.Org Foundation" ];then
		echo single
	else
		echo dual
	fi
	exit 0
fi

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
vendor=$(/usr/share/sax/sysp/script/vendor.pl nvidia)
if [ "$vendor" = "The XFree86 Project" ] || \
   [ "$vendor" = "X.Org Foundation" ];then
	echo "Desktop -> [X] ->  CalcModelines = no"            > $profile
	echo "Monitor -> [X] ->  CalcAlgorithm = XServerPool"  >> $profile
fi

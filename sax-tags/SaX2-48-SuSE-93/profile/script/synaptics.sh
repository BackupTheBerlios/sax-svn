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
# Add default mouse if needed
#------------------------------------
sysp -q mouse | grep Profile | grep -qi "<undefined>"
if [ $? -ne 0 ];then
	echo "InputDevice->[X+2]->Identifier = Mouse[[X+2]]"    >> $profile
	echo "InputDevice->[X+2]->Driver     = mouse"           >> $profile
	echo "InputDevice->[X+2]->Option->InputFashion = Mouse" >> $profile 
	echo "InputDevice->[X+2]->Option->ZAxisMapping = 4 5"   >> $profile
	echo "InputDevice->[X+2]->Option->ButtonNumber = 5"     >> $profile
	echo "InputDevice->[X+2]->Option->Device = /dev/input/mice"     >> $profile
	echo "InputDevice->[X+2]->Option->Name = USB-Mouse;ExplorerPS/2 on USB" >> $profile
	echo "ServerLayout->all->InputDevice->[X+2]->id = Mouse[[X+2]]" >> $profile
	echo "ServerLayout->all->InputDevice->[X+2]->usage = SendCoreEvents" >> $profile
fi

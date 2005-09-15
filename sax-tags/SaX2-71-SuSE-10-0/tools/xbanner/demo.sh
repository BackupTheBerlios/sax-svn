#!/bin/sh
# Copyright (c) 2000 SuSE GmbH Nuernberg, Germany.  All rights reserved.
#
# Author: Marcus Schaefer <sax@suse.de>, 2000
#
# xbanner script to set label and penguin
#
# CVS ID:
# --------
# Status: Up-to-date
#

trap "signal" EXIT HUP INT TERM PIPE QUIT ABRT SEGV

#---[ init ]-----#
function init {
#--------------------------------
# set some variables
#
	XBANNER="/usr/X11R6/bin/xbanner"
	FREE="/usr/X11R6/bin/freetemp"
	XBOUND="/usr/sbin/xbound"
	FILE="/usr/sbin/demo"
	SCREEN="/usr/sbin/screen"
	XMIRROR="/usr/sbin/xmirror"

	DISP=$1
	if [ -z "$DISP" ];then
		DISP=$DISPLAY
	fi
	DSP=`echo $DISP | cut -f2 -d: | cut -f1 -d.`
}

#---[ getcolor ]-----#
function getcolor {
#-----------------------------------
# get color depth of root window
#
	COL=`xdpyinfo -display $DISP | \
		grep -i "depth of root window" | cut -f2 -d:`
	COL=`echo $COL | grep -i "^4 Planes"`
	if [ -z "$COL" ];then
		COL="ok"
	else
		COL=""
	fi
	echo $COL
}

#---[ getgeometry ]-----#
function getgeometry {
#-----------------------------------
# get dimension of the root window
#
	DIM=`xdpyinfo -display $DISP | \
		grep dimensions: | cut -f2 -d: | cut -f5 -d" "`
	echo $DIM
}

#---[ signal ]--------#
function signal {
#---------------------------------
# clean sweep if the job is done
#
	COUNT=0
	SCR=${#MPID[*]}
	while true;do
		kill ${MPID[$COUNT]} 2>/dev/null
		COUNT=`expr $COUNT + 1`
		if ([ "$COUNT" = "$SCR" ] || [ "$COUNT" = "10" ]);then
			break
		fi
	done

	COUNT=0
	SCR=${#BPID[*]}
	while true;do
		kill ${BPID[$COUNT]} 2>/dev/null
		COUNT=`expr $COUNT + 1`
		if ([ "$COUNT" = "$SCR" ] || [ "$COUNT" = "10" ]);then
			break
		fi
	done

	COUNT=0
	SCR=${#SPID[*]}
	while true;do
		kill ${SPID[$COUNT]} 2>/dev/null
		COUNT=`expr $COUNT + 1`
		if ([ "$COUNT" = "$SCR" ] || [ "$COUNT" = "10" ]);then
			break
		fi
	done
	exit 0
}

#-------------------------#
# main part....           #
#-------------------------#
init $1
COL=`getcolor`
DIM=`getgeometry`
SCR=0

for i in $DIM;do
	X=`echo $i | cut -f1 -dx`
	Y=`echo $i | cut -f2 -dx`

	# middle point...
	XMIDDLE=`expr $X / 2`; XMIDDLE=`expr $XMIDDLE - 100`
	YMIDDLE=`expr $Y / 2`; YMIDDLE=`expr $YMIDDLE - 50`

	# mirror size...
	XM=`expr $X - 380`
	YM=`expr $Y - 120`

	# banner size...
	X=`expr $X - 210`
	Y=`expr $Y - 80`

	$XBOUND -d :$DSP.$SCR >/dev/null 2>/dev/null &
	BPID[$SCR]=$!
	if [ "$COL" = "ok" ];then
		$XBANNER -file $FILE -display :$DSP.$SCR -px $X -py $Y &>/dev/null
		$XMIRROR -d :$DSP.$SCR -x $XM -y $YM \
			--width 380 --height 120 2>/dev/null &
		MPID[$SCR]=$!
		$XBANNER -label "( $SCR )" -file $SCREEN -display :$DSP.$SCR 2>/dev/null
		$XMIRROR -d :$DSP.$SCR -x $XMIDDLE -y $YMIDDLE \
			--width 150 --height 70 2>/dev/null &
		SPID[$SCR]=$!
	fi

	SCR=`expr $SCR + 1`
done

while true;do
	sleep 1
done


#!/bin/sh
# Copyright (c) 2000 SuSE GmbH Nuernberg, Germany.  All rights reserved.
#
# Author: Marcus Schaefer <sax@suse.de>, 1999
# SaX (sax) configuration level 3
#
# CVS ID:
# --------
# Status: Up-to-date
#
#==================================
# Variables...
#----------------------------------
INIT="/usr/X11R6/lib/sax/init.pl"
XC="/usr/X11R6/lib/sax/xc.pl"
DOTS="/usr/X11R6/lib/sax/tools/dots"
PCI="/usr/X11R6/lib/sax/pci.pl"
XFT="/var/cache/xfine/*"
ERR="/var/log/SaX.log"
REF="/var/cache/sax/files/XF86Config.first"
CVSB="/usr/X11R6/lib/sax/svnbuild"

#==================================
# Init option variables
#----------------------------------
QUIET="--quiet no"
CMDLINE=$*
FASTPATH=""
QUICK_START=""
GPMSTATUS=3
BATCH_MODE=""
AUTO_CONF=""
LOW_RES=""
XMODE=""
MODLIST=""
VESA=""
AUTOMODE=""
CHIP=""
GPM=""
NODE=""
TYPE=""
NOSCAN=""
SYS_CONFIG=""
DBMNEW=""
IGNORE_PROFILE=""
DIALOG=""
REINIT=0

#==================================
# Functions...
#----------------------------------
function quit() {
	/sbin/killproc $DOTS
	exit 1
}
function privilege() {
	if [ ! $UID = "0" ];then
		echo "SaX: only root can do this"
		exit 0
	fi
}
function StartGPM() {
	if [ "$GPMSTATUS" = 0 ];then 
	if [ -f "/sbin/rcgpm" ];then
		/sbin/rcgpm start 2>/dev/null >/dev/null
	fi
	fi
}
function StopGPM() {
	if [ ! -f "/sbin/rcgpm" ];then
		return
	fi
	pidof gpm 2>/dev/null >/dev/null
	GPMSTATUS=$?
	if [ "$GPMSTATUS" = 0 ];then
		/sbin/rcgpm stop 2>/dev/null >/dev/null
	fi
}
function version() {
	ID='$Id: sax.sh,v 1.49 2003/03/17 13:39:51 ms Exp $'
	ID=`echo $ID | cut -f3-4 -d" "`
	echo "SaX2 version 4.8 - CVS Release: $ID" 
}
function usage() {
	/sbin/killproc $DOTS
	# ...
	echo "Linux SaX Version 4.8 (2002-03-22)"
	echo "(C) Copyright 2002 - SuSE GmbH <Marcus Schaefer sax@suse.de>"
	echo 
	echo "usage: SaX [ options ]"
	echo "options:"
	echo "[ -h | --help ]"
	echo "  show this message and exit"
	echo
	echo "[ -b | --batchmode [ filename ]]"
	echo "  enable batch mode to set configuration options"
	echo "  via the SaX2 shell. Optionally give a full qualified file"
	echo "  location holding the profile data"
	echo
	echo "[ -a | --auto ]"
	echo "  enable automatic configuration." 
	echo
	echo "[ -l | --lowres ]"
	echo "  use only 640x480 60 Hz standard mode."
	echo "  DDC detection is switched off in this case"
	echo
	echo "[ -V | --vesa ]"
	echo "  This option will set a given resolution and vertical sync"
	echo "  value (in Hz) as VESA standard resolution for a specific"
	echo "  card. Format:  Card:XxY@VSync"
	echo "  Example: 0:1024x768@85";
	echo
	echo "[ -m | --modules ]"
	echo "  comma seperated list of X-Server modules"
	echo "  for example: -m 0=mga,1=nv"
	echo
	echo "[ -c | --chip ]"
	echo "  chip number(s) to scan"
	echo
	echo "[ -x | --xmode ]"
	echo "  use server build in Modelines"
	echo
	echo "[ -u | --automode ]"
	echo "  use server mode suggestion"
	echo 
	echo "[ -p | --pci ]"
	echo "  show PCI/AGP information"
	echo
	echo "[ -n | --node ]"
	echo "  set device node to use for the core pointer"
	echo
	echo "[ -t | --type ]"
	echo "  set protocol type to use for the core pointer"
	echo
	echo "[ -g | --gpm ]"
	echo "  use gpm as repeater of mouse events"
	echo
	echo "[ -s | --sysconfig ]"
	echo "  this option tell SaX2 to import the system wide"
	echo "  config file even if SaX2 was started from a textconsole"
	echo "  which normaly will import the SaX2 HW detection"
	echo "  data"
	echo
	echo "[ -i | --ignoreprofile ]"
	echo "  do not include profiles which are normally applied"
	echo "  automatically"
	echo 
	echo "[ -r | --reinit ]"
	echo "  remove detection database and re-init the"
	echo "  hardware database"
	echo
	echo "[ -v | --version ]"
	echo "  print version information and exit"
}

#==================================
# get options
#----------------------------------
TEMP=`getopt -o gb::alhxm:uc:pn:t:vsrV:dO:i --long gpm,batchmode::,auto,lowres,help,xmode,modules:,automode,chip:,pci,node:,type:,version,sysconfig,reinit,vesa:,dbmnew,ignoreprofile,dialog: \
-n 'SaX' -- "$@"`

if [ $? != 0 ] ; then usage ; exit 1 ; fi
eval set -- "$TEMP"

while true ; do
	case "$1" in
	-b|--batchmode)             # set batch mode and apply file or give
		BATCH_MODE="-b"         # us an interactive shell
	shift 2 ;;

	-a|--auto)                  # create automatic configuration and exit
		AUTO_CONF="-a"          # registry is used if -q is used
	shift ;;

	-r|--reinit)                # remove /var/cache/sax/files to reinit
		REINIT=1                # hardware database
	shift ;;

	-d|--dbmnew)                # use the config.new file instead of the
		DBMNEW="--dbmnew"       # normal config storable file
	shift;;

	-i|--ignoreprofile)         # ignore profiles automatically set
		IGNORE_PROFILE="-i"     # via CDB information
	shift;;

	-O|--dialog)                # start with custom dialog
		DIALOG="-O $2"
	shift 2 ;;

	-l|--lowres)                # enable 640x480 virtual screen to make
		LOW_RES="-v"            # sure this mode is used for config
	shift ;;

	-s|--sysconfig)             # read the system installed config file
		SYS_CONFIG="-s"         # instead of the HW detection data
	shift ;;                    # only take effect if SaX2 start its own server

	-u|--automode)              # do not set my mode suggestion, let X11
		AUTOMODE="-u"           # select the resolution
	shift ;;

	-h|--help)                  # get help message...
		usage ;  exit 0 ;;

	-v|--version)               # print version information...
		version; exit 0 ;;

	-x|--xmode)                 # do not use my modelines use the X11
		XMODE="-x"; shift ;;    # mode pool instead

	-m|--modules)               # set module(x) to use for card X...
		MODLIST="-m $2"  
	shift 2 ;;

	-V|--vesa)                  # set resolution and vsync value to
		VESA="--vesa $2"        # build a special profile for activating
	shift 2 ;;                  # this resolution @ vsync

	-c|--chip)                  # set chip(s) to configure...
		CHIP="-c $2"     
	shift 2 ;;

	-n|--node)                  # set device node for primary mouse
		NODE="-n $2"            # no multiple mouse device support
	shift 2 ;;

	-t|--type)                  # set primary mouse protocol type
		TYPE="-t $2"            # no multiple mouse device support
	shift 2 ;;

	-p|--pci)                   # show the detected cards and exit  
		privilege; $PCI 
	exit 0 ;;

	-g|--gpm)                   # use gpm as repeater for the mouse
		GPM="-g"                # GPM use /dev/gpmdata and MouseSystems 
	shift ;;

	--)
	if [ "$BATCH_MODE" = "-b" ];then
		BATCH_MODE="-b $2"
	fi
	shift;
	break;;

	*) 
	echo "SaX: Internal error!" ; exit 1 ;;
	esac
done

#==================================
# prove root privilege
#----------------------------------
privilege

#==================================
# setup QT home dir...
#----------------------------------
temp_QT_HOME_DIR="`eval echo ~$USER/.qt/`"
if test -r "$temp_QT_HOME_DIR/qtrc"; then
	export QT_HOME_DIR="$temp_QT_HOME_DIR"
fi

#==================================
# run the dots...
#----------------------------------
nice -n 19 $DOTS &
sleep 1

#==================================
# export LANG from user environment
#----------------------------------
TMPFILE=`ls -1 /tmp/sax_lenv* 2>/dev/null` 
if [ -f "$TMPFILE" ];then
	export LANG=`cat $TMPFILE`
else
	export LANG=$LANG 
fi

#==================================
# if POSIX use en_US language
#----------------------------------
if [ -z "$LANG" ] || [ "$LANG" = "POSIX" ];then
 export LANG="en_US"
fi

#==================================
# clean sweep...
#----------------------------------
rm -f  $TMPFILE $REF
rm -rf $XFT $ERR
if [ "$REINIT" = 1 ];then
	rm -f /var/cache/sax/files/*
fi

#==================================
# add ver. and commandline to log
#----------------------------------
BNR="unknown"
if [ -f "$CVSB" ];then
	BNR=`cat $CVSB`
fi
CVS=`version`
DATE=`LANG=POSIX date`
FBSET=`fbset -i 2>/dev/null`
echo "/*************"                                                   >> $ERR
echo "SaX2 log     : $CVS"                                              >> $ERR
echo "**************"                                                   >> $ERR
echo "CVS BUILD    : $BNR"                                              >> $ERR
echo "             :"                                                   >> $ERR
echo "DESCRIPTION  : X11 configuration log file to collect information" >> $ERR
echo "             : about detection, startup and configuration."       >> $ERR
echo "             : There are three parts of logging:"                 >> $ERR
echo "             : ---"                                               >> $ERR
echo "             : 1) INIT     ( detection, 3D )"                     >> $ERR
echo "             : 2) STARTUP  ( XF86Config, X11 log, glxinfo )"      >> $ERR
echo "             : 3) CONFIG   ( config actions )"                    >> $ERR
echo "             : ---"                                               >> $ERR
echo "             :"                                                   >> $ERR
echo "VERSION      : SaX2 compiled for: [SuSE-Linux]"                   >> $ERR
if [ -z "$CMDLINE" ];then
	echo "PARAMETER    : no parameter(s) given"                         >> $ERR
else
	echo "PARAMETER    : $CMDLINE"                                      >> $ERR
fi
echo "             :"                                                   >> $ERR
echo "LOG DATE     : $DATE"                                             >> $ERR
echo "*************/"                                                   >> $ERR
echo "============================"                                     >> $ERR
echo "Framebuffer Info:"                                                >> $ERR
echo "----------------------------"                                     >> $ERR
if [ -z "$FBSET" ];then
	echo "Framebuffer not active"                                       >> $ERR
else
	echo "$FBSET"                                                       >> $ERR
fi
echo                                                                    >> $ERR

#==================================
# remove hardware.chg if exist...
#----------------------------------
if [ "$SYS_CONFIG" = "-s" ];then
	rm -f "/var/cache/sax/files/hardware.chg"
fi

#==================================
# set option strings...
#----------------------------------
XC_OPT="$XMODE $AUTO_CONF $SYS_CONFIG $DIALOG"
IN_OPT="$LOW_RES $BATCH_MODE $MODLIST $AUTOMODE $CHIP"
IN_OPT="$IN_OPT $GPM $NODE $TYPE $VESA $DBMNEW $QUIET $IGNORE_PROFILE"

#==================================
# call init now...
#----------------------------------
StopGPM
/sbin/killproc $DOTS
echo -ne "\r"
echo "SaX: initializing please wait..."
$INIT $IN_OPT
if [ ! $? = 0 ];then
	quit
fi
 
#==================================
# start X11 manager... 
#----------------------------------
echo "SaX: startup"
if [ -z "$AUTO_CONF" ];then
	$XC $XC_OPT 2>>$ERR
else
	echo; echo "SaX: creating config file please wait..."
	echo
	$XC $XC_OPT 2>>$ERR
fi

#==================================
# restart GPM and exit
#----------------------------------
StartGPM; echo
exit 0


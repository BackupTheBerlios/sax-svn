#!/bin/bash
#
# DrvSelectInSecBootMode.sh
#

function Native()
{
    echo "native"
    exit 0
}

function Modesetting16() 
{
    echo "modesetting16"
    exit 0
}

function Modesetting24() 
{
    echo "modesetting24"
    exit 0
}

function Fbdev()
{
    echo "fbdev"
    exit 0
}

secureboot=off
if mokutil --sb-state 2>/dev/null | grep -q "SecureBoot enabled"; then
    secureboot=on
elif xxd -p \
    /sys/firmware/efi/vars/SecureBoot-8be4df61-93ca-11d2-aa0d-00e098032b8c/data 2>/dev/null | \
    grep -q "01"; then
    secureboot=on
elif cat /proc/cmdline | grep -q "secureboot_enable=1"; then
    secureboot=on
fi

if [ "$secureboot" == "off" ]; then
    Native
else
    if [ -r /proc/fb ]; then
	fb=$(cat /proc/fb)
        ### for testing purposes
        #fb="0 UEFI VGA"
        #fb="0 radeondrmfb"
        #fb="0 inteldrmfb"
        #fb="0 mgadrmfb"
        #fb="0 cirrusdrmfb"
        #fb="0 astdrmfb"
	case $fb in
            *radeon*|\
            *intel*) 
		Native 
		;;
	    *mga*)
		Modesetting24
		;;
            *cirrus*)
		Modesetting16
		;;
            #astdrmfb, UEFI VGA
            *)
		Fbdev
		;;
	esac
    else
	Native
    fi
fi

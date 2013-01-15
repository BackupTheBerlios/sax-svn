#!/bin/bash
#
# KMSAndNotIntelOrAMD.sh
#

function No()
{
    echo "no"
    exit 0
}

function Yes() 
{
    echo "yes"
    exit 0
}

if [ -r /proc/fb ]; then
    fb=$(cat /proc/fb)
    ### for testing purposes
    #fb=fb
    #fb="0 radeondrmfb"
    #fb="0 inteldrmfb"
    #fb="0 mgadrmfb"
    if echo $fb | grep -q drmfb; then
        case $fb in
            *radeon*|\
            *intel*) 
                No 
                ;;
            *) 
                Yes 
                ;;
        esac
    else
        No
    fi
fi
No

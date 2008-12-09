#!/bin/sh

if [ $# -ne 1 ]; then
  echo "Usage: $(basename $0) <xkb_layout>"
  exit 1
fi

layout=""
layout=$(grep -E \"${1}\" /usr/share/YaST2/data/xkblayout2keyboard.ycp | \
         cut -d ":" -f 2 | sed -e 's/\"//g' -e 's/,//g' -e 's/ //g')

if [ "$layout" == "" ]; then
  echo "No appropriate console keyboard layout found for $1"
  exit 1
else
  echo "Setting console keyboard layout to $layout (xkb-layout: $1)"
  yast keyboard set layout=${layout}
fi

#!/usr/bin/env bash

status=0


make TOOL=gcc_arm CONFIG=release  LOAD=ram clean 
if [ "$?" != "0" ]; then
    status=-1
fi

if [ "${1}" != "nopause" ]; then
read -p "Press any key to continue... " -n1 -s
fi

exit $status


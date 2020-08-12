#!/bin/bash
if [ -z $1 ]
then
echo "Usage: $0 filename"
echo "Example: $0 MobyDick"
exit 1
fi
NM=$1
export NM
cat $NM | vocab | sort >$NM.lst

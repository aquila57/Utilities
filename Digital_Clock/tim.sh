#!/bin/bash
if [ -z $1 ]
then
echo "Usage: $0 #seconds"
echo "Example: $0 300"
exit 1
fi
cat /etc/crsclk/9x15.txt | ./tim $1

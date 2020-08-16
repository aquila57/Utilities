#!/bin/bash
if [ -z $1 ]
then
echo "Usage: $0 filename"
echo "Example: $0 mylist.txt"
fi
python scrmbl.py <$1

#!/bin/sh

###
# to make this script executable via cmd+O or double click,
# simply rename it from `autorun.sh` to `autorun.command`.
# you might also need to change the path to `audioIO.app` in $COMMAND
##

export DISPLAY=:0.0

PROGRAM='audioIO.app'
COMMAND='open -n ./bin/audioIO.app --args --autostart'

while true; do
  if ps aux | grep -v grep | grep $PROGRAM
    then
      # echo "found"
    else
      echo "relaunching"
      $COMMAND
  fi
done

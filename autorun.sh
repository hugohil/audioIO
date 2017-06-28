#!/bin/sh

export DISPLAY=:0.0

PROGRAM='audioIO.app'
COMMAND='open -n ./bin/audioIO.app --args --host 127.0.0.1 --device 4 --autostart'

while true; do
  if ps aux | grep -v grep | grep $PROGRAM
    then
      echo "found"
    else
      echo "relaunching"
      $COMMAND
  fi
done

#!/usr/bin/env bash
pid=$1
timedelay=$2

sleep $timedelay
if [ -d "/proc/${pid}" ]
then
  kill $pid
  echo "Killed $pid"
fi

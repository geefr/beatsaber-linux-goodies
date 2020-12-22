#!/usr/bin/env bash

function testinarray {
  local -n array=$1
  local -n testvalue=$2

  for i in "${array[@]}"
  do
    if [ "$i" = "$testvalue" ]
    then
      return 1
    fi
  done
  return 0
}

oldpids=()

while true
do
  pids=( $(pgrep mscorsvw.exe | tr "\n" " ") )
  newpids=()
  for pid in "${pids[@]}"
  do
    testinarray oldpids pid # If pid is new
    if [ "$?" = "0" ]
    then
      newpids+=("$pid")
      ./workarounds/proc-stopper.sh $pid 60 &
    fi
  done
  #echo "PID DEBUG BELOW"
  #echo "All: ${pids[@]}"
  #echo "New: ${newpids[@]}"
  #echo "Old: ${oldpids[@]}"
  oldpids=( ${pids[@]} )
  sleep 1
done

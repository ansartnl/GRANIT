#!/bin/bash

SCRIPT=$( readlink -f $0 )
THIS_DIR=$( dirname $SCRIPT )
cd $THIS_DIR

CURRENT_DATE_TIME=`date +'%Y.%m.%d_%H-%M-%S'`

read host
read ip
vars=

while read oid val
do
  if [ "$vars" = "" ]
  then
    vars="$oid=$val"
  else
    vars="$vars $oid=$val"
  fi
done

/home/user/netSnmpTrapHandler $1 $host "$ip" $vars

echo [$CURRENT_DATE_TIME] trap: $1 // $host // $ip // $vars >> /home/user/1.sh.out

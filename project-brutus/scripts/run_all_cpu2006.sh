#!/bin/sh

# check the number of arguments
if [ $# -ne 2 ]
then
  echo "Usage: $0 <config_name> <max_parallel_tasks>"
  exit
fi

script_path=`readlink -f $0`
basedir=`dirname "$script_path"`
cfg=$basedir/../exp/cfg/$1.cfg

# check if config file exists
if [ ! -s $cfg ]
then
  echo "$0 : Config file \"$cfg\" not found"
  exit
fi

~/parallel -vv --no-run-if-empty -j $2 --colsep " " --bar $basedir/run.py {1} --chkpt {2} --cfg $1 -q --warm 500000 --svm_no_logallinsts :::: $basedir/simlist.txt


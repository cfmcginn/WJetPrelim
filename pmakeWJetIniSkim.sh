#!/bin/bash
if [ $# -ne 4 ]
then 
  echo "Usage: ./pmakeWJetIniSkim.sh <inputList> <sampleType> <outName> <outDir>"
  exit 1
fi

now="WJetIniSkimTreeJob_$(date +"%m_%d_%Y__%H_%M_%S")"
mkdir $now
mkdir -p $4
len=`wc -l $1 | awk '{print $1}'`
cp makeWJetIniSkim.sh $now
cp $1 $now

NAME="makeWJetIniSkim.C"
g++ $NAME $(root-config --cflags --libs) -Werror -Wall -O2 -o "${NAME/%.C/}.exe"

cp makeWJetIniSkim.exe $now

cat pmakeWJetIniSkim.condor | sed "s@log_flag@$now@g" | sed "s@dir_flag@$PWD/$now@g" | sed "s@arg1@$1@g" | sed "s@arg2@$2@g" | sed "s@arg3@$3@g" | sed "s@arg4@$4@g" | sed "s@njobs@$len@g" > $now/pmakeWJetIniSkim.condor
echo -=-
cat $now/pmakeWJetIniSkim.condor
echo -=-

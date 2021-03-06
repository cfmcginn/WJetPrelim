#!/bin/bash

if [ $# -ne 3 ]
then 
  echo "Usage: ./pmakeWJetAnaSkim.sh <inputList> <sType> <outDir>"
  exit 1
fi

now="WJetAnaSkimTreeJob_$(date +"%m_%d_%Y__%H_%M_%S")"
mkdir $now
mkdir -p $3
len=`wc -l $1 | awk '{print $1}'`
cp makeWJetAnaSkim.sh $now

cp $1 $now

NAME="makeWJetAnaSkim.C"
g++ $NAME $(root-config --cflags --libs) -lHistPainter -Werror -Wall -O2 -o "${NAME/%.C/}.exe"
cp makeWJetAnaSkim.exe $now

cat pmakeWJetAnaSkim.condor | sed "s@log_flag@$now@g" | sed "s@dir_flag@$PWD/$now@g" | sed "s@arg1@$1@g" | sed "s@arg2@$2@g" | sed "s@arg3@$3@g" | sed "s@njobs@$len@g" > $now/pmakeWJetAnaSkim.condor
echo -=-
cat $now/pmakeWJetAnaSkim.condor
echo -=-

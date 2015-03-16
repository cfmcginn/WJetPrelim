#!/bin/bash

if [ $# -ne 4 ]
then 
  echo "Usage: ./makeWJetAnaSkim.sh <inputList> <sType> <outDir> <#>"
  exit 1
fi

echo | awk -v inputList=$1 -v sType=$2 -v num=$4 '{print "./makeWJetAnaSkim.exe \""inputList"\" \""sType"\" \""num"\""}' | bash

mv *Skim*.root $3
rm *.root 

echo "job done successfully"
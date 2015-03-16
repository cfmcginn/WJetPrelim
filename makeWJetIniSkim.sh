#!/bin/bash
if [ $# -ne 5 ]
then 
  echo "Usage: ./makeWJetIniSkim.sh <inputList> <sampleType> <outputFile> <outDir> <#>"
  exit 1
fi

echo | awk -v inputList=$1 -v sampleType=$2 -v outputFile=$3 -v num=$5 '{print "./makeWJetIniSkim.exe \""inputList"\" \""sampleType"\" \""outputFile"\" \""num"\""}' | bash
mv $3_$5*.root $4
rm *.root

echo "job done successfully"
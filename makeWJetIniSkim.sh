#!/bin/bash
if [ $# -ne 6 ]
then 
  echo "Usage: ./makeWJetIniSkim.sh <inputList> <sampleType> <isMinBias> <outputFile> <outDir> <#>"
  exit 1
fi

echo | awk -v inputList=$1 -v sampleType=$2 -v isMinBias=$3 -v outputFile=$4 -v num=$6 '{print "./makeWJetIniSkim.exe \""inputList"\" \""sampleType"\" \""isMinBias"\" \""outputFile"\" \""num"\""}' | bash
mv $4_$6*.root $5
rm *.root

echo "job done successfully"
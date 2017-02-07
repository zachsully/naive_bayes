#!/bin/bash

echo "Running Baseline"
# ./nb

echo "Running Gibbs updates"

echo "Lang,Docs,Categories,Vocab,Trial,Time" > gibbstimes.csv
for i in `seq 1 10`; do
  ./NBHakaru 10 3 100 $i >> gibbstimes.csv
done

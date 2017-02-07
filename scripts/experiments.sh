#!/bin/bash

echo "Alg,Docs,Categories,VocabSize,Trial,Time" > experiments.csv
for docs in "20" "30" "40" "50" "60" "70" "80"; do
    for i in `seq 10`; do
        ./NBHakaru $docs 10 100 $i >> experiments.csv;
        done;
done;

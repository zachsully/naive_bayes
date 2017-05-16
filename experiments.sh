#!/bin/bash

echo "Alg,Docs,Categories,VocabSize,Trial,Time" > experiments.csv
for docs in $(seq 20 10 80); do
    for i in $(seq 10); do
        ./NBHakaru $docs 10 100 $i >> experiments.csv;
        done;
done;

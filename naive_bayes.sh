#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "./naive_bayes.sh <docsPerTopic> <trial>"
    exit
fi

start=`date +%s.%N`
./GetNews $1
./naive_bayes.R $1 $2
end=`date +%s.%N`
echo "$end - $start" | bc -l

if [ -f nbsimp.hk ]; then
    rm nbsimp.hk nbsimp2.hk GibbsOptBucket.hs
fi
start=`date +%s.%N`
simplify naive_bayes_gibbs.hk > nbsimp.hk
perl unsample.pl nbsimp.hk > nbsimp2.hk
summary nbsimp2.hk -o GibbsOptBucket.hs -M GibbsOptBucket --logfloat-prelude
ghc -v0 -O2 NaiveBayesMain.hs -o NaiveBayes
./NaiveBayes $1 $2
end=`date +%s.%N`
echo "$end - $start" | bc -l

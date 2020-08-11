#!/bin/bash

OMA="$HOME/01-DNA-inz/00-Prototype/OMA_Prototype/cpp/bin/Release/OMA-program"

# for f in normal/*.binmap;
# do 
#     $OMA -m 100 -i $f -o "$f.result"
# done

for f in ./*.binmap;
do 
    $OMA -m 50 -i $f -o "$f.result"
done

#!/bin/bash

inst="./inst/*"

for f in $inst
do
echo $f
(python3 src/main.py < $f) > ./solutions/$f.Solved.txt
done

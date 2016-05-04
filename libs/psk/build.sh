#!/bin/bash

mkdir tmp

cd tmp
cmake ../
make

mv libpsk.a ../../

cd ..
rm -rf tmp

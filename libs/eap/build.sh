#!/bin/bash

mkdir tmp

cd tmp
cmake ../
make

mv libeap.a ../../

cd ..
rm -rf tmp

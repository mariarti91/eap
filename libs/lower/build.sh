#!/bin/bash

mkdir tmp
cd tmp

cmake ../
make

cp liblower.a ../../
cd ..

rm -rf tmp

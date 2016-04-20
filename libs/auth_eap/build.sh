#!/bin/bash

mkdir tmp

cd tmp
cmake ../
make

mv libauth.a ../../

cd ..
rm -rf tmp

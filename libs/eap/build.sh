#!/bin/bash

mkdir tmp

cd tmp
cmake ../eap
make

mv libeap.a ../

cd ..
rm -rf tmp

#!/bin/bash

cd tmp

cmake ../
make

cp liblower.a ../../
cd ..

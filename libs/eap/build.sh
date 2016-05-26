#!/bin/bash

cd tmp
cmake ../
make

mv libeap.a ../../

cd ..

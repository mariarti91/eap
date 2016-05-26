#!/bin/bash

cd tmp
cmake ../
make

mv libpsk.a ../../

cd ..

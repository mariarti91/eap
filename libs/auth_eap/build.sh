#!/bin/bash

cd tmp
cmake ../
make

mv libauth.a ../../

cd ..

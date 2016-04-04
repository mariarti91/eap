#!/bin/bash

mkdir tmp

cd tmp
cmake ../
make

mv eap_server.elf ../..

cd ..
rm -rf tmp

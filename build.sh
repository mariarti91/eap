#!/bin/bash

mkdir tmp

cd tmp
cmake ../src
make

mv eap_peer.elf ../

cd ..
rm -rf tmp

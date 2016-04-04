#!/bin/bash

mkdir tmp

cd tmp
cmake ../
make

mv eap_peer.elf ../..

cd ..
rm -rf tmp

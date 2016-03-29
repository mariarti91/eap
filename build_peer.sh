#!/bin/bash

mkdir tmp

cd tmp
cmake ../peer
make

mv eap_peer.elf ../

cd ..
rm -rf tmp

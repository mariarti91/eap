#!/bin/bash

cd tmp
cmake ../
make

mv eap_peer.elf ../..

cd ..

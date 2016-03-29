#!/bin/bash

mkdir tmp

cd tmp
cmake ../serv
make

mv eap_server.elf ../

cd ..
rm -rf tmp

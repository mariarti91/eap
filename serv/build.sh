#!/bin/bash

cd tmp
cmake ../
make

mv eap_server.elf ../..

cd ..

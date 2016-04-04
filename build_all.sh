#!/bin/bash

cd libs/lower && ./build.sh && cd ..
cd eap && ./build.sh && cd ../..

cd serv && ./build.sh && cd ..
cd peer && ./build.sh && cd ..

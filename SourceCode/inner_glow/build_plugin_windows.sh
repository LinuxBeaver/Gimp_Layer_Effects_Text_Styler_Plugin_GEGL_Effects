#!/bin/bash


mkdir WindowsBinaries

TOP=$(pwd)  

chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'inner-glow.dll') $TOP/WindowsBinaries

cd .. 

cd port_load && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'loadport.dll') $TOP/WindowsBinaries
 



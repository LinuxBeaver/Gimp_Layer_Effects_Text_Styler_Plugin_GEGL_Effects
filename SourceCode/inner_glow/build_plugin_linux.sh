#!/bin/bash


mkdir LinuxBinaries

TOP=$(pwd)  

chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'inner-glow.so') $TOP/LinuxBinaries

cd .. 

cd port_load && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'loadport.so') $TOP/LinuxBinaries
 



#!/bin/bash

mkdir LinuxBinaries

TOP=$(pwd)  

chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'gegleffectspending.so') $TOP/LinuxBinaries

TOP=$(pwd)  

cd ..

cd bevel && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'bevel.so') $TOP/LinuxBinaries

cd .. 

cd inner_glow && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'inner-glow.so') $TOP/LinuxBinaries

cd .. 

cd glass_over_text && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'glassovertext.so') $TOP/LinuxBinaries
 

cd .. 

cd zzstrokebevelimage && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'zzstrokebevelimage.so') $TOP/LinuxBinaries
 

cd .. 



#!/bin/bash

mkdir WindowsBinaries

TOP=$(pwd)  

chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'gegleffectspending.dll') $TOP/WindowsBinaries

TOP=$(pwd)  

cd ..

cd bevel && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'bevel.dll') $TOP/WindowsBinaries

cd .. 

cd inner_glow && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'inner-glow.dll') $TOP/WindowsBinaries

cd .. 

cd glass_over_text && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'glassovertext.dll') $TOP/WindowsBinaries
 

cd .. 

cd zzstrokebevelimage && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'zzstrokebevelimage.dll') $TOP/WindowsBinaries
 

cd .. 


 

#!/bin/bash

mkdir WindowsBinaries

TOP=$(pwd)  

chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*gegleffectsrework.dll') $TOP/WindowsBinaries


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

cd custom_bevel && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'cbevel.dll') $TOP/WindowsBinaries

cd .. 

cd ssg && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*ssg.dll') $TOP/WindowsBinaries

cd .. 



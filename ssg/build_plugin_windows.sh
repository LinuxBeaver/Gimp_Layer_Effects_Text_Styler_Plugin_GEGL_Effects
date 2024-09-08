#!/bin/bash


mkdir WindowsBinaries

TOP=$(pwd)  

chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*.dll') $TOP/WindowsBinaries





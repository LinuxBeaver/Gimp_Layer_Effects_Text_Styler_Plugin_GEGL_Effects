#!/bin/bash


mkdir LinuxBinaries

TOP=$(pwd)  

chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*.so') $TOP/LinuxBinaries





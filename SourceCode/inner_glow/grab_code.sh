#!/bin/bash

mkdir SourceCode

TOP=$(pwd)  

cd ..

## Type folder names here for each plugin (nothing else is needed)

cp -r inner_glow port_load  $TOP/SourceCode

cd $TOP/SourceCode && gio trash */build_plugin_windows.sh  */build_plugin_linux.sh */grab_code.sh */.zip */SourceCode */build */windows_binaries_gegl_plugin.zip */linux_binaries_gegl_plugin.zip  */SourceCode.zip

cd .. 
#out of source code into plugin directory

cd .. 
# into main directory 

cp -r build_everything_windows.sh build_everything_linux.sh $TOP/SourceCode

cd $TOP/SourceCode

cd ..

zip -r SourceCode SourceCode/

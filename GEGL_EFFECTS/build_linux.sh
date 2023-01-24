#!/bin/bash


meson setup --buildtype=release build && ninja -C build

cd ./compile_innerglow_here
./build_linux.sh

cd ../compile_bevel_here
./build_linux.sh

cd ../

mv ./build/gegleffects.so ~/.local/share/gegl-0.4/plug-ins/gegleffects.so
mv ./compile_innerglow_here/build/inner-glow.so ~/.local/share/gegl-0.4/plug-ins/inner-glow.so
mv ./compile_bevel_here/build/bevel.so ~/.local/share/gegl-0.4/plug-ins/bevel.so

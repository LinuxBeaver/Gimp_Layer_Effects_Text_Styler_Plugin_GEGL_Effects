#!/bin/bash


meson setup --buildtype=release build && ninja -C build

cd ./compile_innerglow_here
./build_linux.sh

cd ../compile_bevel_here
./build_linux.sh

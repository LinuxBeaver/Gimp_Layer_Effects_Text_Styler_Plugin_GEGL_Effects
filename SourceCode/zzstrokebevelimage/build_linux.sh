#!/bin/bash


meson setup --buildtype=release build && ninja -C build

#!/bin/bash

for dir in *
do
    if test -f "$dir/build_linux.sh"
    then
      echo "building in $dir"
     ( cd "$dir" && bash ./build_linux.sh ) || exit 1
    fi
done

  mkdir -p LinuxBinaries
  mv $(find . -name '*.so') LinuxBinaries/


